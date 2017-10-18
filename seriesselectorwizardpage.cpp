#include "seriesselectorwizardpage.h"

struct SeriesSelectorWizardPage::Pimpl
{
  //TODO
};

SeriesSelectorWizardPage::SeriesSelectorWizardPage(QWidget *parent)
    : QWizardPage(parent)
{
    setWindowTitle(tr("Series selector"));
}

SeriesSelectorWizardPage::~SeriesSelectorWizardPage()
{
}
