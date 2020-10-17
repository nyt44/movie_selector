#include "map_collector.hpp"

#include <string_view>
#include <filesystem>
#include <fstream>
#include <exception>
#include <regex>
#include <sstream>
#include <iomanip>

namespace
{

MapCollector::SeriesMap CalculateSeriesMap(std::string_view root_dir, std::string_view series_dir)
{
  MapCollector::SeriesMap result;

  std::filesystem::path desc_file_path{root_dir};
  desc_file_path /= series_dir;
  desc_file_path /= "opis odcinkow.txt";

  std::ifstream desc_file(desc_file_path);
  if (!desc_file.is_open())
  {
    std::string err_msg = "Cannot open " + desc_file_path.string();
    throw std::runtime_error{err_msg};
  }

  std::string desc_line;
  std::getline(desc_file, desc_line);
  std::vector<std::string> descriptions;
  while (desc_file)
  {
    auto cr_pos = desc_line.find('\r');
    if (cr_pos != std::string::npos)
    {
        desc_line.erase(cr_pos);
    }

    if (!desc_line.empty())
    {
        descriptions.emplace_back(desc_line);
    }
    std::getline(desc_file, desc_line);
  }

  std::filesystem::path series_dir_path{root_dir};
  series_dir_path /= series_dir;
  std::regex desc_entry_pattern("(\\d)\\.(\\d{1,2}) - .*");
  std::smatch season_and_episode_match;

  for (const auto& desc : descriptions)
  {
    std::string shortened;
    if (std::regex_match(desc, season_and_episode_match, desc_entry_pattern)
             && season_and_episode_match.size() == 3)
    {
      auto season = std::stoi(season_and_episode_match[1].str());
      auto episode = std::stoi(season_and_episode_match[2].str());

      std::ostringstream oss;
      oss << "S" << std::setw(2) << std::setfill('0') << season
          << "E" << std::setw(2) << std::setfill('0') << episode;
      shortened = oss.str();
    }
    else
    {
      // line has invalid format
      continue;
    }

    std::regex file_pattern(".*" + shortened + ".*", std::regex_constants::icase);
    std::regex sub_pattern{".*" + shortened + ".*\\.txt", std::regex_constants::icase};
    bool file_matched = false;
    bool sub_matched = false;
    std::string curr_path_str;
    std::string file_path_str;
    std::string sub_path_str;

    for (const auto & path : std::filesystem::recursive_directory_iterator(series_dir_path))
    {
      curr_path_str = path.path().string();
      if (std::regex_match(curr_path_str, sub_pattern))
      {
        sub_path_str = curr_path_str;
        sub_matched = true;
      }
      else if (std::regex_match(curr_path_str, file_pattern))
      {
        file_path_str = curr_path_str;
        file_matched = true;
      }

      if (file_matched && sub_matched)
      {
        break;
      }
    }

    if (file_matched)
    {
      result[shortened] = MapCollector::EpisodeInfo{desc, file_path_str, sub_path_str};
    }
    else
    {
      std::string err_msg = "\"" + desc + "\" - corresponding file not found";
      throw std::runtime_error{err_msg};
    }
  }

  return result;
}

} // namespace

MapCollector::MapCollector(Configuration& config) : config_{config}
{
  auto number_of_series = config_.GetSeriesCount();
  maps_getters_.reserve(number_of_series);
  series_maps_.reserve(number_of_series);

  for (int i = 0; i < number_of_series; ++i)
  {
    maps_getters_.emplace_back(std::async(std::launch::async, CalculateSeriesMap, config_.GetRootDir(),
                                          config_.GetSeriesDirName(i)));
    series_maps_.emplace_back();
  }
}

const MapCollector::SeriesMap& MapCollector::GetMap(int idx)
{
  if (maps_getters_[idx].valid())
  {
    series_maps_[idx] = maps_getters_[idx].get();
  }
  return series_maps_[idx];
}
