#include "seriesselectorwizardpage.h"

#include <QGroupBox>
#include <QRadioButton>
#include <QBoxLayout>

#include <memory>

#include "singleton.h"

using std::unique_ptr;
using std::make_unique;

struct SeriesSelectorWizardPage::Pimpl
{
  QGroupBox * createGui();

  unique_ptr<QVBoxLayout> vbox_;
  unique_ptr<QGroupBox> series_group_;
  unique_ptr<QRadioButton> penguins_radio_;
  unique_ptr<QRadioButton> clone_wars_radio_;
  unique_ptr<QRadioButton> rebels_radio_;
  unique_ptr<QVBoxLayout> radio_layout_;
};

//Public Methods

SeriesSelectorWizardPage::SeriesSelectorWizardPage(QWidget *parent)
    : QWizardPage(parent)
{
    setWindowTitle(tr("Series selector"));

    pimpl_ = make_unique<Pimpl>();
    pimpl_->vbox_ = make_unique<QVBoxLayout>();
    pimpl_->vbox_->addWidget(pimpl_->createGui());

    setLayout(pimpl_->vbox_.get());

    Singleton & singleton = Singleton::getOnlyInstance();
    connect(pimpl_->penguins_radio_.get(), SIGNAL(clicked()), &singleton, SLOT(setPenguinsSlot()));
    connect(pimpl_->clone_wars_radio_.get(), SIGNAL(clicked()), &singleton, SLOT(setCwSlot()));
    connect(pimpl_->rebels_radio_.get(), SIGNAL(clicked()), &singleton, SLOT(setRebelsSlot()));
}

SeriesSelectorWizardPage::~SeriesSelectorWizardPage()
{
}

//Private functions

QGroupBox * SeriesSelectorWizardPage::Pimpl::createGui()
{
    series_group_ = make_unique<QGroupBox>(tr("Please select the series."));

    penguins_radio_ = make_unique<QRadioButton>(tr("The Penguins of Madagascar"));
    clone_wars_radio_ = make_unique<QRadioButton>(tr("Star Wars: The Clone Wars"));
    rebels_radio_ = make_unique<QRadioButton>(tr("Star Wars: Rebels"));
    penguins_radio_->setChecked(true);

    radio_layout_ = make_unique<QVBoxLayout>();
    radio_layout_->addWidget(penguins_radio_.get());
    radio_layout_->addWidget(clone_wars_radio_.get());
    radio_layout_->addWidget(rebels_radio_.get());

    series_group_->setLayout(radio_layout_.get());

    return series_group_.get();
}
