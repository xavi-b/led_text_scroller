import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import android

Window {
    width: 640
    height: 480
    visible: true

    Connections {
        target: BluetoothManager
        function onLog(str: string) {
            textEdit.append(str)
        }
    }

    ColumnLayout {
        anchors.fill: parent

        Button {
            text: "Discover"
            Layout.fillWidth: true
            onClicked: {
                BluetoothManager.discoverDevices()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "lightgrey"

            Flickable {
                id: flick

                anchors.fill: parent
                contentWidth: textEdit.contentWidth
                contentHeight: textEdit.contentHeight
                clip: true
                flickableDirection: Flickable.VerticalFlick

                TextEdit {
                    id: textEdit
                    width: flick.width
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: false

            TextField {
                id: textField
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Button {
                text: "Send"
                onClicked: {
                    BluetoothManager.sendTextMessage(textField.text)
                    textField.clear()
                }
            }
        }
    }
}
