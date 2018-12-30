#include "seriesdatakeeper.h"

#include <unordered_map>
#include <mutex>
#include <atomic>
#include <array>
#include <queue>
#include <utility>

struct SeriesDataKeeper::Pimpl
{
    Pimpl();

    uint16_t curr_id_;
    std::unordered_map<uint16_t, std::pair<std::string, std::string>> all_;
    std::mutex all_mutex_;
    std::array<std::queue<uint16_t>, 3> selected_;
    std::array<std::mutex, 3>  selected_mutex_;
    uint8_t no_queue_to_read_;
    uint8_t no_queue_to_write_;
    uint8_t no_next_read_;
    std::mutex queue_number_mutex_;
    std::atomic<bool> currently_reading_;
    std::atomic<bool> change_read_number_;
};

//Public methods

SeriesDataKeeper::SeriesDataKeeper()
    : pimpl_(std::make_unique<Pimpl>())
{
}
SeriesDataKeeper::~SeriesDataKeeper()
{
}

void SeriesDataKeeper::pushBackEpisode(std::string && path, std::string && desc)
{
    std::lock_guard<std::mutex> _(pimpl_->all_mutex_);
    pimpl_->all_[pimpl_->curr_id_++] = std::pair<std::string, std::string>(std::move(path), std::move(desc));
}

std::string SeriesDataKeeper::getDesc(uint16_t id) const
{
    std::lock_guard<std::mutex> _(pimpl_->all_mutex_);

    auto found =  pimpl_->all_.find(id);

    if (found != pimpl_->all_.end())
    {
        return found->second.second;
    }
    else
    {
        return "";
    }
}

std::string SeriesDataKeeper::getPath(uint16_t id) const
{
    std::lock_guard<std::mutex> _(pimpl_->all_mutex_);

    auto found =  pimpl_->all_.find(id);

    if (found != pimpl_->all_.end())
    {
        return found->second.first;
    }
    else
    {
        return "";
    }
}

std::string SeriesDataKeeper::getPath(const std::string & desc)
{
    auto it = std::find_if(pimpl_->all_.begin(), pimpl_->all_.end(),
                           [desc](const auto & elem) { return desc == elem.second.second; });
    if (it == pimpl_->all_.end())
    {
        return "";
    }
    else
    {
        return it->second.first;
    }
}

bool SeriesDataKeeper::getId(uint16_t & res) const
{
    std::lock_guard<std::mutex> lk(pimpl_->queue_number_mutex_);
    std::lock_guard<std::mutex> _(pimpl_->selected_mutex_[pimpl_->no_queue_to_read_]);

    if (pimpl_->selected_[pimpl_->no_queue_to_read_].empty())
    {
        return false;
    }

    res = pimpl_->selected_[pimpl_->no_queue_to_read_].front();
    pimpl_->selected_[pimpl_->no_queue_to_read_].pop();

    return true;
}

void SeriesDataKeeper::pushBackId(uint16_t id)
{
    std::lock_guard<std::mutex> lk(pimpl_->queue_number_mutex_);
    std::lock_guard<std::mutex> _(pimpl_->selected_mutex_[pimpl_->no_queue_to_write_]);

    pimpl_->selected_[pimpl_->no_queue_to_write_].push(id);
}

void SeriesDataKeeper::startIdWriting()
{
    std::lock_guard<std::mutex> lk(pimpl_->queue_number_mutex_);
    std::lock_guard<std::mutex> _(pimpl_->selected_mutex_[pimpl_->no_queue_to_write_]);

    while (!pimpl_->selected_[pimpl_->no_queue_to_write_].empty())
    {
        pimpl_->selected_[pimpl_->no_queue_to_write_].pop();
    }
}

void SeriesDataKeeper::stopIdWriting()
{
    std::lock_guard<std::mutex> lk(pimpl_->queue_number_mutex_);
    if (pimpl_->currently_reading_)
    {
        pimpl_->change_read_number_ = true;
        std::swap(pimpl_->no_next_read_, pimpl_->no_queue_to_write_);
    }
    else
    {
        std::swap(pimpl_->no_queue_to_read_, pimpl_->no_queue_to_write_);
    }
}
void SeriesDataKeeper::startIdReading()
{
    pimpl_->currently_reading_ = true;
}

void SeriesDataKeeper::stopIdReading()
{
    std::lock_guard<std::mutex> lk(pimpl_->queue_number_mutex_);

    if (pimpl_->change_read_number_)
    {
        std::swap(pimpl_->no_queue_to_read_, pimpl_->no_next_read_);
        pimpl_->change_read_number_ = false;
    }

    pimpl_->currently_reading_ = false;
}
uint16_t SeriesDataKeeper::mapSize() const
{
    return pimpl_->curr_id_;
}

//Private functions

SeriesDataKeeper::Pimpl::Pimpl()
    : curr_id_(0),
      no_queue_to_read_(0),
      no_queue_to_write_(1),
      no_next_read_(2),
      currently_reading_(false),
      change_read_number_(false)
{

}

//CwDataKeeper overriden methods

std::string CwDataKeeper::subDirName() const
{
    return "Clone Wars";
}
bool CwDataKeeper::isGivenSeries(SeriesChoice choice) const
{
    return choice == SeriesChoice::kCloneWars;
}

//RebDataKeeper overriden methods

std::string RebDataKeeper::subDirName() const
{
    return "Rebels";
}
bool RebDataKeeper::isGivenSeries(SeriesChoice choice) const
{
    return choice == SeriesChoice::kRebels;
}

//PenDataKeeper overriden methods

std::string PenDataKeeper::subDirName() const
{
    return "pingwiny z madagaskaru";
}
bool PenDataKeeper::isGivenSeries(SeriesChoice choice) const
{
    return choice == SeriesChoice::kPenguins;
}
