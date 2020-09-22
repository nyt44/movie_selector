#include "series_selector_wizard_page.hpp"


SeriesSelectorWizardPage::SeriesSelectorWizardPage(Configuration& config, QWidget* parent)
  : QWizardPage(parent), config_{config}
{
  setWindowTitle(tr("Series selector"));

  vbox_ = std::make_unique<QVBoxLayout>();
  vbox_->addWidget(createGui());

  setLayout(vbox_.get());
}

QGroupBox * SeriesSelectorWizardPage::createGui()
{
  series_group_ = std::make_unique<QGroupBox>(tr("Please select the series."));

  auto radios_count = config_.GetSeriesCount();
  for (int i = 0; i < radios_count; ++i)
  {
    radio_buttons_.emplace_back(std::make_unique<QRadioButton>(config_.GetSeriesName(i)));
  }
  radio_buttons_.at(0)->setChecked(true);

  radio_layout_ = std::make_unique<QVBoxLayout>();
  for (int i = 0; i < radios_count; ++i)
  {
    radio_layout_->addWidget(radio_buttons_.at(i).get());
  }

  series_group_->setLayout(radio_layout_.get());
  return series_group_.get();
}
