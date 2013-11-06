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

#include "synthengine.h"
#include "synthaudiooutput.h"
#include "synthpreset.h"

#include <QtCore/QStringListIterator>
#include <QtCore/QDebug>

SynthEngine::SynthEngine(QObject *parent)
    : QObject(parent)
    , m_fluidSynth(0)
    , m_fluidAudioDriver(0)
    , m_fluidMidiRouter(0)
    , m_fluidMidiDriver(0)
    , m_midiEvent(0)
    , m_midiState(0)
    , m_active(false)
    , m_settings(0)
    , m_audioOutput(0)
{
}

SynthEngine::~SynthEngine()
{
}

float SynthEngine::gain() const
{
    return m_gain;
}

void SynthEngine::setGain(float gain)
{
    if (m_gain != gain) {
        m_gain = gain;
        //Actually change the gain in realtime
        if (m_fluidSynth)
            ::fluid_synth_set_gain(m_fluidSynth, m_gain);
        emit gainChanged();
    }
}

static int synth_handle_midi_event ( void *pvData, fluid_midi_event_t *pMidiEvent )
{
    SynthEngine *engine = (SynthEngine *)pvData;
    int eventType = fluid_midi_event_get_type(pMidiEvent);
    int noteValue = -1;
    switch (eventType) {
    case 144: //NOTE_ON
        noteValue = fluid_midi_event_get_key(pMidiEvent);
        QMetaObject::invokeMethod(engine, "midiKeyPressed", Qt::QueuedConnection, Q_ARG(int, noteValue));
        break;
    case 128: //NOTE_OFF
        noteValue = fluid_midi_event_get_key(pMidiEvent);
        QMetaObject::invokeMethod(engine, "midiKeyReleased", Qt::QueuedConnection, Q_ARG(int, noteValue));
        break;
    default:
        break;
    }

    //qDebug("event type: %d\n", fluid_midi_event_get_type(pMidiEvent));
    return ::fluid_synth_handle_midi_event(engine->synthHandle(), pMidiEvent);
}

bool SynthEngine::start()
{
    //Create a synth using the current settings

    //If we've already set up a synth, there is nothing to do
    if (m_fluidSynth)
        return true;

    //Commit current settings
    if (!m_settings) {
        //fatal error: no settings object set
        qWarning() << "failed to find SynthSettings Object assigned to settings\ncan not continue!";
        return false;
    }

    m_settings->commitSettings();

    //Create the Synth
    m_fluidSynth = ::new_fluid_synth(m_settings->settingsHandle());
    if (!m_fluidSynth) {
        qWarning("failed to create synthesizer");
        return false;
    }
    m_active = true;

    //Load soundfonts
    int i = 0;
    QStringListIterator iter(m_soundfonts);
    while (iter.hasNext()) {
        const QString& sFilename = iter.next();
        // Is it a soundfont file...
        if (fluid_is_soundfont(sFilename.toLocal8Bit().data())) {
            int iBankOffset = m_bankoffsets[i].toInt();
            int iSFID = ::fluid_synth_sfload(m_fluidSynth, sFilename.toLocal8Bit().data(), 1);
            if (iSFID < 0)
                qWarning() << "failed to load the soundfont: " << sFilename;
            else
                if (::fluid_synth_set_bank_offset(m_fluidSynth, iSFID, iBankOffset) < 0) {
                    qWarning() << "failed to set bank offset " << iBankOffset << " for soundfont: " << sFilename;
                }
        }
        i++;
    }

    //Start synthesis thread (audio driver)
    m_fluidAudioDriver  = new_fluid_audio_driver(m_settings->settingsHandle(), m_fluidSynth);
    if (!m_fluidAudioDriver) {
        qWarning() << "Failed to create the audio driver" << m_settings->audioDriver() << "\nfalling back to QtMultimedia";
        if (m_audioOutput) {
            m_audioOutput->close();
            delete m_audioOutput;
        }
        m_audioOutput = new SynthAudioOutput(m_fluidSynth, this);
        m_audioOutput->open();
    }

    //Start the midi router and link to synth
    if (m_settings->midiIn()) {
        m_fluidMidiRouter = ::new_fluid_midi_router(m_settings->settingsHandle(), synth_handle_midi_event, (void *)this);
        if (!m_fluidMidiRouter)
            qWarning() << "Failed to create MIDI input router " << m_settings->midiDriver() << "\n\nNo MIDI input will be available";
        else {
            ::fluid_synth_set_midi_router(m_fluidSynth, m_fluidMidiRouter);
            m_fluidMidiDriver = ::new_fluid_midi_driver(m_settings->settingsHandle(), ::fluid_midi_router_handle_midi_event, (void*)m_fluidMidiRouter);
            if (!m_fluidMidiDriver) {
                qWarning() << "Failed to create the MIDI driver " << m_settings->midiDriver() << "\n\nNo MIDI input will be available";
            }
        }
    }

    //set initial gain value
    ::fluid_synth_set_gain(m_fluidSynth, m_gain);
    emit activeChanged();

    return true;
}

bool SynthEngine::restart()
{
    stop();
    return start();
}

void SynthEngine::stop()
{
    if (!m_fluidSynth)
        return;

    // Destroy MIDI router.
    if (m_fluidMidiRouter) {
        if (m_fluidMidiDriver) {
            ::delete_fluid_midi_driver(m_fluidMidiDriver);
            m_fluidMidiDriver = 0;
        }
        ::delete_fluid_midi_router(m_fluidMidiRouter);
        m_fluidMidiRouter = 0;
    }

    // Destroy audio driver.
    if (m_fluidAudioDriver) {
        ::delete_fluid_audio_driver(m_fluidAudioDriver);
        m_fluidAudioDriver = 0;
    }
    if (m_audioOutput) {
        m_audioOutput->close();
        delete m_audioOutput;
        m_audioOutput = 0;
    }

    // Unload soundfonts from actual synth stack...
    int cSoundFonts = ::fluid_synth_sfcount(m_fluidSynth);
    for (int i = 0; i < cSoundFonts; i++) {
        fluid_sfont_t *pSoundFont = ::fluid_synth_get_sfont(m_fluidSynth, i);
        if (pSoundFont) {
            int iSFID = pSoundFont->id;
            const QString sName = pSoundFont->get_name(pSoundFont);
            if (::fluid_synth_sfunload(m_fluidSynth, iSFID, 0) < 0)
                qWarning() << "Failed to unload the soundfont: " << sName;
        }
    }

    // And finally, destroy the synthesizer engine.
    if (m_fluidSynth) {
        ::delete_fluid_synth(m_fluidSynth);
        m_fluidSynth = 0;
        m_active = false;
        emit activeChanged();
    }

}

void SynthEngine::setPreset(int channel, SynthPreset *preset)
{
    if (m_fluidSynth) {
        //::fluid_synth_set_preset(m_fluidSynth, channel, preset->handle());
        ::fluid_synth_bank_select(m_fluidSynth, channel, preset->bank());
        ::fluid_synth_program_change(m_fluidSynth, channel, preset->num());
        ::fluid_synth_program_reset(m_fluidSynth);
    }
}

SynthSettings *SynthEngine::settings() const
{
    return m_settings;
}

void SynthEngine::setSettings(SynthSettings *settings)
{
    m_settings = settings;
    emit settingsChanged();
}

QStringList SynthEngine::bankoffsets() const
{
    return m_bankoffsets;
}

void SynthEngine::setBankoffsets(const QStringList &bankoffsets)
{
    m_bankoffsets = bankoffsets;
    emit bankoffsetsChanged();
}

bool SynthEngine::active() const
{
    return m_active;
}

QStringList SynthEngine::soundfonts() const
{
    return m_soundfonts;
}

void SynthEngine::setSoundfonts(const QStringList &soundfonts)
{
    m_soundfonts = soundfonts;
    emit soundfontsChanged();
}

