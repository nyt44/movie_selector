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
    ~MovieSelectorWizard();
    virtual void accept() override;
    virtual void reject() override;
private:
    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};
