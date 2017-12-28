#pragma once

#include "seriesdatakeeper.h"

#include <QString>
#include <QObject>

#include <memory>

class ThreadMgr : public QObject
{
    Q_OBJECT
public:
    ThreadMgr();
    ~ThreadMgr();
    void start();
    void stop();
    SeriesDataKeeper * getSeriesDataKeeper() const;

private:
    ThreadMgr(const ThreadMgr &) = delete;
    ThreadMgr & operator=(const ThreadMgr &) = delete;
    ThreadMgr & operator=(ThreadMgr &&) = delete;

    struct Pimpl;

    std::unique_ptr<Pimpl> pimpl_;

public slots:
    void newTextGivenSlot(const QString & new_text);
    void seriesTypeChangedSlot(const std::string & search_str);
signals:
    void updateSignal(SeriesDataKeeper * data_keeper);
};

extern ThreadMgr thread_mgr;
