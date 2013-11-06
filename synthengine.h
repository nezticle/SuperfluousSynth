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

#ifndef SYNTHENGINE_H
#define SYNTHENGINE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "synthsettings.h"

#include <fluidsynth.h>

class SynthAudioOutput;
class SynthPreset;

class SynthEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)
    Q_PROPERTY(float gain READ gain WRITE setGain NOTIFY gainChanged)
    Q_PROPERTY(QStringList soundfonts READ soundfonts WRITE setSoundfonts NOTIFY soundfontsChanged)
    Q_PROPERTY(QStringList bankoffsets READ bankoffsets WRITE setBankoffsets NOTIFY bankoffsetsChanged)
    Q_PROPERTY(SynthSettings* settings READ settings WRITE setSettings NOTIFY settingsChanged)

public:
    explicit SynthEngine(QObject *parent = 0);
    ~SynthEngine();

    float gain() const;
    void setGain(float gain);

    QStringList soundfonts() const;
    void setSoundfonts(const QStringList &soundfonts);

    QStringList bankoffsets() const;
    void setBankoffsets(const QStringList &bankoffsets);

    fluid_synth_t *synthHandle() const { return m_fluidSynth; }
    bool active() const;

    SynthSettings *settings() const;
    void setSettings(SynthSettings *settings);

public slots:
    bool start();
    bool restart();
    void stop();

    void setPreset(int channel, SynthPreset *preset);

signals:
    void activeChanged();
    void gainChanged();
    void soundfontsChanged();
    void bankoffsetsChanged();
    void settingsChanged();
    void midiKeyPressed(int value);
    void midiKeyReleased(int value);

private:

    fluid_synth_t *m_fluidSynth;
    fluid_audio_driver_t *m_fluidAudioDriver;
    fluid_midi_router_t *m_fluidMidiRouter;
    fluid_midi_driver_t *m_fluidMidiDriver;

    int m_midiEvent;
    int m_midiState;

    //Property members
    bool m_active;
    float m_gain;

    QStringList m_soundfonts;
    QStringList m_bankoffsets;
    SynthSettings *m_settings;

    SynthAudioOutput *m_audioOutput;
};

Q_DECLARE_METATYPE(SynthEngine*)

#endif // SYNTHENGINE_H
