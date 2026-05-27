#pragma once
#include <QObject>
#include <QStringList>
#include <QVariantList>
#include "PlayerModel.h"
#include "PlaylistManager.h"
#include "DatabaseManager.h"

class PlayerViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool playing READ playing NOTIFY playingChanged)
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString currentTitle READ currentTitle NOTIFY currentTrackChanged)
    Q_PROPERTY(QString coverPath READ coverPath NOTIFY currentTrackChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentTrackChanged)
    Q_PROPERTY(QVariantList tracks READ tracks NOTIFY tracksChanged)
    Q_PROPERTY(int language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorChanged)

public:
    explicit PlayerViewModel(QObject *parent = nullptr);

    bool playing() const;
    qint64 position() const;
    qint64 duration() const;
    QString currentTitle() const;
    QString coverPath() const;
    int currentIndex() const;
    QVariantList tracks() const;
    int language() const;
    float volume() const;
    QString errorMessage() const;

    void setLanguage(int lang);
    Q_INVOKABLE void setVolume(float vol);

    Q_INVOKABLE void playTrack(int index);
    Q_INVOKABLE void togglePlayPause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void next();
    Q_INVOKABLE void previous();
    Q_INVOKABLE void seek(qint64 position);
    Q_INVOKABLE void scanFolder(const QString &path);
    Q_INVOKABLE void addFile(const QString &path);
    Q_INVOKABLE void removeTrack(int id);
    Q_INVOKABLE void clearPlaylist();
    Q_INVOKABLE QString formatTime(qint64 ms) const;

signals:
    void playingChanged();
    void positionChanged();
    void durationChanged();
    void currentTrackChanged();
    void tracksChanged();
    void languageChanged();
    void volumeChanged();
    void errorChanged();

private:
    PlayerModel      m_player;
    DatabaseManager  m_db;
    PlaylistManager  m_playlist;
    int              m_currentIndex;
    int              m_language;
    float            m_volume;
    QString          m_errorMessage;
    QList<TrackInfo> m_tracks;

    void refreshTracks();
    void playCurrentIndex();
};