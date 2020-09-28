#pragma once

#include <string>
#include <map>
#include <vector>
#include <future>

#include "configuration.hpp"

class MapCollector
{
 public:
  struct EpisodeInfo
  {
    std::string episode_description;
    std::string path_to_episode;
    std::string subtitle_path;
  };
  using SeriesMap = std::map<std::string, EpisodeInfo>;

  explicit MapCollector(Configuration& config);

  const SeriesMap& GetMap(int idx);

 private:
  Configuration& config_;
  std::vector<std::future<SeriesMap>> maps_getters_;
  std::vector<SeriesMap> series_maps_;
};
