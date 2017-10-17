#pragma once

#include <QWizardPage>

#include <memory>

class MainWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    MainWizardPage(QWidget *parent = 0);

private:
    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};
