#pragma once

#include <string>
#include <vector>
#include <future>

#include "configuration.hpp"

class SeriesDataCalculator
{
 public:
  struct EpisodeInfo
  {
    std::string episode_description;
    std::string path_to_episode;
    std::string subtitle_path;
  };
  using SeriesData = std::vector<EpisodeInfo>;

  explicit SeriesDataCalculator(Configuration& config);

  const SeriesData& GetSeriesData(int idx);

 private:
  Configuration& config_;
  std::vector<std::future<SeriesData>> data_getters_;
  std::vector<SeriesData> all_series_data_;
};
