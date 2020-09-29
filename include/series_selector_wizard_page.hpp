#pragma once

#include <memory>
#include <vector>

#include <QWizardPage>
#include <QGroupBox>
#include <QRadioButton>
#include <QBoxLayout>
#include <QButtonGroup>

#include "configuration.hpp"

class SeriesSelectorWizardPage : public QWizardPage
{
  Q_OBJECT
  friend class Wizard;
public:
  SeriesSelectorWizardPage(Configuration& config, QWidget* parent = 0);

private:
  QGroupBox * createGui();

  Configuration& config_;
  std::unique_ptr<QVBoxLayout> vbox_;
  std::unique_ptr<QGroupBox> series_group_;
  std::vector<std::unique_ptr<QRadioButton>> radio_buttons_;
  std::unique_ptr<QVBoxLayout> radio_layout_;
  std::unique_ptr<QButtonGroup> button_group_;
};
