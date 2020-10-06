#include "episode_runner.hpp"

#include <stdexcept>
#include <string>

#include <unistd.h>

EpisodeRunner::EpisodeRunner(Configuration& config) : config_{config} {}

void EpisodeRunner::run(std::string_view path)
{
  std::string program_name{config_.GetVlcPath()};
  std::string episode_path{path};
  char* const argv[] = {program_name.data(), episode_path.data(), nullptr};

  pid_t child_pid = fork();
  if (child_pid == 0) {
    //Child process
    execv(argv[0], argv);
    throw std::runtime_error{"Unknown command"};
  }
}
