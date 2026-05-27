import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    Rectangle {
        anchors.fill: parent
        color: "#1a1a2e"
    }

    Column {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 12
        }
        spacing: 12

        Text {
            text: ["Плейлист", "Плэйліст", "Playlist"][vm.language]
            font.pixelSize: 22
            font.bold: true
            color: "#e94560"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 8

            Button {
                text: ["Добавить папку", "Дадаць папку", "Add Folder"][vm.language]
                onClicked: folderDialog.open()
                background: Rectangle {
                    radius: 8
                    color: "#e94560"
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                text: ["Очистить", "Ачысціць", "Clear"][vm.language]
                onClicked: vm.clearPlaylist()
                background: Rectangle {
                    radius: 8
                    color: "#2a2a4a"
                    border.color: "#e94560"
                    border.width: 1
                }
                contentItem: Text {
                    text: parent.text
                    color: "#e94560"
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: ["Треков: ", "Трэкаў: ", "Tracks: "][vm.language] + vm.tracks.length
            color: "#8888aa"
            font.pixelSize: 13
        }

        ListView {
            width: parent.width
            height: parent.height - 150
            clip: true
            model: vm.tracks

            delegate: Rectangle {
                width: ListView.view.width
                height: 56
                color: vm.currentIndex === index ? "#2a2a4a" : "transparent"
                radius: 8

                Row {
                    anchors {
                        left: parent.left
                        right: removeBtn.left
                        verticalCenter: parent.verticalCenter
                        leftMargin: 12
                    }
                    spacing: 8

                    Text {
                        text: (index + 1) + "."
                        color: "#8888aa"
                        font.pixelSize: 13
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        Text {
                            text: modelData.title
                            color: "#ffffff"
                            font.pixelSize: 14
                            font.bold: vm.currentIndex === index
                            elide: Text.ElideRight
                            width: 200
                        }
                        Text {
                            text: modelData.artist.length > 0 ? modelData.artist : "—"
                            color: "#8888aa"
                            font.pixelSize: 12
                        }
                    }
                }

                Text {
                    id: removeBtn
                    anchors {
                        right: parent.right
                        rightMargin: 12
                        verticalCenter: parent.verticalCenter
                    }
                    text: "✕"
                    color: "#e94560"
                    font.pixelSize: 16

                    MouseArea {
                        anchors.fill: parent
                        onClicked: vm.removeTrack(modelData.id)
                    }
                }

                MouseArea {
                    anchors {
                        left: parent.left
                        right: removeBtn.left
                        top: parent.top
                        bottom: parent.bottom
                    }
                    onClicked: vm.playTrack(index)
                }

                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: "#2a2a4a"
                }
            }
        }
    }

    FolderDialog {
        id: folderDialog
        title: ["Выберите папку", "Выберыце папку", "Select Folder"][vm.language]
        onAccepted: vm.scanFolder(selectedFolder.toString().replace("file:///", ""))
    }
}