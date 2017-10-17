#include "movieselectorwizard.h"

#include "mainwizardpage.h"
#include "seriesselectorwizardpage.h"

MovieSelectorWizard::MovieSelectorWizard(QWidget *parent)
    : QWizard(parent),
      series_selector_page_(std::make_unique<SeriesSelectorWizardPage>()),
      episode_selector_page_(std::make_unique<MainWizardPage>())
{

    addPage(series_selector_page_.get());
    addPage(episode_selector_page_.get());

    setWindowTitle(tr("Movie Selector"));
}

void MovieSelectorWizard::accept()
{
    //TODO
}
