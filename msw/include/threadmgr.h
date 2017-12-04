#pragma once

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

private:
    ThreadMgr(const ThreadMgr &) = delete;
    ThreadMgr & operator=(const ThreadMgr &) = delete;
    ThreadMgr & operator=(ThreadMgr &&) = delete;

    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;

public slots:
    void newTextGivenSlot(const QString & new_text);
};

extern ThreadMgr thread_mgr;
