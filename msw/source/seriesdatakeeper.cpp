#include "seriesdatakeeper.h"

#include <unordered_map>
#include <mutex>
#include <atomic>

struct SeriesDataKeeper::Pimpl
{
    Pimpl();

    std::unordered_map<uint16_t, std::pair<std::string, std::string>> all_clone_wars_;
    std::mutex all_mutex_;
    std::vector<uint16_t> selected_clone_wars1_;
    std::vector<uint16_t> selected_clone_wars2_;
    std::mutex selected_mutex1_;
    std::mutex selected_mutex2_;
    std::atomic<uint8_t> no_vector_to_read_;
    std::atomic<uint8_t> no_vector_to_write_;
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
    //TODO
}

std::string SeriesDataKeeper::getDesc(uint16_t id) const
{
    std::string result;

    //TODO

    return result;
}

std::string SeriesDataKeeper::getPath(uint16_t id) const
{
    std::string result;

    //TODO

    return result;
}

void SeriesDataKeeper::getIdVector(std::vector<uint16_t> & result) const
{
    //TODO
}

void SeriesDataKeeper::pushBackId(uint16_t id)
{
    //TODO
}

void SeriesDataKeeper::clearIdVector()
{
    //TODO
}

//Private functions

SeriesDataKeeper::Pimpl::Pimpl()
    : no_vector_to_read_(0), no_vector_to_write_(1)
{

}
