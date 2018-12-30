#pragma once

class RunnerInterface
{
public:
    virtual ~RunnerInterface() {}
    virtual void run() = 0;
};
