#pragma once

#include <memory>

#include <QWizard>

#include "series_selector_wizard_page.hpp"
#include "episode_selector_wizard_page.hpp"
#include "configuration.hpp"

class Wizard : public QWizard
{
    Q_OBJECT
public:
    Wizard(Configuration& config, QWidget* parent = 0);
    virtual void accept() override;

private:
    std::unique_ptr<SeriesSelectorWizardPage> series_selector_page_;
    std::unique_ptr<EpisodeSelectorWizardPage> episode_selector_page_;
};
