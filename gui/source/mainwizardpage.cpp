#include "mainwizardpage.h"

#include "singleton.h"
#include "threadmgr.h"

#include <QBoxLayout>
#include <QListView>
#include <QStringListModel>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>

using std::unique_ptr;
using std::make_unique;

struct MainWizardPage::Pimpl
{
    Pimpl();
    QGroupBox * createForm();
    QListView * createEpisodesList();

    unique_ptr<QVBoxLayout> vbox_;
    unique_ptr<QGroupBox> form_group_box_;
    unique_ptr<QLabel> search_label_;
    unique_ptr<QLineEdit> search_edit_;
    unique_ptr<QFormLayout> form_layout_;
    unique_ptr<QListView> episodes_list_;
    unique_ptr<QStringListModel> episodes_list_model_;
    unique_ptr<QStringList> episodes_list_items_;

    SeriesDataKeeper * series_data_keeper_;
};

//Public functions

MainWizardPage::MainWizardPage(QWidget *parent)
    : QWizardPage(parent)
{
    setWindowTitle(tr("Episode selector"));

    pimpl_ = make_unique<Pimpl>();
    pimpl_->vbox_ = make_unique<QVBoxLayout>();
    pimpl_->vbox_->addWidget(pimpl_->createForm());
    pimpl_->vbox_->addWidget(pimpl_->createEpisodesList());

    setLayout(pimpl_->vbox_.get());

    QObject::connect(pimpl_->search_edit_.get(), SIGNAL(textChanged(const QString &)),
                     &thread_mgr, SLOT(newTextGivenSlot(const QString & )));
}

MainWizardPage::~MainWizardPage()
{
}

void MainWizardPage::updateEpisodeList(SeriesDataKeeper * data_keeper)
{
    pimpl_->series_data_keeper_ = data_keeper;
    pimpl_->episodes_list_model_->removeRows(0, pimpl_->episodes_list_model_->rowCount());
    pimpl_->episodes_list_items_->clear();

    uint16_t id;

    pimpl_->series_data_keeper_->startIdReading();
    while (pimpl_->series_data_keeper_->getId(id))
    {
        std::string desc = pimpl_->series_data_keeper_->getDesc(id);
        if (desc == "")
        {
            break;
        }
        else
        {
            pimpl_->episodes_list_items_->append(tr(desc.c_str()));
        }
    }
    pimpl_->series_data_keeper_->stopIdReading();

    pimpl_->episodes_list_model_->setStringList(*(pimpl_->episodes_list_items_));
}

std::string MainWizardPage::getSelectedEpisode() const
{
    QModelIndex index = pimpl_->episodes_list_->currentIndex();
    QString item_text = index.data(Qt::DisplayRole).toString();

    return item_text.toStdString();
}

std::string MainWizardPage::descToPath(const std::string & desc) const
{
    return pimpl_->series_data_keeper_->getPath(desc);
}


//Private functions

MainWizardPage::Pimpl::Pimpl() : series_data_keeper_(thread_mgr.getSeriesDataKeeper())
{
}

QGroupBox * MainWizardPage::Pimpl::createForm()
{
    form_group_box_ = make_unique<QGroupBox>();

    search_label_ = make_unique<QLabel>(tr("Search: "));
    search_edit_ = make_unique<QLineEdit>();

    form_layout_ = make_unique<QFormLayout>();
    form_layout_->addRow(search_label_.get(), search_edit_.get());

    form_group_box_->setLayout(form_layout_.get());

    return form_group_box_.get();
}

QListView * MainWizardPage::Pimpl::createEpisodesList()
{

    episodes_list_ = make_unique<QListView>();
    episodes_list_items_ = make_unique<QStringList>();
    episodes_list_model_ = make_unique<QStringListModel>(*episodes_list_items_, nullptr);
    episodes_list_->setModel(episodes_list_model_.get());

    return episodes_list_.get();
}
