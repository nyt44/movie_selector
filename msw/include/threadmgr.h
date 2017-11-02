#pragma once

#include <memory>

class ThreadMgr
{
public:
    ThreadMgr();
    ~ThreadMgr();

private:
    ThreadMgr(const ThreadMgr &) = delete;
    ThreadMgr & operator=(const ThreadMgr &) = delete;
    ThreadMgr & operator=(ThreadMgr &&) = delete;

    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};
