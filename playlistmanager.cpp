#include "playlistmanager.h"
#include <QFileInfo>
#include <QDebug>

PlaylistManager::PlaylistManager(DatabaseManager *db, QObject *parent)
    : QObject(parent), m_db(db) {}

void PlaylistManager::scanFolder(const QString &folderPath) {
    try {
        QDir dir(folderPath);
        if (!dir.exists())
            throw std::runtime_error("Folder does not exist: " +
                                     folderPath.toStdString());
        QStringList filters = {"*.mp3","*.wav","*.ogg","*.flac","*.m4a"};
        dir.setNameFilters(filters);
        const auto files = dir.entryInfoList(QDir::Files);
        for (const QFileInfo &fi : files) {
            m_db->addTrack(fi.absoluteFilePath(), extractTitle(fi.fileName()));
        }
        qDebug() << "[Playlist] Scanned" << files.size() << "files from" << folderPath;
        emit playlistChanged();
    } catch (const std::exception &e) {
        qCritical() << "[Playlist] scanFolder error:" << e.what();
    }
}

void PlaylistManager::addFile(const QString &filePath) {
    try {
        QFileInfo fi(filePath);
        if (!fi.exists())
            throw std::runtime_error("File not found: " + filePath.toStdString());
        m_db->addTrack(fi.absoluteFilePath(), extractTitle(fi.fileName()));
        emit playlistChanged();
    } catch (const std::exception &e) {
        qCritical() << "[Playlist] addFile error:" << e.what();
    }
}

void PlaylistManager::removeTrack(int id) {
    m_db->removeTrack(id);
    emit playlistChanged();
}

void PlaylistManager::clearAll() {
    m_db->clearPlaylist();
    emit playlistChanged();
}

QList<TrackInfo> PlaylistManager::tracks() const {
    return m_db->getAllTracks();
}

QString PlaylistManager::extractTitle(const QString &fileName) {
    return QFileInfo(fileName).completeBaseName();
}