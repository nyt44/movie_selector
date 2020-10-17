#pragma once
#include <memory>
#include <string_view>
#include <utility>

#include <QWizardPage>
#include <QBoxLayout>
#include <QListView>
#include <QStringListModel>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include "configuration.hpp"
#include "map_collector.hpp"

class EpisodeSelectorWizardPage : public QWizardPage
{
  Q_OBJECT
public:
  EpisodeSelectorWizardPage(Configuration& config, QWidget *parent = 0);
  std::pair<std::string_view, std::string_view> GetPathToSelectedEpisode();

public slots:
  void UpdateEpisodeList(int page_id);
  void SetCurrentId(int current_id);
  void FilterEpisodeList(const QString& filter);

private:
  QGroupBox * createForm();
  QListView * createEpisodesList();

  Configuration& config_;
  MapCollector map_collector_;
  int current_id_;

  std::unique_ptr<QVBoxLayout> vbox_;
  std::unique_ptr<QGroupBox> form_group_box_;
  std::unique_ptr<QLabel> search_label_;
  std::unique_ptr<QLineEdit> search_edit_;
  std::unique_ptr<QFormLayout> form_layout_;
  std::unique_ptr<QListView> episodes_list_;
  std::unique_ptr<QStringListModel> episodes_list_model_;
  std::unique_ptr<QStringList> episodes_list_items_;
};
