#pragma once

#include <string>
#include <vector>
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
    void getIdVector(std::vector<uint16_t> & result) const;
    void pushBackId(uint16_t id);
    void clearIdVector();
private:
    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};
