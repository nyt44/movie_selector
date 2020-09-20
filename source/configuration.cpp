#include "configuration.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

Configuration::Configuration(const char* json_path)
{
  boost::property_tree::ptree tree;
  boost::property_tree::json_parser::read_json(json_path, tree);

  root_dir_ = tree.get_child("root_dir").get_value<std::string>();

  const auto& series_tree = tree.get_child("series");
  for (const auto& series : series_tree)
  {
    const auto& name = series.second.get_child("name");
    series_names_.emplace_back(name.get_value<std::string>());

    const auto& dir_name = series.second.get_child("dir_name");
    series_dir_names_.emplace_back(dir_name.get_value<std::string>());
  }
}

std::string_view Configuration::GetRootDir() const
{
  return root_dir_;
}

std::string_view Configuration::GetSeriesName(int idx) const
{
  return series_names_.at(idx);
}

std::string_view Configuration::GetSeriesDirName(int idx) const
{
  return series_dir_names_.at(idx);
}

int Configuration::GetSeriesCount() const
{
  return series_names_.size();
}
