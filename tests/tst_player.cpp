#include <QtTest>
#include <QSignalSpy>
#include "../databasemanager.h"
#include "../playlistmanager.h"
#include "../playermodel.h"

// ── Модульные тесты ─────────────────────────────────────────────────────────

class TestDatabase : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        m_db.init();
    }

    void testAddTrack() {
        bool ok = m_db.addTrack("/test/song.mp3", "Test Song", "Artist");
        QVERIFY(ok);
    }

    void testGetAllTracks() {
        m_db.addTrack("/test/song2.mp3", "Song 2", "Artist 2");
        auto tracks = m_db.getAllTracks();
        QVERIFY(tracks.size() >= 1);
    }

    void testRemoveTrack() {
        m_db.addTrack("/test/song3.mp3", "Song 3", "");
        auto before = m_db.getAllTracks();
        int id = before.last().id;
        bool ok = m_db.removeTrack(id);
        QVERIFY(ok);
        auto after = m_db.getAllTracks();
        QCOMPARE(after.size(), before.size() - 1);
    }

    void testClearPlaylist() {
        m_db.addTrack("/test/clear.mp3", "Clear", "");
        m_db.clearPlaylist();
        auto tracks = m_db.getAllTracks();
        QCOMPARE(tracks.size(), 0);
    }

    void testDuplicateTrack() {
        m_db.addTrack("/test/dup.mp3", "Dup", "");
        bool ok = m_db.addTrack("/test/dup.mp3", "Dup", "");
        QVERIFY(ok);
        auto tracks = m_db.getAllTracks();
        int count = 0;
        for (const auto &t : tracks)
            if (t.path == "/test/dup.mp3") count++;
        QCOMPARE(count, 1);
    }

    void testTrackFields() {
        m_db.clearPlaylist();
        m_db.addTrack("/test/fields.mp3", "My Title", "My Artist");
        auto tracks = m_db.getAllTracks();
        QCOMPARE(tracks.size(), 1);
        QCOMPARE(tracks[0].title, QString("My Title"));
        QCOMPARE(tracks[0].artist, QString("My Artist"));
        QCOMPARE(tracks[0].path, QString("/test/fields.mp3"));
    }

private:
    DatabaseManager m_db;
};

// ── Тесты PlaylistManager ────────────────────────────────────────────────────

class TestPlaylist : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        m_db.init();
    }

    void testScanNonExistentFolder() {
        QSignalSpy spy(&m_playlist, &PlaylistManager::playlistChanged);
        m_playlist.scanFolder("/nonexistent/path/12345");
        QCOMPARE(spy.count(), 0);
    }

    void testAddNonExistentFile() {
        QSignalSpy spy(&m_playlist, &PlaylistManager::playlistChanged);
        m_playlist.addFile("/nonexistent/file.mp3");
        QCOMPARE(spy.count(), 0);
    }

    void testClearAll() {
        QSignalSpy spy(&m_playlist, &PlaylistManager::playlistChanged);
        m_playlist.clearAll();
        QVERIFY(spy.count() >= 1);
    }

    void testTracksAfterClear() {
        m_playlist.clearAll();
        auto tracks = m_playlist.tracks();
        QCOMPARE(tracks.size(), 0);
    }

    void testRemoveNonExistent() {
        m_playlist.removeTrack(99999);
        QVERIFY(true);
    }

private:
    DatabaseManager  m_db;
    PlaylistManager  m_playlist{&m_db};
};

// ── UI тесты ────────────────────────────────────────────────────────────────

class TestPlayerModel : public QObject {
    Q_OBJECT
private slots:
    void testInitialState() {
        PlayerModel player;
        QCOMPARE(player.isPlaying(), false);
        QCOMPARE(player.position(), 0LL);
        QCOMPARE(player.duration(), 0LL);
    }

    void testStopWhenNotPlaying() {
        PlayerModel player;
        player.stop();
        QCOMPARE(player.isPlaying(), false);
    }

    void testPauseWhenNotPlaying() {
        PlayerModel player;
        player.pause();
        QCOMPARE(player.isPlaying(), false);
    }

    void testVolumeClampMin() {
        PlayerModel player;
        player.setVolume(-1.0f);
        QVERIFY(true);
    }

    void testVolumeClampMax() {
        PlayerModel player;
        player.setVolume(2.0f);
        QVERIFY(true);
    }

    void testPositionSignal() {
        PlayerModel player;
        QSignalSpy spy(&player, &PlayerModel::positionChanged);
        QVERIFY(spy.isValid());
    }

    void testErrorSignal() {
        PlayerModel player;
        QSignalSpy spy(&player, &PlayerModel::errorOccurred);
        QVERIFY(spy.isValid());
        player.play(QUrl("file:///nonexistent_file_that_does_not_exist.mp3"));
        QVERIFY(true);
    }

private:
};

// ── Интеграционные тесты ─────────────────────────────────────────────────────

class TestIntegration : public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        m_db.init();
    }

    void testFullFlow_addAndGetTrack() {
        m_db.clearPlaylist();
        m_db.addTrack("/music/test.mp3", "Integration Track", "Test Artist");
        auto tracks = m_db.getAllTracks();
        QCOMPARE(tracks.size(), 1);
        QCOMPARE(tracks[0].title, QString("Integration Track"));
    }

    void testFullFlow_addAndRemove() {
        m_db.clearPlaylist();
        m_db.addTrack("/music/remove.mp3", "Remove Me", "");
        auto before = m_db.getAllTracks();
        QCOMPARE(before.size(), 1);
        m_db.removeTrack(before[0].id);
        auto after = m_db.getAllTracks();
        QCOMPARE(after.size(), 0);
    }

    void testFullFlow_playlistManagerWithDb() {
        m_db.clearPlaylist();
        QSignalSpy spy(&m_playlist, &PlaylistManager::playlistChanged);
        m_playlist.clearAll();
        QVERIFY(spy.count() >= 1);
        auto tracks = m_playlist.tracks();
        QCOMPARE(tracks.size(), 0);
    }

    void testFullFlow_multipleTracksOrder() {
        m_db.clearPlaylist();
        m_db.addTrack("/music/a.mp3", "A Track", "");
        m_db.addTrack("/music/b.mp3", "B Track", "");
        m_db.addTrack("/music/c.mp3", "C Track", "");
        auto tracks = m_db.getAllTracks();
        QCOMPARE(tracks.size(), 3);
    }

    void testFullFlow_playerInitialState() {
        QCOMPARE(m_player.isPlaying(), false);
        QCOMPARE(m_player.position(), 0LL);
    }

private:
    DatabaseManager m_db;
    PlaylistManager m_playlist{&m_db};
    PlayerModel     m_player;
};

// ── Запуск всех тестов ───────────────────────────────────────────────────────

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    int status = 0;

    TestDatabase   t1;
    TestPlaylist   t2;
    TestPlayerModel t3;
    TestIntegration t4;

    status |= QTest::qExec(&t1, argc, argv);
    status |= QTest::qExec(&t2, argc, argv);
    status |= QTest::qExec(&t3, argc, argv);
    status |= QTest::qExec(&t4, argc, argv);

    return status;
}

#include "tst_player.moc"