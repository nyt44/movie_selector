#pragma once

#include "singleton.h"

#include <string>
#include <cstdint>
#include <memory>

class SeriesDataKeeper
{
  public:
    SeriesDataKeeper();
    virtual ~SeriesDataKeeper();
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
    virtual std::string subDirName() const = 0;
    virtual bool isGivenSeries(SeriesChoice choice) const =0;
private:
    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};

class CwDataKeeper : public SeriesDataKeeper
{
public:
    virtual std::string subDirName() const override;
    virtual bool isGivenSeries(SeriesChoice choice) const override;
};

class RebDataKeeper : public SeriesDataKeeper
{
public:
    virtual std::string subDirName() const override;
    virtual bool isGivenSeries(SeriesChoice choice) const override;
};

class PenDataKeeper : public SeriesDataKeeper
{
public:
    virtual std::string subDirName() const override;
    virtual bool isGivenSeries(SeriesChoice choice) const override;
};
