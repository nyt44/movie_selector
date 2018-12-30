#pragma once

#include "runnerinterface.h"

#include <string>

class WindowsRunner : public RunnerInterface
{
public:
    WindowsRunner(std::string path);
    void run() override;
private:
    std::string path_;
};
