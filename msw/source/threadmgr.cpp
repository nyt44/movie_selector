#include "threadmgr.h"

#include "singleton.h"

#include <QObject>

#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>


const std::string kBasePath = "E:\\dysk\\seriale\\";

struct ThreadMgr::Pimpl
{
    Pimpl();
    ~Pimpl();
    void cloneWarsInitFunc();
    void worker();

    Singleton & singleton_;
    std::vector<std::string> clone_wars_descriptions_;
    bool ready_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread work_thread_;

};

//Public functions

ThreadMgr::ThreadMgr() : pimpl_(std::make_unique<Pimpl>())
{
}

ThreadMgr::~ThreadMgr()
{
}
void ThreadMgr::start()
{
    {
        std::lock_guard<std::mutex> _(pimpl_->mutex_);
        pimpl_->ready_ = true;
    }

    pimpl_->cv_.notify_one();
}

//Private functions

ThreadMgr::Pimpl::Pimpl() : singleton_(Singleton::getOnlyInstance()),
                            ready_(false),
                            work_thread_(&Pimpl::worker, this)
{
}

ThreadMgr::Pimpl::~Pimpl()
{
    work_thread_.join();
}

void ThreadMgr::Pimpl::cloneWarsInitFunc()
{
    std::string in_file_name = "Clone Wars\\opis odcinkow.txt";
    in_file_name = kBasePath + in_file_name;

    std::ifstream fin(in_file_name.c_str());
    if (!fin.is_open())
    {
        singleton_.logError("Cannot open \"opis odcinkow\" file for Clone Wars");
        return;
    }

    std::string temp_str;
    std::getline(fin, temp_str);
    while (fin)
    {
        if (!temp_str.empty())
        {
            clone_wars_descriptions_.emplace_back(temp_str);
        }
        std::getline(fin, temp_str);
    }

    singleton_.updateSignal(&clone_wars_descriptions_);
}
void ThreadMgr::Pimpl::worker()
{
    {
        std::unique_lock<std::mutex> lk(mutex_);
        cv_.wait(lk, [=]() { return ready_; });
    }

    std::thread clone_wars_init_thread(&ThreadMgr::Pimpl::cloneWarsInitFunc, this);

    clone_wars_init_thread.join();
}
