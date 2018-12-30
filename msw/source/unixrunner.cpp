#ifdef __linux__
#include "unixrunner.h"

#include <experimental/filesystem>

#include <glib.h>

UnixRunner::UnixRunner(std::string path)
{
    std::experimental::filesystem::path full_path{path};

    dir_ = full_path.parent_path().string();
    command_ = "vlc \"" + full_path.filename().string() + "\"";
}

void UnixRunner::run()
{
    gchar     cmd_line[256];
    gchar   **argv;
    gint      argp;
    gint      rc = 0;

    command_.copy(cmd_line, command_.size());
    cmd_line[command_.size()] = '\0';

    rc = g_shell_parse_argv (cmd_line, &argp, &argv, nullptr);
    if (!rc)
    {
        g_strfreev (argv);
        return;
    }

    rc = g_spawn_async (dir_.c_str(), argv, nullptr,
                        GSpawnFlags(G_SPAWN_STDOUT_TO_DEV_NULL | G_SPAWN_SEARCH_PATH),
                        nullptr, nullptr, nullptr, nullptr);

    g_strfreev (argv);
}
#endif
