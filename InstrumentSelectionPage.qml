import QtQuick 2.1
import com.bsquask.synth 1.0

Item {
    Component {
        id: instrumentDelegate
        Rectangle {
            width: instrumentSelectionView.width
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
                    instrumentsModel.currentIndex = index;
                    synthEngine.setPreset(0, preset);
                }
            }
        }
    }

    ListView {
        id: instrumentSelectionView
        anchors.fill: parent
        model: instrumentsModel
        delegate: instrumentDelegate
        clip: true;
        currentIndex: instrumentsModel.currentIndex
    }

}
