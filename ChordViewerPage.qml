import QtQuick 2.1
import com.bsquask.synth 1.0

Item {
    Text {
        id: chordLabel
        anchors.fill: parent
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: chordRecognizer.chordName;
        minimumPointSize: 16
        font.pointSize: 100

        color: "#14aaff"
    }
}
