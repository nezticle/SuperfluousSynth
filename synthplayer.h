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

#ifndef SYNTHPLAYER_H
#define SYNTHPLAYER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <fluidsynth.h>
#include <QtCore/QTimer>

class SynthEngine;

class SynthPlayer : public QObject
{
    Q_OBJECT
    Q_ENUMS(Status)
    Q_PROPERTY(SynthEngine* engine READ engine WRITE setEngine NOTIFY engineChanged)
    Q_PROPERTY(QStringList midifiles READ midifiles WRITE setMidifiles NOTIFY midifilesChanged)
    //Times left to loop the playlist. -1 means loop infinitely.
    Q_PROPERTY(int loop READ loop WRITE setLoop NOTIFY loopChanged)
    //Tempo to set playback speed to (in microseconds per quarter note, as per MIDI file spec
    Q_PROPERTY(int midiTempo READ midiTempo WRITE setMidiTempo NOTIFY midiTempoChanged)
    //Tempo in beats per minute
    Q_PROPERTY(int bpm READ bpm WRITE setBpm NOTIFY bpmChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
public:
    enum Status {
        Ready,
        Playing,
        Done
    };

    explicit SynthPlayer(QObject *parent = 0);
    ~SynthPlayer();

    SynthEngine *engine() const;
    void setEngine(SynthEngine *engine);

    QStringList midifiles() const;
    void setMidifiles(const QStringList &midifiles);

    int loop() const;
    void setLoop(int loop);

    Status status() const;

    int midiTempo() const;
    void setMidiTempo(int midiTempo);

    int bpm() const;
    void setBpm(int bpm);

public slots:
    bool play();
    void stop();

private slots:
    void engineActivityChanged();
    void updateStatus();

signals:
    void engineChanged();
    void midifilesChanged();
    void loopChanged();
    void midiTempoChanged();
    void bpmChanged();
    void statusChanged();

private:
    int updateMidiFiles();
    void createPlayer();
    void destroyPlayer();

    fluid_player_t *m_fluidPlayer;
    SynthEngine *m_engine;
    QStringList m_midifiles;
    int m_loop;
    int m_midiTempo;
    int m_bpm;
    Status m_status;
    QTimer m_pollStatusTimer;
};

#endif // SYNTHPLAYER_H
