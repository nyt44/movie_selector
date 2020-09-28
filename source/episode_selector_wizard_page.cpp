#include "episode_selector_wizard_page.hpp"

namespace
{
constexpr int kEpsiodeSelectorPageId = 1;
} // namespace

EpisodeSelectorWizardPage::EpisodeSelectorWizardPage(Configuration& config, QWidget *parent)
  : QWizardPage(parent), config_{config}, map_collector_{config}, current_id_{0}
{
  setWindowTitle(tr("Episode selector"));

  vbox_ = std::make_unique<QVBoxLayout>();
  vbox_->addWidget(createForm());
  vbox_->addWidget(createEpisodesList());

  setLayout(vbox_.get());
}

void EpisodeSelectorWizardPage::UpdateEpisodeList(int page_id)
{
  if (page_id == kEpsiodeSelectorPageId)
  {
    episodes_list_model_->removeRows(0, episodes_list_model_->rowCount());
    episodes_list_items_->clear();

    const auto& current_series_map = map_collector_.GetMap(current_id_);

    for (const auto& [unused, episode_info] : current_series_map)
    {
      episodes_list_items_->append(tr(episode_info.episode_description.c_str()));
    }

    episodes_list_model_->setStringList(*(episodes_list_items_));
  }
}

QGroupBox * EpisodeSelectorWizardPage::createForm()
{
  form_group_box_ = std::make_unique<QGroupBox>();

  search_label_ = std::make_unique<QLabel>(tr("Search: "));
  search_edit_ = std::make_unique<QLineEdit>();

  form_layout_ = std::make_unique<QFormLayout>();
  form_layout_->addRow(search_label_.get(), search_edit_.get());

  form_group_box_->setLayout(form_layout_.get());

  return form_group_box_.get();
}

QListView * EpisodeSelectorWizardPage::createEpisodesList()
{

  episodes_list_ = std::make_unique<QListView>();
  episodes_list_->setEditTriggers(QAbstractItemView::NoEditTriggers);
  episodes_list_items_ = std::make_unique<QStringList>();
  episodes_list_model_ = std::make_unique<QStringListModel>(*episodes_list_items_, nullptr);
  episodes_list_->setModel(episodes_list_model_.get());

  return episodes_list_.get();
}
