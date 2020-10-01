#include "episode_runner.hpp"

#include <stdexcept>

#include <unistd.h>

void EpisodeRunner::run(std::string path)
{
  char program_name[] ="/usr/bin/vlc";
  char* const argv[] = {program_name, path.data(), nullptr};

  pid_t child_pid = fork();
  if (child_pid == 0) {
    //Child process
    execv(argv[0], argv);
    throw std::runtime_error{"Unknown command"};
  }
}
