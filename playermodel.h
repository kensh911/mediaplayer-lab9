#pragma once
#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

class PlayerModel : public QObject {
    Q_OBJECT
public:
    explicit PlayerModel(QObject *parent = nullptr);
    void play(const QUrl &url);
    void pause();
    void stop();
    void seek(qint64 position);
    void setVolume(float volume);
    bool isPlaying() const;
    qint64 position() const;
    qint64 duration() const;
signals:
    void positionChanged(qint64 pos);
    void durationChanged(qint64 dur);
    void playbackStateChanged(bool playing);
    void errorOccurred(const QString &error);
private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audio;
};