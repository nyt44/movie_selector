#pragma once

#include "runnerinterface.h"

#include <string>

class UnixRunner : public RunnerInterface
{
public:
    UnixRunner(std::string path);
    void run() override;
private:
    std::string dir_;
    std::string command_;
};
