#include "playermodel.h"
#include <QDebug>

PlayerModel::PlayerModel(QObject *parent) : QObject(parent) {
    m_player = new QMediaPlayer(this);
    m_audio  = new QAudioOutput(this);
    m_player->setAudioOutput(m_audio);
    m_audio->setVolume(0.8f);

    connect(m_player, &QMediaPlayer::positionChanged,
            this, &PlayerModel::positionChanged);
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &PlayerModel::durationChanged);
    connect(m_player, &QMediaPlayer::playbackStateChanged,
            this, [this](QMediaPlayer::PlaybackState s) {
                emit playbackStateChanged(s == QMediaPlayer::PlayingState);
            });
    connect(m_player, &QMediaPlayer::errorOccurred,
            this, [this](QMediaPlayer::Error, const QString &str) {
                qCritical() << "[Player] Error:" << str;
                emit errorOccurred(str);
            });
}

void PlayerModel::play(const QUrl &url) {
    try {
        if (m_player->source() != url)
            m_player->setSource(url);
        m_player->play();
        qDebug() << "[Player] Playing:" << url;
    } catch (const std::exception &e) {
        qCritical() << "[Player] play error:" << e.what();
        emit errorOccurred(QString::fromStdString(e.what()));
    }
}

void PlayerModel::pause()  { m_player->pause(); }
void PlayerModel::stop()   { m_player->stop();  }

void PlayerModel::seek(qint64 position) {
    m_player->setPosition(position);
}

void PlayerModel::setVolume(float volume) {
    m_audio->setVolume(qBound(0.0f, volume, 1.0f));
}

bool PlayerModel::isPlaying() const {
    return m_player->playbackState() == QMediaPlayer::PlayingState;
}

qint64 PlayerModel::position() const { return m_player->position(); }
qint64 PlayerModel::duration() const { return m_player->duration(); }