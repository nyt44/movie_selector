#include "singleton.h"

#include <fstream>
#include <mutex>

struct Singleton::Pimpl
{
    Pimpl();

    std::ofstream err_stream_;
    SeriesChoice series_choice_;
    std::mutex choice_mutex_;
};

//Public methods
Singleton::Singleton() : pimpl_(std::make_unique<Pimpl>())
{
}

Singleton::~Singleton()
{
}

Singleton & Singleton::getOnlyInstance()
{
    static Singleton instance;
    return instance;
}

bool Singleton::logError(const std::string & msg) const
{
    if (!pimpl_->err_stream_.is_open())
    {
        pimpl_->err_stream_.open("err.txt");

        if (!pimpl_->err_stream_.is_open())
        {
            return false;
        }
    }

    pimpl_->err_stream_ << msg << std::endl;

    return true;
}

SeriesChoice Singleton::getSeriesChoice() const
{
    std::lock_guard<std::mutex> _(pimpl_->choice_mutex_);
    return pimpl_->series_choice_;
}

//Public slots

void Singleton::setPenguinsSlot()
{
    std::lock_guard<std::mutex> _(pimpl_->choice_mutex_);
    pimpl_->series_choice_ = SeriesChoice::kPenguins;
}

void Singleton::setCwSlot()
{
    std::lock_guard<std::mutex> _(pimpl_->choice_mutex_);
    pimpl_->series_choice_ = SeriesChoice::kCloneWars;
}

void Singleton::setRebelsSlot()
{
    std::lock_guard<std::mutex> _(pimpl_->choice_mutex_);
    pimpl_->series_choice_ = SeriesChoice::kRebels;
}

//Private functions
Singleton::Pimpl::Pimpl() : series_choice_(SeriesChoice::kPenguins)
{
}
