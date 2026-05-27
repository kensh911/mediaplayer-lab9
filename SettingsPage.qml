import QtQuick
import QtQuick.Controls

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
            margins: 20
        }
        spacing: 20

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: ["Настройки", "Налады", "Settings"][vm.language]
            font.pixelSize: 22
            font.bold: true
            color: "#e94560"
        }

        Text {
            text: ["Язык", "Мова", "Language"][vm.language]
            color: "#aaaacc"
            font.pixelSize: 15
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10

            Repeater {
                model: ["Русский", "Беларуская", "English"]
                delegate: Button {
                    text: modelData
                    width: 100
                    height: 40
                    background: Rectangle {
                        radius: 8
                        color: vm.language === index ? "#e94560" : "#2a2a4a"
                        border.color: "#e94560"
                        border.width: 1
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "#ffffff"
                        font.pixelSize: 13
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: vm.language = index
                }
            }
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#2a2a4a"
        }

        Text {
            text: ["О приложении", "Аб праграме", "About"][vm.language]
            color: "#aaaacc"
            font.pixelSize: 15
        }

        Text {
            text: "Media Player v1.0\nQt 6.11 + QML\nBSU FPMI 2026"
            color: "#8888aa"
            font.pixelSize: 13
            lineHeight: 1.5
        }
    }
}