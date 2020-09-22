#pragma once
#include <memory>
#include <string>

#include <QWizardPage>
#include <QBoxLayout>
#include <QListView>
#include <QStringListModel>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include "configuration.hpp"

class EpisodeSelectorWizardPage : public QWizardPage
{
  Q_OBJECT
public:
  EpisodeSelectorWizardPage(Configuration& config, QWidget *parent = 0);

private:
  QGroupBox * createForm();
  QListView * createEpisodesList();

  Configuration& config_;
  std::unique_ptr<QVBoxLayout> vbox_;
  std::unique_ptr<QGroupBox> form_group_box_;
  std::unique_ptr<QLabel> search_label_;
  std::unique_ptr<QLineEdit> search_edit_;
  std::unique_ptr<QFormLayout> form_layout_;
  std::unique_ptr<QListView> episodes_list_;
  std::unique_ptr<QStringListModel> episodes_list_model_;
  std::unique_ptr<QStringList> episodes_list_items_;
};
