#pragma once

#include <string_view>

#include "configuration.hpp"

class EpisodeRunner
{
 public:
  EpisodeRunner(Configuration& config);

  void run(std::string_view path, std::string_view subtitles);

 private:
  Configuration& config_;
};
