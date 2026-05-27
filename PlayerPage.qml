import QtQuick
import QtQuick.Controls

Item {
    id: playerPage

    property bool isWide: width > 600

    Rectangle {
        anchors.fill: parent
        color: "#1a1a2e"
    }

    // ── Широкий экран (Desktop/Linux) ──────────────────────────────────────
    Row {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 24
        visible: isWide

        Column {
            width: parent.width * 0.4
            anchors.verticalCenter: parent.verticalCenter
            spacing: 16

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 220
                height: 220
                radius: 16
                color: "#2a2a4a"
                border.color: "#e94560"
                border.width: 3
                clip: true

                Image {
                    anchors.fill: parent
                    source: vm.coverPath.length > 0 ? "file:///" + vm.coverPath : ""
                    fillMode: Image.PreserveAspectCrop
                    visible: vm.coverPath.length > 0
                }

                Text {
                    anchors.centerIn: parent
                    text: "♪"
                    font.pixelSize: 90
                    color: "#e94560"
                    visible: vm.coverPath.length === 0

                    SequentialAnimation on scale {
                        running: vm.playing
                        loops: Animation.Infinite
                        NumberAnimation { to: 1.1; duration: 600; easing.type: Easing.InOutQuad }
                        NumberAnimation { to: 1.0; duration: 600; easing.type: Easing.InOutQuad }
                    }
                }
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: vm.currentTitle.length > 0
                      ? vm.currentTitle
                      : ["Нет трека", "Няма трэка", "No track"][vm.language]
                font.pixelSize: 16
                font.bold: true
                color: "#ffffff"
                elide: Text.ElideRight
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Column {
            width: parent.width * 0.55
            anchors.verticalCenter: parent.verticalCenter
            spacing: 20

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 4
                Text { text: vm.formatTime(vm.position); color: "#aaaacc"; font.pixelSize: 14 }
                Text { text: " / "; color: "#aaaacc"; font.pixelSize: 14 }
                Text { text: vm.formatTime(vm.duration); color: "#aaaacc"; font.pixelSize: 14 }
            }

            Slider {
                width: parent.width
                from: 0
                to: vm.duration > 0 ? vm.duration : 1
                value: vm.position
                onPressedChanged: { if (!pressed) vm.seek(value) }
                background: Rectangle {
                    x: parent.leftPadding
                    y: parent.topPadding + parent.availableHeight / 2 - height / 2
                    width: parent.availableWidth
                    height: 4; radius: 2; color: "#2a2a4a"
                    Rectangle {
                        width: parent.width * parent.parent.visualPosition
                        height: parent.height; radius: 2; color: "#e94560"
                    }
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 16
                Repeater {
                    model: [
                        { text: "⏮", action: "prev"   },
                        { text: "⏯", action: "toggle" },
                        { text: "⏭", action: "next"   },
                        { text: "⏹", action: "stop"   }
                    ]
                    delegate: Rectangle {
                        id: btn1
                        width: 56; height: 56; radius: 28
                        color: "#2a2a4a"; border.color: "#e94560"; border.width: 2
                        Text { anchors.centerIn: parent; text: modelData.text; font.pixelSize: 22; color: "#e94560" }
                        MouseArea {
                            anchors.fill: parent
                            onPressed: btn1.scale = 0.9
                            onReleased: btn1.scale = 1.0
                            onClicked: {
                                if (modelData.action === "prev")   vm.previous()
                                if (modelData.action === "toggle") vm.togglePlayPause()
                                if (modelData.action === "next")   vm.next()
                                if (modelData.action === "stop")   vm.stop()
                            }
                        }
                        Behavior on scale { NumberAnimation { duration: 100 } }
                    }
                }
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 8
                Text { text: "🔈"; font.pixelSize: 18; color: "#aaaacc"; anchors.verticalCenter: parent.verticalCenter }
                Slider {
                    width: 160; from: 0.0; to: 1.0; value: vm.volume
                    onValueChanged: vm.setVolume(value)
                }
                Text { text: "🔊"; font.pixelSize: 18; color: "#aaaacc"; anchors.verticalCenter: parent.verticalCenter }
            }

            Text {
                text: vm.errorMessage
                color: "#e94560"; font.pixelSize: 12
                visible: vm.errorMessage.length > 0
                wrapMode: Text.WordWrap; width: parent.width
            }
        }
    }

    // ── Узкий экран (Android/iOS/Mobile) ───────────────────────────────────
    Column {
        anchors {
            top: parent.top; left: parent.left
            right: parent.right; bottom: parent.bottom
            margins: 20
        }
        spacing: 16
        visible: !isWide

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: ["Плеер", "Плэер", "Player"][vm.language]
            font.pixelSize: 22; font.bold: true; color: "#e94560"
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 160; height: 160; radius: 16
            color: "#2a2a4a"; border.color: "#e94560"; border.width: 3; clip: true

            Image {
                anchors.fill: parent
                source: vm.coverPath.length > 0 ? "file:///" + vm.coverPath : ""
                fillMode: Image.PreserveAspectCrop
                visible: vm.coverPath.length > 0
            }

            Text {
                anchors.centerIn: parent; text: "♪"
                font.pixelSize: 70; color: "#e94560"
                visible: vm.coverPath.length === 0

                SequentialAnimation on scale {
                    running: vm.playing; loops: Animation.Infinite
                    NumberAnimation { to: 1.1; duration: 600; easing.type: Easing.InOutQuad }
                    NumberAnimation { to: 1.0; duration: 600; easing.type: Easing.InOutQuad }
                }
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: vm.currentTitle.length > 0
                  ? vm.currentTitle
                  : ["Нет трека", "Няма трэка", "No track"][vm.language]
            font.pixelSize: 16; font.bold: true; color: "#ffffff"
            elide: Text.ElideRight; width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 4
            Text { text: vm.formatTime(vm.position); color: "#aaaacc"; font.pixelSize: 13 }
            Text { text: " / "; color: "#aaaacc"; font.pixelSize: 13 }
            Text { text: vm.formatTime(vm.duration); color: "#aaaacc"; font.pixelSize: 13 }
        }

        Slider {
            width: parent.width
            from: 0; to: vm.duration > 0 ? vm.duration : 1; value: vm.position
            onPressedChanged: { if (!pressed) vm.seek(value) }
            background: Rectangle {
                x: parent.leftPadding
                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                width: parent.availableWidth; height: 4; radius: 2; color: "#2a2a4a"
                Rectangle {
                    width: parent.width * parent.parent.visualPosition
                    height: parent.height; radius: 2; color: "#e94560"
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 12
            Repeater {
                model: [
                    { text: "⏮", action: "prev"   },
                    { text: "⏯", action: "toggle" },
                    { text: "⏭", action: "next"   },
                    { text: "⏹", action: "stop"   }
                ]
                delegate: Rectangle {
                    id: btn2
                    width: 56; height: 56; radius: 28
                    color: "#2a2a4a"; border.color: "#e94560"; border.width: 2
                    Text { anchors.centerIn: parent; text: modelData.text; font.pixelSize: 22; color: "#e94560" }
                    MouseArea {
                        anchors.fill: parent
                        onPressed: btn2.scale = 0.9
                        onReleased: btn2.scale = 1.0
                        onClicked: {
                            if (modelData.action === "prev")   vm.previous()
                            if (modelData.action === "toggle") vm.togglePlayPause()
                            if (modelData.action === "next")   vm.next()
                            if (modelData.action === "stop")   vm.stop()
                        }
                    }
                    Behavior on scale { NumberAnimation { duration: 100 } }
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 8
            Text { text: "🔈"; font.pixelSize: 16; color: "#aaaacc"; anchors.verticalCenter: parent.verticalCenter }
            Slider {
                width: 130; from: 0.0; to: 1.0; value: vm.volume
                onValueChanged: vm.setVolume(value)
            }
            Text { text: "🔊"; font.pixelSize: 16; color: "#aaaacc"; anchors.verticalCenter: parent.verticalCenter }
        }

        Text {
            text: vm.errorMessage; color: "#e94560"; font.pixelSize: 12
            visible: vm.errorMessage.length > 0
            wrapMode: Text.WordWrap; width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }
    }
}