#include "episode_runner.hpp"

#include <filesystem>
#include <windows.h>
#include <cstdio>
#include <locale>
#include <codecvt>

namespace
{

std::wstring ConvertPathToWstring(std::filesystem::path path)
{
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.from_bytes(path.make_preferred().string());
}

} // namespace

EpisodeRunner::EpisodeRunner(Configuration& config) : config_{config} {}

void EpisodeRunner::run(std::string_view epidode, std::string_view subtitles)
{
  auto vlc_path = ConvertPathToWstring(config_.GetVlcPath());
  auto episode_path = ConvertPathToWstring(epidode);

  std::wstring command = L"\"" + vlc_path + L"\" \"" + episode_path + L"\"";

  if (!subtitles.empty())
  {
    auto subtitle_path = ConvertPathToWstring(subtitles);
    command += L" --sub-file=\"" + subtitle_path + L"\"";
  }

  STARTUPINFO startup_info;
  PROCESS_INFORMATION process_info;
  ZeroMemory(&process_info, sizeof(process_info));
  ZeroMemory(&startup_info, sizeof(startup_info));

  if (CreateProcess(NULL, command.data(), NULL, NULL, TRUE, 0, NULL, NULL, &startup_info, &process_info)) {
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);
  }
}

