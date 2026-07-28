import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Vclib

ApplicationWindow {
    id: root
    width: 1024
    height: 768
    visible: true
    title: "vclib QML BGFX Readback"
    
    color: "#2b2b2b"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        Text {
            text: "Hello Triangle (BGFX -> QML via Readback)"
            color: "white"
            font.pointSize: 20
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        VclBgfxItem {
            id: bgfxItem
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
