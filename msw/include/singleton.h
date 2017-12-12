# pragma once

#include <QObject>

#include <memory>
#include <string>

class Singleton : public QObject
{
    Q_OBJECT
public:
    static Singleton & getOnlyInstance();
    bool logError(const std::string & msg) const;
    void updateSignal();

private:
    Singleton();
    ~Singleton();

    Singleton(const Singleton &) = delete;
    Singleton & operator=(const Singleton &) = delete;
    Singleton & operator=(Singleton &&) = delete;

    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;

signals:
    void cloneWarsInitialized();
};
