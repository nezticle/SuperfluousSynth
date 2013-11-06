/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/

import QtQuick 2.1
import com.bsquask.synth 1.0
import "VexFlow.js" as VexFlow
import "notevalues.js" as Notes

Rectangle {
    id: noteViewer
    width: 280
    height: 480
    color: "white"

    property var keyValues: []

    function addNote(noteValue) {
        //Must keep keyValues sorted so insert
        if (keyValues.length === 0)
            keyValues.push(noteValue);
        else
            for(var i = 0; i < keyValues.length; i++) {
                if (keyValues[i] > noteValue) {
                    //insert before i
                    keyValues.splice(i, 0, noteValue);
                    break;
                } else if (keyValues.length == (i + 1)) {
                    //last item
                    keyValues.push(noteValue)
                    break;
                }
            }

        musicCanvas.requestPaint();
    }

    function removeNote(noteValue) {

        for (var i = 0; i < keyValues.length; ++i) {
            if (keyValues[i] == noteValue) {
                keyValues.splice(i, 1);
                break;
            }
        }

        musicCanvas.requestPaint();
    }

    Canvas {
        id: musicCanvas
        anchors.centerIn: parent

        function updateGeometry() {
            var viewerRatio = parent.width / parent.height;
            var canvasRatio = 140 / 240;

            if (viewerRatio > canvasRatio) {
                width = 140 * parent.height / 240;
                height = parent.height;
                scale = parent.height / 240;
            } else {
                width = parent.width
                height = 240 * parent.width / 140;
                scale = parent.width / 140;
            }
        }

        Connections {
            target: noteViewer
            onHeightChanged: {
                musicCanvas.updateGeometry();
            }
            onWidthChanged: {
                musicCanvas.updateGeometry();
            }

        }

        height: 240
        width: 140
        //logical size 140 x 240

        property var renderer
        property var context
        property var trebleStave
        property var bassStave
        property real scale: 1

        onPaint: {
            context.save(); //clear only
            context.clearRect(0, 0, width, height);
            context.restore();
            context.save();
            context.scale(scale, scale);
            trebleStave.setContext(context)
            trebleStave.draw();
            bassStave.setContext(context);
            bassStave.draw();
            if (noteViewer.keyValues.length) {
                var noteKeys = toNoteArray(noteViewer.keyValues);
                var dynamicNote = new VexFlow.Vex.Flow.StaveNote({ keys: noteKeys, duration: "q" });
                for(var i = 0; i < noteKeys.length; ++i) {
                    if (noteKeys[i][1] === "#") {
                        dynamicNote.addAccidental(i, new VexFlow.Vex.Flow.Accidental("#"));
                    }
                }

                VexFlow.Vex.Flow.Formatter.FormatAndDraw(context, trebleStave, [dynamicNote]);
            }
            context.restore(); //scale
        }

        onAvailableChanged: {
            if (available) {
                updateGeometry();
                musicCanvas.renderer = new VexFlow.Vex.Flow.Renderer(musicCanvas, VexFlow.Vex.Flow.Renderer.Backends.CANVAS);
                musicCanvas.context = renderer.getContext();
                trebleStave = new VexFlow.Vex.Flow.Stave(10, 30, 120);
                trebleStave.addClef("treble")
                bassStave = new VexFlow.Vex.Flow.Stave(10, (trebleStave.options.num_lines + 1) * trebleStave.options.spacing_between_lines_px + 30, 120);
                bassStave.addClef("bass");
            }
        }

        function toNoteArray(valueArray) {
            var noteKeys = [];
            for(var i = 0; i < valueArray.length; ++i) {
                var key = Notes.noteTable[valueArray[i]];
                var keyString = key.name + "/" + key.octave;
                noteKeys.push(keyString)
            }

            return noteKeys;
        }
    }
}
