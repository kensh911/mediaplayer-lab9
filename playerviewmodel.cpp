#include "PlayerViewModel.h"
#include <QDebug>
#include <QUrl>
#include <QFileInfo>

PlayerViewModel::PlayerViewModel(QObject *parent)
    : QObject(parent)
    , m_playlist(&m_db)
    , m_currentIndex(-1)
    , m_language(0)
    , m_volume(0.8f)
{
    m_db.init();
    refreshTracks();

    connect(&m_player, &PlayerModel::positionChanged,
            this, &PlayerViewModel::positionChanged);
    connect(&m_player, &PlayerModel::durationChanged,
            this, &PlayerViewModel::durationChanged);
    connect(&m_player, &PlayerModel::playbackStateChanged,
            this, &PlayerViewModel::playingChanged);
    connect(&m_player, &PlayerModel::errorOccurred,
            this, [this](const QString &err) {
                m_errorMessage = err;
                emit errorChanged();
            });
    connect(&m_playlist, &PlaylistManager::playlistChanged,
            this, [this]() {
                refreshTracks();
                emit tracksChanged();
            });
}

void PlayerViewModel::refreshTracks() {
    m_tracks = m_playlist.tracks();
}

bool    PlayerViewModel::playing()      const { return m_player.isPlaying(); }
qint64  PlayerViewModel::position()     const { return m_player.position(); }
qint64  PlayerViewModel::duration()     const { return m_player.duration(); }
int     PlayerViewModel::currentIndex() const { return m_currentIndex; }
int     PlayerViewModel::language()     const { return m_language; }
float   PlayerViewModel::volume()       const { return m_volume; }
QString PlayerViewModel::errorMessage() const { return m_errorMessage; }

QString PlayerViewModel::currentTitle() const {
    if (m_currentIndex >= 0 && m_currentIndex < m_tracks.size())
        return m_tracks[m_currentIndex].title;
    return "";
}

QString PlayerViewModel::coverPath() const {
    if (m_currentIndex < 0 || m_currentIndex >= m_tracks.size())
        return "";
    QFileInfo fi(m_tracks[m_currentIndex].path);
    QString dir = fi.absolutePath();
    QStringList covers = {"cover.jpg", "cover.png", "folder.jpg",
                          "album.jpg", "front.jpg"};
    for (const QString &name : covers) {
        QString path = dir + "/" + name;
        if (QFileInfo::exists(path))
            return path;
    }
    return "";
}

QVariantList PlayerViewModel::tracks() const {
    QVariantList list;
    for (const TrackInfo &t : m_tracks) {
        QVariantMap map;
        map["id"]     = t.id;
        map["title"]  = t.title;
        map["artist"] = t.artist;
        map["path"]   = t.path;
        list.append(map);
    }
    return list;
}

void PlayerViewModel::setLanguage(int lang) {
    m_language = lang;
    emit languageChanged();
}

void PlayerViewModel::setVolume(float vol) {
    m_volume = qBound(0.0f, vol, 1.0f);
    m_player.setVolume(m_volume);
    emit volumeChanged();
}

void PlayerViewModel::playTrack(int index) {
    if (index < 0 || index >= m_tracks.size()) return;
    m_currentIndex = index;
    emit currentTrackChanged();
    playCurrentIndex();
}

void PlayerViewModel::playCurrentIndex() {
    if (m_currentIndex < 0 || m_currentIndex >= m_tracks.size()) return;
    QUrl url = QUrl::fromLocalFile(m_tracks[m_currentIndex].path);
    m_player.play(url);
}

void PlayerViewModel::togglePlayPause() {
    if (m_player.isPlaying()) m_player.pause();
    else if (m_currentIndex >= 0) playCurrentIndex();
}

void PlayerViewModel::stop() { m_player.stop(); }

void PlayerViewModel::next() {
    if (m_tracks.isEmpty()) return;
    m_currentIndex = (m_currentIndex + 1) % m_tracks.size();
    emit currentTrackChanged();
    playCurrentIndex();
}

void PlayerViewModel::previous() {
    if (m_tracks.isEmpty()) return;
    m_currentIndex = (m_currentIndex - 1 + m_tracks.size()) % m_tracks.size();
    emit currentTrackChanged();
    playCurrentIndex();
}

void PlayerViewModel::seek(qint64 position) { m_player.seek(position); }

void PlayerViewModel::scanFolder(const QString &path) {
    m_playlist.scanFolder(path);
}

void PlayerViewModel::addFile(const QString &path) {
    m_playlist.addFile(path);
}

void PlayerViewModel::removeTrack(int id) {
    m_playlist.removeTrack(id);
    emit tracksChanged();
}

void PlayerViewModel::clearPlaylist() {
    m_playlist.clearAll();
    m_currentIndex = -1;
    emit currentTrackChanged();
    emit tracksChanged();
}

QString PlayerViewModel::formatTime(qint64 ms) const {
    qint64 s = ms / 1000;
    return QString("%1:%2")
        .arg(s / 60, 2, 10, QChar('0'))
        .arg(s % 60, 2, 10, QChar('0'));
}