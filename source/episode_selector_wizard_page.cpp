#include "episode_selector_wizard_page.hpp"

#include <algorithm>
#include <regex>
#include <vector>

namespace
{
constexpr int kEpsiodeSelectorPageId = 1;
} // namespace

EpisodeSelectorWizardPage::EpisodeSelectorWizardPage(Configuration& config, QWidget *parent)
  : QWizardPage(parent), config_{config}, series_data_calculator_{config}, current_id_{0}
{
  setWindowTitle(tr("Episode selector"));

  vbox_ = std::make_unique<QVBoxLayout>();
  vbox_->addWidget(CreateForm());
  vbox_->addWidget(CreateEpisodesList());

  setLayout(vbox_.get());

  QObject::connect(search_edit_.get(), SIGNAL(textChanged(const QString&)),
                       this, SLOT(FilterEpisodeList(const QString&)));
}

std::pair<std::string_view, std::string_view> EpisodeSelectorWizardPage::GetPathToSelectedEpisode()
{
  auto index = episodes_list_->currentIndex();
  auto item_text = index.data(Qt::DisplayRole).toString().toStdString();

  const auto& current_series_data = series_data_calculator_.GetSeriesData(current_id_);
  auto found_element = std::find_if(current_series_data.begin(), current_series_data.end(), [&item_text](const auto& item)
  {
    return item.episode_description == item_text;
  });
  if (found_element == current_series_data.end())
  {
    throw std::runtime_error{"Cannot find selected episode description"};
  }
  return std::make_pair<std::string_view, std::string_view>(found_element->path_to_episode,
                                                            found_element->subtitle_path);
}

void EpisodeSelectorWizardPage::UpdateEpisodeList(int page_id)
{
  if (page_id == kEpsiodeSelectorPageId)
  {
    search_edit_->clear();
    DestroyEpisodeList();
    constexpr int kEpisodEListIndex = 1;
    vbox_->insertWidget(kEpisodEListIndex, CreateEpisodesList());

    const auto& current_series_data = series_data_calculator_.GetSeriesData(current_id_);

    for (const auto& episode_info : current_series_data)
    {
      episodes_list_items_->append(tr(episode_info.episode_description.c_str()));
    }

    episodes_list_model_->setStringList(*(episodes_list_items_));
  }
}

void EpisodeSelectorWizardPage::SetCurrentId(int current_id)
{
  current_id_ = current_id;
}

void EpisodeSelectorWizardPage::FilterEpisodeList(const QString& filter)
{
  std::regex search_pattern{".*" + filter.toStdString() + ".*", std::regex_constants::icase};
  const auto& current_series_data = series_data_calculator_.GetSeriesData(current_id_);
  std::vector<SeriesDataCalculator::SeriesData::const_iterator> iterators_to_filtered_elements;

  for (auto it = current_series_data.begin(); it != current_series_data.end(); ++it)
  {
    if (std::regex_match(it->episode_description, search_pattern))
    {
      iterators_to_filtered_elements.emplace_back(it);
    }
  }

  episodes_list_model_->removeRows(0, episodes_list_model_->rowCount());
  episodes_list_items_->clear();

  for (const auto& it : iterators_to_filtered_elements)
  {
    episodes_list_items_->append(tr(it->episode_description.c_str()));
  }

  episodes_list_model_->setStringList(*(episodes_list_items_));
}

QGroupBox * EpisodeSelectorWizardPage::CreateForm()
{
  form_group_box_ = std::make_unique<QGroupBox>();

  search_label_ = std::make_unique<QLabel>(tr("Search: "));
  search_edit_ = std::make_unique<QLineEdit>();

  form_layout_ = std::make_unique<QFormLayout>();
  form_layout_->addRow(search_label_.get(), search_edit_.get());

  form_group_box_->setLayout(form_layout_.get());

  return form_group_box_.get();
}

QListView * EpisodeSelectorWizardPage::CreateEpisodesList()
{

  episodes_list_ = std::make_unique<QListView>();
  episodes_list_->setEditTriggers(QAbstractItemView::NoEditTriggers);
  episodes_list_items_ = std::make_unique<QStringList>();
  episodes_list_model_ = std::make_unique<QStringListModel>(*episodes_list_items_, nullptr);
  episodes_list_->setModel(episodes_list_model_.get());

  return episodes_list_.get();
}

void EpisodeSelectorWizardPage::DestroyEpisodeList()
{
  episodes_list_items_.reset();
  episodes_list_model_.reset();
  episodes_list_.reset();
}
