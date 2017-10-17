#include "mainwizardpage.h".h"

struct MainWizardPage::Pimpl
{
  //TODO
};

MainWizardPage::MainWizardPage(QWidget *parent)
    : QWizardPage(parent)
{
    setWindowTitle(tr("Episode selector"));
}
