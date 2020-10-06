#pragma once

#include <string_view>

#include "configuration.hpp"

class EpisodeRunner
{
 public:
  EpisodeRunner(Configuration& config);

  void run(std::string_view path);

 private:
  Configuration& config_;
};
