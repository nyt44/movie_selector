#include "threadmgr.h"

#include "singleton.h"
#include "seriesdatakeeper.h"

#include <QObject>

#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <filesystem>
#include <regex>
#include <sstream>
#include <iomanip>

namespace fs = std::experimental::filesystem;


const std::string kBasePath = "E:\\dysk\\seriale\\";

struct ThreadMgr::Pimpl
{
    Pimpl();
    ~Pimpl();
    void cloneWarsInitFunc();
    void worker();
    void setCwPathsMap(std::vector<std::string> & clone_wars_descs);
    void setCwIds();


    Singleton & singleton_;
    SeriesDataKeeper * series_data_keeper_;
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
    series_data_keeper_ = singleton_.getSeriesDataKeeper();
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
    std::vector<std::string> clone_wars_descriptions;
    while (fin)
    {
        if (!temp_str.empty())
        {
            clone_wars_descriptions.emplace_back(temp_str);
        }
        std::getline(fin, temp_str);
    }

    setCwPathsMap(clone_wars_descriptions);
    setCwIds();

    singleton_.updateSignal();
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

void ThreadMgr::Pimpl::setCwPathsMap(std::vector<std::string> & clone_wars_descs)
{
    std::deque<std::string>  paths;
    for (auto & path : fs::recursive_directory_iterator(kBasePath + "Clone Wars"))
    {
        paths.emplace_back(path.path().string());
    }
    std::regex desc_entry_pattern("(\\d)\\.(\\d{1,2}) - .*");
    std::smatch season_and_episode_match;

    for (auto desc_it = clone_wars_descs.begin(); desc_it != clone_wars_descs.end(); ++desc_it)
    {
        std::string shortened;
        if (std::regex_match(*desc_it, season_and_episode_match, desc_entry_pattern)
                && season_and_episode_match.size() == 3)
        {
            int season;
            int episode;
            std::string season_str = season_and_episode_match[1].str();
            std::string episode_str = season_and_episode_match[2].str();

            std::istringstream (season_str + " " + episode_str) >> season >> episode;

            std::ostringstream oss;
            oss << "S" << std::setw(2) << std::setfill('0') << season
                << "E" << std::setw(2) << std::setfill('0') << episode;
            shortened = oss.str();
        }
        else
        {
            singleton_.logError("\"" + *desc_it + "\" line has invalid format");
            continue;
        }

        std::regex file_pattern(".*" + shortened + ".*");
        std::smatch file_match;
        bool file_matched = false;

        for (auto path_it = paths.begin(); path_it != paths.end(); ++path_it)
        {
            if (std::regex_match(*path_it, file_match, file_pattern))
            {
                file_matched = true;
                std::string found_file = file_match[0].str();
                series_data_keeper_->pushBackEpisode(std::move(found_file), std::move(*desc_it));
                path_it = paths.erase(path_it);
                break;
            }
        }
        if (!file_matched)
        {
            singleton_.logError("\"" + *desc_it + "\" - corresponding file not found");
        }
    }
}

void ThreadMgr::Pimpl::setCwIds()
{
    series_data_keeper_->startIdWriting();

    uint16_t series_amount = series_data_keeper_->mapSize();
    for (auto i = 0u; i < series_amount; ++i)
    {
        series_data_keeper_->pushBackId(i);
    }

    series_data_keeper_->stopIdWriting();
}
