import QtQuick 2.0
import com.bsquask.synth 1.0
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
            midiDriver: "oss"
            midiDevice: "/dev/snd/midiC1D0"
            midiChannels: 16
            audioDriver: "alsa"
            options: ["audio.period-size=128", "audio.sample-rate=44100", "audio.periods=16", "audio.alsa.device=hw:0", "synth.reverb.active=no", "synth.chorus.active=no"]
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

    ChordRecognizer {
        id: chordRecognizer
    }

    Rectangle {
        id: infoArea
        color: "#35322f"
        anchors.right: parent.right
        height: parent.height
        width: parent.width / 2

        SynthPresetsModel {
            id: instrumentsModel;
            engine: synthEngine
        }

        Component {
            id: instrumentDelegate
            Rectangle {
                width: infoArea.width - 16
                height: 64
                color: ListView.isCurrentItem ? "#14aaff" : "white"
                border.color: "black"
                Text {
                    anchors.fill: parent
                    text: preset.name;
                    fontSizeMode: Text.Fit
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    minimumPointSize: 12
                    font.pointSize: 32
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        instrumentSelectionView.currentIndex = index;
                        synthEngine.setPreset(0, preset);
                    }
                }
            }
        }

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

        ListView {
            id: instrumentSelectionView
            width: parent.width - 16
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: logo.bottom
            anchors.bottom: chordLabel.top

            model: instrumentsModel
            delegate: instrumentDelegate
            clip: true;
            currentIndex: 0
        }


        Text {
            id: chordLabel
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            width: parent.width - 16
            height: parent.height / 4
            fontSizeMode: Text.Fit
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: chordRecognizer.chordName;
            minimumPointSize: 16
            font.pointSize: 100

            color: "#14aaff"
        }
    }
}
