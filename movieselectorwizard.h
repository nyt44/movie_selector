#pragma once

#include "seriesselectorwizardpage.h"
#include "mainwizardpage.h"

#include <QWizard>

#include <memory>

class MovieSelectorWizard : public QWizard
{
    Q_OBJECT
public:
    MovieSelectorWizard(QWidget *parent = 0);
    virtual void accept() override;
private:
    std::unique_ptr<SeriesSelectorWizardPage> series_selector_page_;
    std::unique_ptr<MainWizardPage> episode_selector_page_;
};
