#include "databasemanager.h"
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) m_db.close();
}

bool DatabaseManager::init() {
    QString dbPath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    QDir().mkpath(dbPath);
    dbPath += "/playlist.db";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    if (!m_db.open()) {
        qCritical() << "[DB] Cannot open:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "[DB] Opened:" << dbPath;
    return createTables();
}

bool DatabaseManager::createTables() {
    QSqlQuery q(m_db);
    bool ok = q.exec(
        "CREATE TABLE IF NOT EXISTS tracks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "path TEXT NOT NULL UNIQUE,"
        "title TEXT,"
        "artist TEXT,"
        "duration INTEGER DEFAULT 0)"
        );
    if (!ok)
        qCritical() << "[DB] Create table error:" << q.lastError().text();
    return ok;
}

bool DatabaseManager::addTrack(const QString &path, const QString &title,
                               const QString &artist) {
    QSqlQuery q(m_db);
    q.prepare("INSERT OR IGNORE INTO tracks (path,title,artist) VALUES(?,?,?)");
    q.addBindValue(path);
    q.addBindValue(title);
    q.addBindValue(artist);
    if (!q.exec()) {
        qCritical() << "[DB] Insert error:" << q.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::removeTrack(int id) {
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM tracks WHERE id=?");
    q.addBindValue(id);
    return q.exec();
}

QList<TrackInfo> DatabaseManager::getAllTracks() {
    QList<TrackInfo> list;
    QSqlQuery q("SELECT id,title,path,artist,duration FROM tracks", m_db);
    while (q.next()) {
        TrackInfo t;
        t.id       = q.value(0).toInt();
        t.title    = q.value(1).toString();
        t.path     = q.value(2).toString();
        t.artist   = q.value(3).toString();
        t.duration = q.value(4).toInt();
        list.append(t);
    }
    return list;
}

void DatabaseManager::clearPlaylist() {
    QSqlQuery q(m_db);
    q.exec("DELETE FROM tracks");
}