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

#ifndef SYNTHSETTINGS_H
#define SYNTHSETTINGS_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <fluidsynth.h>

class SynthSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool midiIn READ midiIn WRITE setMidiIn NOTIFY midiInChanged)
    Q_PROPERTY(QString midiDriver READ midiDriver WRITE setMidiDriver NOTIFY midiDriverChanged)
    Q_PROPERTY(QString midiDevice READ midiDevice WRITE setMidiDevice NOTIFY midiDeviceChanged)
    Q_PROPERTY(QString midiBankSelect READ midiBankSelect WRITE setMidiBankSelect NOTIFY midiBankSelectChanged)
    Q_PROPERTY(int midiChannels READ midiChannels WRITE setMidiChannels NOTIFY midiChannelsChanged)
    Q_PROPERTY(QString midiName READ midiName WRITE setMidiName NOTIFY midiNameChanged)
    Q_PROPERTY(QString audioDriver READ audioDriver WRITE setAudioDriver NOTIFY audioDriverChanged)
    Q_PROPERTY(QString audioDevice READ audioDevice WRITE setAudioDevice NOTIFY audioDeviceChanged)
    Q_PROPERTY(QString jackName READ jackName WRITE setJackName NOTIFY jackNameChanged)
    Q_PROPERTY(bool jackAutoConnect READ jackAutoConnect WRITE setJackAutoConnect NOTIFY jackAutoConnectChanged)
    Q_PROPERTY(bool jackMulti READ jackMulti WRITE setJackMulti NOTIFY jackMultiChanged)
    Q_PROPERTY(int audioChannels READ audioChannels WRITE setAudioChannels NOTIFY audioChannelsChanged)
    Q_PROPERTY(int audioGroups READ audioGroups WRITE setAudioGroups NOTIFY audioGroupsChanged)
    Q_PROPERTY(int audioBufferSize READ audioBufferSize WRITE setAudioBufferSize NOTIFY audioBufferSizeChanged)
    Q_PROPERTY(int audioBufferCount READ audioBufferCount WRITE setAudioBufferCount NOTIFY audioBufferCountChanged)
    Q_PROPERTY(QString sampleFormat READ sampleFormat WRITE setSampleFormat NOTIFY sampleFormatChanged)
    Q_PROPERTY(float sampleRate READ sampleRate WRITE setSampleRate NOTIFY sampleRateChanged)
    Q_PROPERTY(int polyphony READ polyphony WRITE setPolyphony NOTIFY polyphonyChanged)
    Q_PROPERTY(bool reverbActive READ reverbActive WRITE setReverbActive NOTIFY reverbActiveChanged)
    Q_PROPERTY(double reverbRoom READ reverbRoom WRITE setReverbRoom NOTIFY reverbRoomChanged)
    Q_PROPERTY(double reverbDamp READ reverbDamp WRITE setReverbDamp NOTIFY reverbDampChanged)
    Q_PROPERTY(double reverbWidth READ reverbWidth WRITE setReverbWidth NOTIFY reverbWidthChanged)
    Q_PROPERTY(double reverbLevel READ reverbLevel WRITE setReverbLevel NOTIFY reverbLevelChanged)
    Q_PROPERTY(bool chorusActive READ chorusActive WRITE setChorusActive NOTIFY chorusActiveChanged)
    Q_PROPERTY(int chorusNr READ chorusNr WRITE setChorusNr NOTIFY chorusNrChanged)
    Q_PROPERTY(double chorusLevel READ chorusLevel WRITE setChorusLevel NOTIFY chorusLevelChanged)
    Q_PROPERTY(double chorusSpeed READ chorusSpeed WRITE setChorusSpeed NOTIFY chorusSpeedChanged)
    Q_PROPERTY(double chorusDepth READ chorusDepth WRITE setChorusDepth NOTIFY chorusDepthChanged)
    Q_PROPERTY(int chorusType READ chorusType WRITE setChorusType NOTIFY chorusTypeChanged)
    Q_PROPERTY(bool ladspaActive READ ladspaActive WRITE setLadspaActive NOTIFY ladspaActiveChanged)
    Q_PROPERTY(QStringList options READ options WRITE setOptions NOTIFY optionsChanged)
public:
    explicit SynthSettings(QObject *parent = 0);
    ~SynthSettings();

    bool midiIn() const;
    void setMidiIn(bool midiIn);

    QString midiDriver() const;
    void setMidiDriver(const QString &midiDriver);

    QString midiDevice() const;
    void setMidiDevice(const QString &midiDevice);

    QString midiBankSelect() const;
    void setMidiBankSelect(const QString &midiBankSelect);

    int midiChannels() const;
    void setMidiChannels(int midiChannels);

    QString midiName() const;
    void setMidiName(const QString &midiName);

    QString audioDriver() const;
    void setAudioDriver(const QString &audioDriver);

    QString audioDevice() const;
    void setAudioDevice(const QString &audioDevice);

    QString jackName() const;
    void setJackName(const QString &jackName);

    bool jackAutoConnect() const;
    void setJackAutoConnect(bool jackAutoConnect);

    bool jackMulti() const;
    void setJackMulti(bool jackMulti);

    int audioChannels() const;
    void setAudioChannels(int audioChannels);

    int audioGroups() const;
    void setAudioGroups(int audioGroups);

    int audioBufferSize() const;
    void setAudioBufferSize(int audioBufferSize);

    int audioBufferCount() const;
    void setAudioBufferCount(int audioBufferCount);

    QString sampleFormat() const;
    void setSampleFormat(const QString &sampleFormat);

    float sampleRate() const;
    void setSampleRate(float sampleRate);

    int polyphony() const;
    void setPolyphony(int polyphony);

    bool reverbActive() const;
    void setReverbActive(bool reverbActive);

    double reverbRoom() const;
    void setReverbRoom(double reverbRoom);

    double reverbDamp() const;
    void setReverbDamp(double reverbDamp);

    double reverbWidth() const;
    void setReverbWidth(double reverbWidth);

    double reverbLevel() const;
    void setReverbLevel(double reverbLevel);

    bool chorusActive() const;
    void setChorusActive(bool chorusActive);

    int chorusNr() const;
    void setChorusNr(int chorusNr);

    double chorusLevel() const;
    void setChorusLevel(double chorusLevel);

    double chorusSpeed() const;
    void setChorusSpeed(double chorusSpeed);

    double chorusDepth() const;
    void setChorusDepth(double chorusDepth);

    int chorusType() const;
    void setChorusType(int chorusType);

    bool ladspaActive() const;
    void setLadspaActive(bool ladspaActive);

    QStringList options() const;
    void setOptions(const QStringList &options);

    fluid_settings_t *settingsHandle() const { return m_fluidSettings; }

public slots:
    void commitSettings();

signals:
    void midiInChanged();
    void midiDriverChanged();
    void midiDeviceChanged();
    void midiBankSelectChanged();
    void midiChannelsChanged();
    void midiNameChanged();
    void audioDriverChanged();
    void audioDeviceChanged();
    void jackNameChanged();
    void jackAutoConnectChanged();
    void jackMultiChanged();
    void audioChannelsChanged();
    void audioGroupsChanged();
    void audioBufferSizeChanged();
    void audioBufferCountChanged();
    void sampleFormatChanged();
    void sampleRateChanged();
    void polyphonyChanged();
    void reverbActiveChanged();
    void reverbRoomChanged();
    void reverbDampChanged();
    void reverbWidthChanged();
    void reverbLevelChanged();
    void chorusActiveChanged();
    void chorusNrChanged();
    void chorusLevelChanged();
    void chorusSpeedChanged();
    void chorusDepthChanged();
    void chorusTypeChanged();
    void ladspaActiveChanged();
    void optionsChanged();

private:
    //Fluidsynth handles
    fluid_settings_t *m_fluidSettings;

    bool m_midiIn;
    QString m_midiDriver;
    QString m_midiDevice;
    QString m_midiBankSelect;
    int m_midiChannels;
    QString m_midiName;
    QString m_audioDriver;
    QString m_audioDevice;
    QString m_jackName;
    bool m_jackAutoConnect;
    bool m_jackMulti;
    int m_audioChannels;
    int m_audioGroups;
    int m_audioBufferSize;
    int m_audioBufferCount;
    QString m_sampleFormat;
    float m_sampleRate;
    int m_polyphony;
    bool m_reverbActive;
    double m_reverbRoom;
    double m_reverbDamp;
    double m_reverbWidth;
    double m_reverbLevel;
    bool m_chorusActive;
    int m_chorusNr;
    double m_chorusLevel;
    double m_chorusSpeed;
    double m_chorusDepth;
    int m_chorusType;
    bool m_ladspaActive;

    QStringList m_options;
};

Q_DECLARE_METATYPE(SynthSettings*)

#endif // SYNTHSETTINGS_H
