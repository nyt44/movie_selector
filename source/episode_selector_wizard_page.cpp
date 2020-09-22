#include "episode_selector_wizard_page.hpp"

EpisodeSelectorWizardPage::EpisodeSelectorWizardPage(Configuration& config, QWidget *parent)
  : QWizardPage(parent), config_{config}
{
  setWindowTitle(tr("Episode selector"));

  vbox_ = std::make_unique<QVBoxLayout>();
  vbox_->addWidget(createForm());
  vbox_->addWidget(createEpisodesList());

  setLayout(vbox_.get());
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
