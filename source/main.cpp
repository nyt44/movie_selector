#include <QApplication>

#include "configuration.hpp"
#include "wizard.hpp"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  Configuration config{"input.json"};
  Wizard wizard{config};

  wizard.resize(800, 600);
  wizard.show();

  return app.exec();
}
