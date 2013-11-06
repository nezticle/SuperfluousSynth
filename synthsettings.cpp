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

#include "synthsettings.h"
#include <QtCore/QStringListIterator>

SynthSettings::SynthSettings(QObject *parent) :
    QObject(parent)
{
}

SynthSettings::~SynthSettings()
{
    if (m_fluidSettings)
        ::delete_fluid_settings(m_fluidSettings);
}

bool SynthSettings::midiIn() const
{
    return m_midiIn;
}

void SynthSettings::setMidiIn(bool midiIn)
{
    if (m_midiIn != midiIn) {
        m_midiIn = midiIn;
        emit midiInChanged();
    }
}

QString SynthSettings::midiDriver() const
{
    return m_midiDriver;
}

void SynthSettings::setMidiDriver(const QString &midiDriver)
{
    if (m_midiDriver != midiDriver) {
        m_midiDriver = midiDriver;
        emit midiDriverChanged();
    }
}

QString SynthSettings::midiDevice() const
{
    return m_midiDevice;
}

void SynthSettings::setMidiDevice(const QString &midiDevice)
{
    if (m_midiDevice != midiDevice) {
        m_midiDevice = midiDevice;
        emit midiDeviceChanged();
    }
}

QString SynthSettings::midiBankSelect() const
{
    return m_midiBankSelect;
}

void SynthSettings::setMidiBankSelect(const QString &midiBankSelect)
{
    if (m_midiBankSelect != midiBankSelect) {
        m_midiBankSelect = midiBankSelect;
        emit midiBankSelectChanged();
    }
}

int SynthSettings::midiChannels() const
{
    return m_midiChannels;
}

void SynthSettings::setMidiChannels(int midiChannels)
{
    if (m_midiChannels != midiChannels) {
        m_midiChannels = midiChannels;
        emit midiChannelsChanged();
    }
}

QString SynthSettings::midiName() const
{
    return m_midiName;
}

void SynthSettings::setMidiName(const QString &midiName)
{
    if (m_midiName != midiName) {
        m_midiName = midiName;
        emit midiNameChanged();
    }
}

QString SynthSettings::audioDriver() const
{
    return m_audioDriver;
}

void SynthSettings::setAudioDriver(const QString &audioDriver)
{
    if (m_audioDevice != audioDriver) {
        m_audioDriver = audioDriver;
        emit audioDriverChanged();
    }
}

QString SynthSettings::audioDevice() const
{
    return m_audioDevice;
}

void SynthSettings::setAudioDevice(const QString &audioDevice)
{
    if (m_audioDevice != audioDevice) {
        m_audioDevice = audioDevice;
        emit audioDeviceChanged();
    }
}

QString SynthSettings::jackName() const
{
    return m_jackName;
}

void SynthSettings::setJackName(const QString &jackName)
{
    if (m_jackName != jackName) {
        m_jackName = jackName;
        emit jackNameChanged();
    }
}

bool SynthSettings::jackAutoConnect() const
{
    return m_jackAutoConnect;
}

void SynthSettings::setJackAutoConnect(bool jackAutoConnect)
{
    if (m_jackAutoConnect != jackAutoConnect) {
        m_jackAutoConnect = jackAutoConnect;
        emit jackAutoConnectChanged();
    }
}

bool SynthSettings::jackMulti() const
{
    return m_jackMulti;
}

void SynthSettings::setJackMulti(bool jackMulti)
{
    if (m_jackMulti != jackMulti) {
        m_jackMulti = jackMulti;
        emit jackMultiChanged();
    }
}

int SynthSettings::audioChannels() const
{
    return m_audioChannels;
}

void SynthSettings::setAudioChannels(int audioChannels)
{
    if (m_audioChannels != audioChannels) {
        m_audioChannels = audioChannels;
        emit audioChannelsChanged();
    }
}

int SynthSettings::audioGroups() const
{
    return m_audioGroups;
}

void SynthSettings::setAudioGroups(int audioGroups)
{
    if (m_audioGroups != audioGroups) {
        m_audioGroups = audioGroups;
        emit audioGroupsChanged();
    }
}

int SynthSettings::audioBufferSize() const
{
    return m_audioBufferSize;
}

void SynthSettings::setAudioBufferSize(int audioBufferSize)
{
    if (m_audioBufferSize != audioBufferSize) {
        m_audioBufferSize = audioBufferSize;
        emit audioBufferSizeChanged();
    }
}

int SynthSettings::audioBufferCount() const
{
    return m_audioBufferCount;
}

void SynthSettings::setAudioBufferCount(int audioBufferCount)
{
    if (m_audioBufferCount != audioBufferCount) {
        m_audioBufferCount = audioBufferCount;
        emit audioBufferCountChanged();
    }
}

QString SynthSettings::sampleFormat() const
{
    return m_sampleFormat;
}

void SynthSettings::setSampleFormat(const QString &sampleFormat)
{
    if (m_sampleFormat != sampleFormat) {
        m_sampleFormat = sampleFormat;
        emit sampleFormatChanged();
    }
}

float SynthSettings::sampleRate() const
{
    return m_sampleRate;
}

void SynthSettings::setSampleRate(float sampleRate)
{
    if (m_sampleRate != sampleRate) {
        m_sampleRate = sampleRate;
        emit sampleRateChanged();
    }
}

int SynthSettings::polyphony() const
{
    return m_polyphony;
}

void SynthSettings::setPolyphony(int polyphony)
{
    if (m_polyphony != polyphony) {
        m_polyphony = polyphony;
        emit polyphonyChanged();
    }
}

bool SynthSettings::reverbActive() const
{
    return m_reverbActive;
}

void SynthSettings::setReverbActive(bool reverbActive)
{
    if (m_reverbActive != reverbActive) {
        m_reverbActive = reverbActive;
        emit reverbActiveChanged();
    }
}

double SynthSettings::reverbRoom() const
{
    return m_reverbRoom;
}

void SynthSettings::setReverbRoom(double reverbRoom)
{
    if (m_reverbRoom != reverbRoom) {
        m_reverbRoom = reverbRoom;
        emit reverbRoomChanged();
    }
}

double SynthSettings::reverbDamp() const
{
    return m_reverbDamp;
}

void SynthSettings::setReverbDamp(double reverbDamp)
{
    if (m_reverbDamp != reverbDamp) {
        m_reverbDamp = reverbDamp;
        emit reverbDampChanged();
    }
}

double SynthSettings::reverbWidth() const
{
    return m_reverbWidth;
}

void SynthSettings::setReverbWidth(double reverbWidth)
{
    if (m_reverbWidth != reverbWidth) {
        m_reverbWidth = reverbWidth;
        emit reverbWidthChanged();
    }
}

double SynthSettings::reverbLevel() const
{
    return m_reverbLevel;
}

void SynthSettings::setReverbLevel(double reverbLevel)
{
    if (m_reverbLevel != reverbLevel) {
        m_reverbLevel = reverbLevel;
        emit reverbLevelChanged();
    }
}

bool SynthSettings::chorusActive() const
{
    return m_chorusActive;
}

void SynthSettings::setChorusActive(bool chorusActive)
{
    if (m_chorusActive != chorusActive) {
        m_chorusActive = chorusActive;
        emit chorusActiveChanged();
    }
}

int SynthSettings::chorusNr() const
{
    return m_chorusNr;
}

void SynthSettings::setChorusNr(int chorusNr)
{
    if (m_chorusNr != chorusNr) {
        m_chorusNr = chorusNr;
        emit chorusNrChanged();
    }
}

double SynthSettings::chorusLevel() const
{
    return m_chorusLevel;
}

void SynthSettings::setChorusLevel(double chorusLevel)
{
    if (m_chorusLevel != chorusLevel) {
        m_chorusLevel = chorusLevel;
        emit chorusLevelChanged();
    }
}

double SynthSettings::chorusSpeed() const
{
    return m_chorusSpeed;
}

void SynthSettings::setChorusSpeed(double chorusSpeed)
{
    if (m_chorusSpeed != chorusSpeed) {
        m_chorusSpeed = chorusSpeed;
        emit chorusSpeedChanged();
    }
}

double SynthSettings::chorusDepth() const
{
    return m_chorusDepth;
}

void SynthSettings::setChorusDepth(double chorusDepth)
{
    if (m_chorusDepth != chorusDepth) {
        m_chorusDepth = chorusDepth;
        emit chorusDepthChanged();
    }
}

int SynthSettings::chorusType() const
{
    return m_chorusType;
}

void SynthSettings::setChorusType(int chorusType)
{
    if (m_chorusType != chorusType) {
        m_chorusType = chorusType;
        emit chorusTypeChanged();
    }
}

bool SynthSettings::ladspaActive() const
{
    return m_ladspaActive;
}

void SynthSettings::setLadspaActive(bool ladspaActive)
{
    if (m_ladspaActive != ladspaActive) {
        m_ladspaActive = ladspaActive;
        emit ladspaActiveChanged();
    }
}

QStringList SynthSettings::options() const
{
    return m_options;
}

void SynthSettings::setOptions(const QStringList &options)
{
    m_options = options;
    emit optionsChanged();
}

void SynthSettings::commitSettings()
{
    if (m_fluidSettings)
        ::delete_fluid_settings(m_fluidSettings);

    m_fluidSettings = ::new_fluid_settings();

    if ((m_audioGroups == 0) && m_audioChannels != 0) {
        m_audioGroups = m_audioChannels;
    }

    // We'll need these to avoid pedandic compiler warnings...
    char *pszKey;
    char *pszVal;

    // First we'll force all other conmmand line options...
    if (!m_midiDriver.isEmpty()) {
        pszKey = (char *) "midi.driver";
        ::fluid_settings_setstr(m_fluidSettings, pszKey,m_midiDriver.toLocal8Bit().data());
    }
    if (m_midiDriver == "alsa_seq" || m_midiDriver == "coremidi") {
        QString sKey = "midi." + m_midiDriver + ".id";
        if (!m_midiName.isEmpty()) {
            ::fluid_settings_setstr(m_fluidSettings, sKey.toLocal8Bit().data(),  m_midiName.toLocal8Bit().data());
        }
    }
    else
        if (!m_midiDevice.isEmpty()) {
            QString sMidiKey = "midi.";
            if (m_midiDriver == "alsa_raw")
                sMidiKey += "alsa";
            else
                sMidiKey += m_midiDriver;
            sMidiKey += ".device";
            ::fluid_settings_setstr(m_fluidSettings, sMidiKey.toLocal8Bit().data(), m_midiDevice.toLocal8Bit().data());
        }

    if (!m_audioDriver.isEmpty()) {
        pszKey = (char *) "audio.driver";
        ::fluid_settings_setstr(m_fluidSettings, pszKey, m_audioDriver.toLocal8Bit().data());
    }
    if (!m_audioDevice.isEmpty()) {
        QString sAudioKey = "audio." + m_audioDriver + '.';
        if (m_audioDriver == "file")
            sAudioKey += "name";
        else
            sAudioKey += "device";
        ::fluid_settings_setstr(m_fluidSettings, sAudioKey.toLocal8Bit().data(), m_audioDevice.toLocal8Bit().data());
    }
    if (!m_jackName.isEmpty()) {
        pszKey = (char *) "audio.jack.id";
        ::fluid_settings_setstr(m_fluidSettings, pszKey, m_jackName.toLocal8Bit().data());
    }

    pszKey = (char *) "audio.jack.autoconnect";
    ::fluid_settings_setint(m_fluidSettings, pszKey, int(m_jackAutoConnect));

    pszKey = (char *) "audio.jack.multi";
    pszVal = (char *) (m_jackMulti ? "yes" : "no");
    ::fluid_settings_setstr(m_fluidSettings, pszKey, pszVal);

    if (!m_sampleFormat.isEmpty()) {
        pszKey = (char *) "audio.sample-format";
        ::fluid_settings_setstr(m_fluidSettings, pszKey, m_sampleFormat.toLocal8Bit().data());
    }
    if (m_audioBufferSize > 0) {
        pszKey = (char *) "audio.period-size";
        ::fluid_settings_setint(m_fluidSettings, pszKey, m_audioBufferSize);
    }
    if (m_audioBufferCount > 0) {
        pszKey = (char *) "audio.periods";
        ::fluid_settings_setint(m_fluidSettings, pszKey, m_audioBufferCount);
    }
    if (m_midiChannels > 0) {
        pszKey = (char *) "synth.midi-channels";
        ::fluid_settings_setint(m_fluidSettings, pszKey, m_midiChannels);
    }

    pszKey = (char *) "synth.midi-bank-select";
    ::fluid_settings_setstr(m_fluidSettings, pszKey, m_midiBankSelect.toLocal8Bit().data());

    if (m_audioChannels > 0) {
        pszKey = (char *) "synth.audio-channels";
        ::fluid_settings_setint(m_fluidSettings, pszKey, m_audioChannels);
    }
    if (m_audioGroups > 0) {
        pszKey = (char *) "synth.audio-groups";
        ::fluid_settings_setint(m_fluidSettings, pszKey, m_audioGroups);
    }
    if (m_sampleRate > 0.0) {
        pszKey = (char *) "synth.sample-rate";
        ::fluid_settings_setnum(m_fluidSettings, pszKey, m_sampleRate);
    }
    if (m_polyphony > 0) {
        pszKey = (char *) "synth.polyphony";
        ::fluid_settings_setint(m_fluidSettings, pszKey, m_polyphony);
    }

    pszKey = (char *) "synth.reverb.active";
    pszVal = (char *) (m_reverbActive ? "yes" : "no");
    ::fluid_settings_setstr(m_fluidSettings, pszKey, pszVal);

    pszKey = (char *) "synth.chorus.active";
    pszVal = (char *) (m_chorusActive ? "yes" : "no");
    ::fluid_settings_setstr(m_fluidSettings, pszKey, pszVal);

    pszKey = (char *) "synth.ladspa.active";
    pszVal = (char *) (m_ladspaActive ? "yes" : "no");
    ::fluid_settings_setstr(m_fluidSettings, pszKey, pszVal);

    // Last we set user supplied options...
    QStringListIterator iter(m_options);
    while (iter.hasNext()) {
        const QString sOpt = iter.next();
        const QString sKey = sOpt.section('=', 0, 0);
        const QString sVal = sOpt.section('=', 1, 1);
        QByteArray tmp = sKey.toLocal8Bit();
        pszKey = tmp.data();
        switch (::fluid_settings_get_type(m_fluidSettings, pszKey)) {
        case FLUID_NUM_TYPE:
            ::fluid_settings_setnum(m_fluidSettings, pszKey,
                                    sVal.toFloat());
            break;
        case FLUID_INT_TYPE:
            ::fluid_settings_setint(m_fluidSettings, pszKey,
                                    sVal.toInt());
            break;
        case FLUID_STR_TYPE:
        default:
            ::fluid_settings_setstr(m_fluidSettings, pszKey,
                                    sVal.toLocal8Bit().data());
            break;
        }
    }
}
