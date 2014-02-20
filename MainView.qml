import QtQuick 2.0
import com.bsquask.synth 1.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import "notevalues.js" as Notes

Item {
    SynthEngine {
        id: synthEngine
        gain: 1.0
        soundfonts: ["/usr/share/sounds/sf2/FluidR3_GM.sf2"]
        //soundfonts: ["/usr/share/sounds/sf2/GS.sf2"]
        bankoffsets: ["0"]

        settings: SynthSettings {
            id: synthSettings
            midiIn: true;
            //midiDriver: "oss"
            midiDriver: "alsa_raw"
            //midiDevice: "/dev/snd/midiC1D0"
            midiChannels: 8
            audioDriver: "alsa"
            //options: ["audio.period-size=128", "audio.sample-rate=44100", "audio.periods=16", "audio.alsa.device=hw:0", "synth.reverb.active=no", "synth.chorus.active=no"]
            options: ["audio.period-size=128", "audio.sample-rate=44100", "audio.periods=8", "synth.reverb.active=no", "synth.chorus.active=no"]
        }

        Component.onCompleted: {
            synthEngine.start();
        }

    }

    NoteViewer {
        id: noteViewer
        anchors.left: parent.left
        height: parent.height
        width: parent.width / 2

    }

    SynthPresetsModel {
        id: instrumentsModel;
        engine: synthEngine
        property int currentIndex: 0
    }

    ChordRecognizer {
        id: chordRecognizer
    }

    Connections {
        target: synthEngine
        onMidiKeyPressed: {
            if (value >= 0 && value < 128) {
                noteViewer.addNote(value);
                chordRecognizer.addNote(Notes.noteTable[value].name);
            }
        }

        onMidiKeyReleased: {
            if (value >= 0 && value < 128) {
                noteViewer.removeNote(value);
                chordRecognizer.removeNote(Notes.noteTable[value].name);
            }
        }
    }

    Rectangle {
        id: infoArea
        color: "#35322f"
        anchors.right: parent.right
        height: parent.height
        width: parent.width / 2

        StackView {
            id: infoMenuStackView
            anchors.margins: 16
            anchors.top: infoArea.top
            anchors.bottom: backButton.top
            anchors.right: infoArea.right
            anchors.left: infoArea.left
            z: 1
            initialItem: Qt.resolvedUrl("InfoMenuItem.qml");
            clip: true;
        }

        Button {
            id: backButton
            text: "Menu"
            onClicked: {
                infoMenuStackView.pop();
            }
            anchors.bottom: infoArea.bottom
            anchors.horizontalCenter: infoArea.horizontalCenter
            height: 64
            visible: infoMenuStackView.depth > 1
            anchors.bottomMargin: 16
            style: ButtonStyle {
                label: Text {
                    text: backButton.text
                    font.pointSize: 32
                }
            }

        }
    }
}
