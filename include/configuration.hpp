#pragma once
#include <string>
#include <string_view>
#include <vector>


class Configuration
{
public:
  Configuration(const char* json_path);

  std::string_view GetRootDir() const;
  std::string_view GetVlcPath() const;
  const char* GetSeriesName(int idx) const;
  std::string_view GetSeriesDirName(int idx) const;
  int GetSeriesCount() const;

private:
  std::string root_dir_;
  std::string vlc_path_;
  std::vector<std::string> series_names_;
  std::vector<std::string> series_dir_names_;
};

