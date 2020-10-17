#include "episode_runner.hpp"

#include <stdexcept>
#include <string>

#include <unistd.h>

EpisodeRunner::EpisodeRunner(Configuration& config) : config_{config} {}

void EpisodeRunner::run(std::string_view epidode, std::string_view subtitles)
{
  std::string program_name{config_.GetVlcPath()};
  std::string episode_path{epidode};
  char* argv[4] = {0};
  argv[0] = program_name.data();
  argv[1] = episode_path.data();

  std::string subtitles_option;
  if (!subtitles.empty())
  {
    subtitles_option = "--sub-file=" + std::string{subtitles};
    argv[2] = subtitles_option.data();
  }
  else
  {
    argv[2] = nullptr;
  }

  pid_t child_pid = fork();
  if (child_pid == 0) {
    //Child process
    execv(argv[0], argv);
    throw std::runtime_error{"Unknown command"};
  }
}
