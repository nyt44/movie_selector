#include "episode_runner.hpp"

#include <filesystem>
#include <windows.h>
#include <cstdio>
#include <locale>
#include <codecvt>

EpisodeRunner::EpisodeRunner(Configuration& config) : config_{config} {}

void EpisodeRunner::run(std::string_view path)
{
  std::filesystem::path vlc_path{config_.GetVlcPath()};
  vlc_path.make_preferred();
  std::filesystem::path episode_path{path};
  episode_path.make_preferred();

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::wstring converted_episode_path = converter.from_bytes(episode_path.string());
  std::wstring converted_vlc_path = converter.from_bytes(vlc_path.string());

  std::wstring command = L"\"" + converted_vlc_path + L"\" \"" + converted_episode_path + L"\"";

  STARTUPINFO startup_info;
  PROCESS_INFORMATION process_info;
  ZeroMemory(&process_info, sizeof(process_info));
  ZeroMemory(&startup_info, sizeof(startup_info));

  if (CreateProcess(NULL, command.data(), NULL, NULL, TRUE, 0, NULL, NULL, &startup_info, &process_info)) {
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);
  }
}

