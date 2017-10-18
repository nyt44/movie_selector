#pragma once

#include <QWizardPage>

#include <memory>

class SeriesSelectorWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    SeriesSelectorWizardPage(QWidget *parent = 0);
    ~SeriesSelectorWizardPage();

private:
    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};
