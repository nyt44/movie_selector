#include "movieselectorwizard.h"
#include <QApplication>

#include "singleton.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MovieSelectorWizard wizard;
    wizard.show();

    return app.exec();
}
