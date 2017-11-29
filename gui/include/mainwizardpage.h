#pragma once

#include <QWizardPage>

#include <memory>
#include <string>

class MainWizardPage : public QWizardPage
{
    Q_OBJECT
public:
    MainWizardPage(QWidget *parent = 0);
    ~MainWizardPage();
    std::string getSelectedEpisode() const;
    std::string descToPath(const std::string & desc) const;

public slots:
    void updateEpisodeList();
private:
    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};
