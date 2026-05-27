#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QList>

struct TrackInfo {
    int id;
    QString title;
    QString path;
    QString artist;
    int duration;
};

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();
    bool init();
    bool addTrack(const QString &path, const QString &title,
                  const QString &artist = "");
    bool removeTrack(int id);
    QList<TrackInfo> getAllTracks();
    void clearPlaylist();
private:
    QSqlDatabase m_db;
    bool createTables();
};