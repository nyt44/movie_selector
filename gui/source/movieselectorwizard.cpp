#include "movieselectorwizard.h"

#include "mainwizardpage.h"
#include "seriesselectorwizardpage.h"
#include "threadmgr.h"

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

    QObject::connect(&thread_mgr, SIGNAL(updateSignal(SeriesDataKeeper*)),
                     pimpl_->episode_selector_page_.get(), SLOT(updateEpisodeList(SeriesDataKeeper*)));

}

MovieSelectorWizard::~MovieSelectorWizard()
{
}

void MovieSelectorWizard::accept()
{
    std:: string sel_desc =pimpl_->episode_selector_page_->getSelectedEpisode();
    std::string sel_path = pimpl_->episode_selector_page_->descToPath(sel_desc);
    sel_path = "\"" + sel_path +"\"";
    //TODO: replace system() with ShellExecute or sth like that
    system(sel_path.c_str());
}
void MovieSelectorWizard::reject()
{
    thread_mgr.stop();
    QWizard::reject();
}

//Private functions

MovieSelectorWizard::Pimpl::Pimpl()
    : series_selector_page_(std::make_unique<SeriesSelectorWizardPage>()),
      episode_selector_page_(std::make_unique<MainWizardPage>())
{
}
