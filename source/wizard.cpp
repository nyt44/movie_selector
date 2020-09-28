#include "wizard.hpp"

Wizard::Wizard(Configuration& config, QWidget* parent)
  : QWizard{parent},
    series_selector_page_{std::make_unique<SeriesSelectorWizardPage>(config, parent)},
    episode_selector_page_{std::make_unique<EpisodeSelectorWizardPage>(config, parent)}
{
  addPage(series_selector_page_.get());
  addPage(episode_selector_page_.get());

  setWindowTitle(tr("Movie Selector"));
  setButtonText(QWizard::FinishButton, tr("Play"));

  QObject::connect(this, SIGNAL(currentIdChanged(int)),
                   episode_selector_page_.get(), SLOT(UpdateEpisodeList(int)));
}

void Wizard::accept()
{
}
