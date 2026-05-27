import QtQuick
import QtQuick.Controls

Window {
    id: root
    width: 400
    height: 700
    visible: true
    title: ["Медиапроигрыватель", "Медыяпрайгравальнік", "Media Player"][vm.language]

    Rectangle {
        anchors.fill: parent
        color: "#1a1a2e"
    }

    StackView {
        id: stack
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: navBar.top
        }
        initialItem: playerPage
    }

    Component { id: playerPage;   PlayerPage   {} }
    Component { id: playlistPage; PlaylistPage {} }
    Component { id: settingsPage; SettingsPage {} }

    Rectangle {
        id: navBar
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 60
        color: "#16213e"

        Row {
            anchors.centerIn: parent
            spacing: 0

            Repeater {
                model: [
                    { icon: "▶",  label: ["Плеер",    "Плэер",    "Player"][vm.language]   },
                    { icon: "☰",  label: ["Плейлист", "Плэйліст", "Playlist"][vm.language] },
                    { icon: "⚙",  label: ["Настройки","Налады",   "Settings"][vm.language] }
                ]

                delegate: Rectangle {
                    width: root.width / 3
                    height: 60
                    color: "transparent"

                    Column {
                        anchors.centerIn: parent
                        spacing: 2

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: modelData.icon
                            font.pixelSize: 20
                            color: "#e94560"
                        }
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: modelData.label
                            font.pixelSize: 11
                            color: "#aaaacc"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (index === 0) stack.replace(playerPage)
                            else if (index === 1) stack.replace(playlistPage)
                            else stack.replace(settingsPage)
                        }
                    }
                }
            }
        }
    }
}