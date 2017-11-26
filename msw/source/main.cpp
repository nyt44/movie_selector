#include "movieselectorwizard.h"
#include <QApplication>

#include "singleton.h"
#include "threadmgr.h"

//Global variables
ThreadMgr thread_mgr;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MovieSelectorWizard wizard;
    wizard.resize(800, 600);

    thread_mgr.start();

    wizard.show();

    return app.exec();
}
