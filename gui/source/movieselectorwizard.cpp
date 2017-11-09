#include "movieselectorwizard.h"

#include "mainwizardpage.h"
#include "seriesselectorwizardpage.h"
#include "singleton.h"

struct MovieSelectorWizard::Pimpl
{
    Pimpl();

    std::unique_ptr<SeriesSelectorWizardPage> series_selector_page_;
    std::unique_ptr<MainWizardPage> episode_selector_page_;
};

//Public methods

MovieSelectorWizard::MovieSelectorWizard(QWidget *parent)
    : QWizard(parent),
      pimpl_(std::make_unique<Pimpl>())
{

    addPage(pimpl_->series_selector_page_.get());
    addPage(pimpl_->episode_selector_page_.get());

    setWindowTitle(tr("Movie Selector"));

    Singleton & singleton = Singleton::getOnlyInstance();
    QObject::connect(&singleton, SIGNAL(cloneWarsInitialized(std::vector<std::string> *)),
                     pimpl_->episode_selector_page_.get(), SLOT(updateEpisodeList(std::vector<std::string> *)));

}

MovieSelectorWizard::~MovieSelectorWizard()
{
}

void MovieSelectorWizard::accept()
{
    //TODO
}

//Private functions

MovieSelectorWizard::Pimpl::Pimpl()
    : series_selector_page_(std::make_unique<SeriesSelectorWizardPage>()),
      episode_selector_page_(std::make_unique<MainWizardPage>())
{
}
