#pragma once

#include <string>
#include <cstdint>
#include <memory>

class SeriesDataKeeper
{
  public:
    SeriesDataKeeper();
    ~SeriesDataKeeper();
    void pushBackEpisode(std::string && path, std::string && desc);
    std::string getDesc(uint16_t id) const;
    std::string getPath(uint16_t id) const;
    std::string getPath(const std::string & desc);
    bool getId(uint16_t & res) const;
    void pushBackId(uint16_t id);
    void startIdWriting();
    void stopIdWriting();
    void startIdReading();
    void stopIdReading();
    uint16_t mapSize() const;
private:
    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};
