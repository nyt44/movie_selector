#include "singleton.h"

#include <fstream>
#include <mutex>

struct Singleton::Pimpl
{
    Pimpl();

    std::ofstream err_stream_;
    SeriesChoice series_choice_;
    std::mutex choice_mutex_;
    std::string search_str_;
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
    std::string copied_temp_str;
    {
        std::lock_guard<std::mutex> _(pimpl_->choice_mutex_);
        pimpl_->series_choice_ = SeriesChoice::kPenguins;
        copied_temp_str = pimpl_->search_str_;
    }
    emit seriesTypeChangedSignal(copied_temp_str);
}

void Singleton::setCwSlot()
{
    std::string copied_temp_str;
    {
        std::lock_guard<std::mutex> _(pimpl_->choice_mutex_);
        pimpl_->series_choice_ = SeriesChoice::kCloneWars;
        copied_temp_str = pimpl_->search_str_;
    }
    emit seriesTypeChangedSignal(copied_temp_str);
}

void Singleton::setRebelsSlot()
{
    std::string copied_temp_str;
    {
        std::lock_guard<std::mutex> _(pimpl_->choice_mutex_);
        pimpl_->series_choice_ = SeriesChoice::kRebels;
        copied_temp_str = pimpl_->search_str_;
    }
    emit seriesTypeChangedSignal(copied_temp_str);
}

void Singleton::setMandalorianSlot()
{
    std::string copied_temp_str;
    {
        std::lock_guard<std::mutex> _(pimpl_->choice_mutex_);
        pimpl_->series_choice_ = SeriesChoice::kMandalorian;
        copied_temp_str = pimpl_->search_str_;
    }
    emit seriesTypeChangedSignal(copied_temp_str);
}

void Singleton::setWitcherSlot()
{
    std::string copied_temp_str;
    {
        std::lock_guard<std::mutex> _(pimpl_->choice_mutex_);
        pimpl_->series_choice_ = SeriesChoice::kWither;
        copied_temp_str = pimpl_->search_str_;
    }
    emit seriesTypeChangedSignal(copied_temp_str);
}

void Singleton::updateSearchStrSlot(const QString & search_str)
{
    std::lock_guard<std::mutex> _(pimpl_->choice_mutex_);
    pimpl_->search_str_ = search_str.toStdString();
}

//Private functions
Singleton::Pimpl::Pimpl() : series_choice_(SeriesChoice::kPenguins)
{
}
