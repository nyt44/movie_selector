#include "threadmgr.h"

#include "singleton.h"

#include <QObject>

#include <thread>
#include <fstream>
#include <string>
#include <vector>


const std::string kBasePath = "E:\\dysk\\seriale\\";

ThreadMgr::~ThreadMgr()
{
}

struct ThreadMgr::Pimpl
{
    Pimpl() : clone_wars_init_thread_(&Pimpl::cloneWarsInitFunc, this),
              singleton_(Singleton::getOnlyInstance())
    {
//        clone_wars_init_thread_.join();
    }
    ~Pimpl()
    {
    }

    std::thread clone_wars_init_thread_;
    std::vector<std::string> clone_wars_descriptions_;
    Singleton & singleton_;

    void cloneWarsInitFunc()
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

        while (fin)
        {
            std::getline(fin, temp_str);
            if (!temp_str.empty())
            {
                clone_wars_descriptions_.emplace_back(temp_str);
            }
        }
        singleton_.updateSignal(&clone_wars_descriptions_);
    }
};

ThreadMgr::ThreadMgr() : pimpl_(std::make_unique<Pimpl>())
{

}
