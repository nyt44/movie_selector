#include "series_data_calculator.hpp"

#include <string_view>
#include <filesystem>
#include <fstream>
#include <exception>
#include <regex>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

namespace
{
std::vector<std::string> GetDescriptions(const fs::path& desc_file_path)
{
  std::vector<std::string> result;

  std::ifstream desc_file(desc_file_path);
  if (!desc_file.is_open())
  {
    std::string err_msg = "Cannot open " + desc_file_path.string();
    throw std::runtime_error{err_msg};
  }

  std::string desc_line;
  std::getline(desc_file, desc_line);
  while (desc_file)
  {
    auto cr_pos = desc_line.find('\r');
    if (cr_pos != std::string::npos)
    {
        desc_line.erase(cr_pos);
    }

    if (!desc_line.empty())
    {
        result.emplace_back(desc_line);
    }
    std::getline(desc_file, desc_line);
  }
  return result;
}

SeriesDataCalculator::EpisodeInfo SearchForEpisodeAndSubtitlePaths(const std::string& shortened,
                                                           const fs::path& series_dir_path,
                                                           const std::string& desc)
{
  std::regex file_pattern(".*" + shortened + ".*", std::regex_constants::icase);
  std::regex sub_pattern{".*" + shortened + ".*\\.txt", std::regex_constants::icase};
  bool file_matched = false;
  bool sub_matched = false;
  std::string curr_path_str;
  std::string file_path_str;
  std::string sub_path_str;

  for (const auto & path : std::filesystem::recursive_directory_iterator(series_dir_path))
  {
    curr_path_str = path.path().u8string();
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

  if (!file_matched)
  {
    std::string err_msg = "\"" + desc + "\" - corresponding file not found";
    throw std::runtime_error{err_msg};
  }
  return SeriesDataCalculator::EpisodeInfo{desc, file_path_str, sub_path_str};;
}

SeriesDataCalculator::SeriesData CalculateSeriesData(std::string_view root_dir, std::string_view series_dir)
{
  SeriesDataCalculator::SeriesData result;

  fs::path desc_file_path{fs::path{root_dir} / series_dir / "opis odcinkow.txt"};
  auto descriptions = GetDescriptions(desc_file_path);

  fs::path series_dir_path{fs::path{root_dir} / series_dir};
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
      continue;
    }

    result.emplace_back(SearchForEpisodeAndSubtitlePaths(shortened, series_dir_path, desc));
  }

  return result;
}

} // namespace

SeriesDataCalculator::SeriesDataCalculator(Configuration& config) : config_{config}
{
  auto number_of_series = config_.GetSeriesCount();
  data_getters_.reserve(number_of_series);
  all_series_data_.reserve(number_of_series);

  for (int i = 0; i < number_of_series; ++i)
  {
    data_getters_.emplace_back(std::async(std::launch::async, CalculateSeriesData, config_.GetRootDir(),
                                          config_.GetSeriesDirName(i)));
    all_series_data_.emplace_back();
  }
}

const SeriesDataCalculator::SeriesData& SeriesDataCalculator::GetSeriesData(int idx)
{
  if (data_getters_[idx].valid())
  {
    all_series_data_[idx] = data_getters_[idx].get();
  }
  return all_series_data_[idx];
}
