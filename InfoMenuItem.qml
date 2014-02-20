import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.0

Item {
    Text {
        id: logo
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 16
        height: parent.height / 8
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "SuperfluousSynth"
        minimumPointSize: 16
        font.pointSize: 100
        color: "#14aaff"
    }

    ColumnLayout {
        anchors.top: logo.bottom
        anchors.bottom: parent.bottom
        //width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        Button {
            id: selectInstrumentButton
            text: "Select Instrument"
            onClicked: {
                infoMenuStackView.push(Qt.resolvedUrl("InstrumentSelectionPage.qml"))
            }
            height: 64
            style: ButtonStyle {
                label: Text {
                    text: selectInstrumentButton.text
                    font.pointSize: 32
                }
            }
        }
        Button {
            id: chordRecognizerButton
            text: "Chord Recognizer"
            onClicked: {
                infoMenuStackView.push(Qt.resolvedUrl("ChordViewerPage.qml"))
            }
            height: 64
            style: ButtonStyle {
                label: Text {
                    text: chordRecognizerButton.text
                    font.pointSize: 32
                }
            }
        }
    }
}
