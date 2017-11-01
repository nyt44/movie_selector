#include "mainwizardpage.h"

#include <QBoxLayout>
#include <QListView>
#include <QStandardItemModel>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

#include <vector>


using std::unique_ptr;
using std::make_unique;

struct MainWizardPage::Pimpl
{
    QGroupBox * createForm()
    {
        form_group_box_ = make_unique<QGroupBox>();

        search_label_ = make_unique<QLabel>(tr("Search: "));
        search_edit_ = make_unique<QLineEdit>();

        form_layout_ = make_unique<QFormLayout>();
        form_layout_->addRow(search_label_.get(), search_edit_.get());

        form_group_box_->setLayout(form_layout_.get());
        return form_group_box_.get();
    }

    QListView * createEpisodesList()
    {
        episodes_list_ = make_unique<QListView>();

        //Example list;
        episodes_list_items_.emplace_back(make_unique<QStandardItem>(tr("Episode1")));
        episodes_list_items_.emplace_back(make_unique<QStandardItem>(tr("Episode2")));

        episodes_list_model_= make_unique<QStandardItemModel>(
                    static_cast<int>(episodes_list_items_.size()), 1);

        for (auto i = 0u; i < episodes_list_items_.size(); ++i)
        {
            episodes_list_model_->setItem(i, 0, episodes_list_items_[i].get());
        }
        episodes_list_->setModel(episodes_list_model_.get());

        return episodes_list_.get();
    }

    unique_ptr<QVBoxLayout> vbox_;
    unique_ptr<QGroupBox> form_group_box_;
    unique_ptr<QLabel> search_label_;
    unique_ptr<QLineEdit> search_edit_;
    unique_ptr<QFormLayout> form_layout_;
    unique_ptr<QListView> episodes_list_;
    unique_ptr<QStandardItemModel > episodes_list_model_;
    std::vector<unique_ptr<QStandardItem>> episodes_list_items_;
};

MainWizardPage::MainWizardPage(QWidget *parent)
    : QWizardPage(parent)
{
    setWindowTitle(tr("Episode selector"));

    pimpl_ = make_unique<Pimpl>();
    pimpl_->vbox_ = make_unique<QVBoxLayout>();
    pimpl_->vbox_->addWidget(pimpl_->createForm());
    pimpl_->vbox_->addWidget(pimpl_->createEpisodesList());

    setLayout(pimpl_->vbox_.get());
}

MainWizardPage::~MainWizardPage()
{
}
