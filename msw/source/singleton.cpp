#include "singleton.h"

#include <fstream>

struct Singleton::Pimpl
{
    Pimpl() {}

    std::ofstream err_stream_;
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


void Singleton::updateSignal(std::vector<std::string> *cw_matched)
{
    emit cloneWarsInitialized(cw_matched);
}
