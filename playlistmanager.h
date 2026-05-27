#pragma once
#include <QObject>
#include <QStringList>
#include <QDir>
#include "DatabaseManager.h"

class PlaylistManager : public QObject {
    Q_OBJECT
public:
    explicit PlaylistManager(DatabaseManager *db, QObject *parent = nullptr);
    void scanFolder(const QString &folderPath);
    void addFile(const QString &filePath);
    void removeTrack(int id);
    void clearAll();
    QList<TrackInfo> tracks() const;
signals:
    void playlistChanged();
private:
    DatabaseManager *m_db;
    QString extractTitle(const QString &path);
};