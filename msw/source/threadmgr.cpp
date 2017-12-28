#include "threadmgr.h"

#include "singleton.h"

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
#include <queue>
#include <atomic>
#include <chrono>
#include <algorithm>

namespace fs = std::experimental::filesystem;

//const std::string kBasePath = "..\\";
const std::string kBasePath = "E:\\dysk\\seriale\\";
//const std::string kBasePath = "F:\\seriale\\";

struct Task
{
    SeriesDataKeeper * data_keeper;
    std::string pattern;
};

struct ThreadMgr::Pimpl
{
    Pimpl();
    ~Pimpl();
    void initFunc(SeriesDataKeeper * data_keeper);
    void penInitFunc();
    void worker();
    void setCwRebPathsMap(SeriesDataKeeper * data_keeper, std::vector<std::string> & descs);
    void setIds(SeriesDataKeeper * data_keeper);
    void process(const Task & task);
    void addNewTask(const std::string & new_text);


    Singleton & singleton_;
    std::unique_ptr<CwDataKeeper> cw_data_keeper_;
    std::unique_ptr<RebDataKeeper> reb_data_keeper_;
    std::unique_ptr<PenDataKeeper> pen_data_keeper_;
    bool ready_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread work_thread_;
    std::queue<Task> task_queue_;
    std::atomic<bool> stopped_;

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

void ThreadMgr::stop()
{
    pimpl_->stopped_ = true;
}

SeriesDataKeeper * ThreadMgr::getSeriesDataKeeper() const
{
    SeriesChoice series_choice = pimpl_->singleton_.getSeriesChoice();

    if (series_choice == SeriesChoice::kCloneWars)
    {
        return pimpl_->cw_data_keeper_.get();
    }
    else if (series_choice == SeriesChoice::kRebels)
    {
      return pimpl_->reb_data_keeper_.get();
    }
    else
    {
        return pimpl_->pen_data_keeper_.get();
    }
}

//Slots

void ThreadMgr::newTextGivenSlot(const QString & new_text)
{
    pimpl_->addNewTask(new_text.toStdString());
}

void ThreadMgr::seriesTypeChangedSlot(const std::string & search_str)
{
    pimpl_->addNewTask(search_str);
}

//Private functions

ThreadMgr::Pimpl::Pimpl() : singleton_(Singleton::getOnlyInstance()),
                            ready_(false), stopped_(false),
                            work_thread_(&Pimpl::worker, this),
                            cw_data_keeper_(std::make_unique<CwDataKeeper>()),
                            reb_data_keeper_(std::make_unique<RebDataKeeper>()),
                            pen_data_keeper_(std::make_unique<PenDataKeeper>())
{
}

ThreadMgr::Pimpl::~Pimpl()
{
    work_thread_.join();
}

void ThreadMgr::Pimpl::initFunc(SeriesDataKeeper * data_keeper)
{
    std::string in_file_name = kBasePath + data_keeper->subDirName() + "\\opis odcinkow.txt";

    std::ifstream fin(in_file_name.c_str());
    if (!fin.is_open())
    {
        singleton_.logError("Cannot open \"opis odcinkow\" file for " + data_keeper->subDirName());
        return;
    }

    std::string temp_str;
    std::getline(fin, temp_str);
    std::vector<std::string> descriptions;
    while (fin)
    {
        if (!temp_str.empty())
        {
            descriptions.emplace_back(temp_str);
        }
        std::getline(fin, temp_str);
    }

    setCwRebPathsMap(data_keeper, descriptions);
    setIds(data_keeper);


    if (data_keeper->isGivenSeries(singleton_.getSeriesChoice()))
    {
        emit thread_mgr.updateSignal(data_keeper);
    }
}

void ThreadMgr::Pimpl::penInitFunc()
{
    std::vector<std::string>  paths;

    for (auto & path : fs::recursive_directory_iterator(kBasePath + pen_data_keeper_->subDirName()))
    {
        paths.emplace_back(path.path().u8string());
    }

    std::regex movie_name_pattern(".*\\\\(\\d{1,3})_(\\d{1,3})_(.*)\\.mkv");
    std::smatch episode_match;
    std::regex two_underscores("__");
    std::regex one_underscore("_");

    std::remove_if(paths.begin(), paths.end(), [movie_name_pattern](const std::string & path)
    {
        return false == std::regex_match(path, movie_name_pattern);
    });

    std::sort(paths.begin(), paths.end(), [movie_name_pattern](const std::string & lhs, const std::string & rhs)
    {
        std::smatch lhs_match, rhs_match;
        int l_episode, r_episode;

        std::regex_match(lhs, lhs_match, movie_name_pattern);
        std::istringstream(lhs_match[1]) >> l_episode;

        std::regex_match(rhs, rhs_match, movie_name_pattern);
        std::istringstream(rhs_match[1]) >> r_episode;

        return l_episode < r_episode;
    });

    for (auto & path : paths)
    {
        if (std::regex_match(path, episode_match, movie_name_pattern)
                && episode_match.size() == 4)
        {
            std::string description = episode_match[3].str();
            description = std::regex_replace(description, two_underscores, ", ");
            description = std::regex_replace(description, one_underscore, " ");
            description = episode_match[1].str() + ", " + episode_match[2].str() + " - " + description;

            pen_data_keeper_->pushBackEpisode(std::move(path), std::move(description));
        }
    }

    setIds(pen_data_keeper_.get());

    if (pen_data_keeper_->isGivenSeries(singleton_.getSeriesChoice()))
    {
        emit thread_mgr.updateSignal(pen_data_keeper_.get());
    }
}

void ThreadMgr::Pimpl::worker()
{
    {
        std::unique_lock<std::mutex> lk(mutex_);
        cv_.wait(lk, [=]() { return ready_; });
    }

    std::thread clone_wars_init_thread(&ThreadMgr::Pimpl::initFunc, this, cw_data_keeper_.get());
    std::thread rebels_init_thread(&ThreadMgr::Pimpl::initFunc, this, reb_data_keeper_.get());
    std::thread penguins_init_thread(&ThreadMgr::Pimpl::penInitFunc, this);

    clone_wars_init_thread.join();
    rebels_init_thread.join();
    penguins_init_thread.join();

    size_t queue_size;
    Task curr_task;
    while (!stopped_)
    {
        {
            std::lock_guard<std::mutex> _(mutex_);
            queue_size = task_queue_.size();
        }

        if (queue_size > 1)
        {
            std::lock_guard<std::mutex> _(mutex_);
            task_queue_.pop();
            continue;
        }
        else if (queue_size == 1)
        {
            {
                std::lock_guard<std::mutex> _(mutex_);
                curr_task = task_queue_.front();
                task_queue_.pop();
            }
            process(curr_task);
            continue;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

void ThreadMgr::Pimpl::setCwRebPathsMap(SeriesDataKeeper * data_keeper, std::vector<std::string> & descs)
{
    std::deque<std::string>  paths;
    for (auto & path : fs::recursive_directory_iterator(kBasePath + data_keeper->subDirName()))
    {
        paths.emplace_back(path.path().string());
    }
    std::regex desc_entry_pattern("(\\d)\\.(\\d{1,2}) - .*");
    std::smatch season_and_episode_match;

    for (auto desc_it = descs.begin(); desc_it != descs.end(); ++desc_it)
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
            if (std::regex_match(*path_it, file_match, file_pattern)
                    && false == std::regex_match(*path_it, std::regex(".*\\.txt")))
            {
                file_matched = true;
                std::string found_file = file_match[0].str();
                data_keeper->pushBackEpisode(std::move(found_file), std::move(*desc_it));
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

void ThreadMgr::Pimpl::setIds(SeriesDataKeeper * data_keeper)
{
    data_keeper->startIdWriting();

    uint16_t series_amount = data_keeper->mapSize();
    for (auto i = 0u; i < series_amount; ++i)
    {
        data_keeper->pushBackId(i);
    }

    data_keeper->stopIdWriting();
}
void ThreadMgr::Pimpl::process(const Task & task)
{
    SeriesDataKeeper * data_keeper = task.data_keeper;

    std::regex search_pattern{".*" + task.pattern + ".*", std::regex_constants::icase};
    data_keeper->startIdWriting();

    uint16_t series_amount = data_keeper->mapSize();
    for (auto i = 0u; i < series_amount; ++i)
    {
        if (std::regex_match(data_keeper->getDesc(i), search_pattern))
        {
            data_keeper->pushBackId(i);
        }
    }
    data_keeper->stopIdWriting();

    emit thread_mgr.updateSignal(data_keeper);
}
void ThreadMgr::Pimpl::addNewTask(const std::string & new_text)
{
    SeriesChoice series_choice = singleton_.getSeriesChoice();
    Task request;

    if (series_choice == SeriesChoice::kCloneWars)
    {
        request.data_keeper = cw_data_keeper_.get();
    }
    else if (series_choice == SeriesChoice::kRebels)
    {
      request.data_keeper = reb_data_keeper_.get();
    }
    else
    {
        request.data_keeper = pen_data_keeper_.get();
    }
    request.pattern = new_text;

    std::lock_guard<std::mutex> _(mutex_);
    task_queue_.push(request);
}
