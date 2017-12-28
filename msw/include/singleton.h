# pragma once

#include <QObject>

#include <memory>
#include <string>
#include <QString>

enum class SeriesChoice
{
    kPenguins = 0u,
    kCloneWars,
    kRebels
};

class Singleton : public QObject
{
    Q_OBJECT
public:
    static Singleton & getOnlyInstance();
    bool logError(const std::string & msg) const;
    void updateSignal();
    SeriesChoice getSeriesChoice() const;

public slots:
    void setPenguinsSlot();
    void setCwSlot();
    void setRebelsSlot();
    void updateSearchStrSlot(const QString & search_str);

signals:
    void seriesTypeChangedSignal(const std::string &);

private:
    Singleton();
    ~Singleton();

    Singleton(const Singleton &) = delete;
    Singleton & operator=(const Singleton &) = delete;
    Singleton & operator=(Singleton &&) = delete;

    struct Pimpl;
    std::unique_ptr<Pimpl> pimpl_;
};
