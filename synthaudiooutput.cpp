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

#include "synthaudiooutput.h"

#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioFormat>

const int BufferSize = 512;

class FluidSynthIODevice : public QIODevice
{
public:
    FluidSynthIODevice(fluid_synth_t *fluidSynth, const QAudioFormat &format, QObject *parent)
        : QIODevice(parent)
        , m_synth(fluidSynth)
        , m_format(format)
    {

    }
    qint64 readData(char *data, qint64 maxlen)
    {
        //qDebug("readData called for %d", maxlen);
        int frames;
        int bytes;

        if (maxlen <= 0)
            return 0;
        if (maxlen < BufferSize)
            bytes = maxlen;
        else
            bytes = BufferSize;
        frames = m_format.framesForBytes(bytes);

        if (m_synth) {
            int result = fluid_synth_write_s16(m_synth, frames, data, 0, 2, data, 1, 2);
            if (result == FLUID_OK) {
                return bytes;
            }
        }
        return 0;
    }
    qint64 writeData(const char *data, qint64 len)
    {
        Q_UNUSED(data)
        Q_UNUSED(len)

        return 0;
    }
private:
    fluid_synth_t *m_synth;
    QAudioFormat m_format;
};

SynthAudioOutput::SynthAudioOutput(fluid_synth_t *fluidSynth, QObject *parent)
    : QObject(parent)
    , m_fluidSynth(fluidSynth)
    , m_audioOutput(0)
    , m_buffer(0)
{
}

SynthAudioOutput::~SynthAudioOutput()
{
    close();
}

void SynthAudioOutput::open()
{
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning("Audio format not supported by backend, can not play audio");
        return;
    }

    m_audioOutput = new QAudioOutput(format, this);
    connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(stateChanged(QAudio::State)));
    if (m_buffer) {
        m_buffer->close();
        delete m_buffer;
    }
    m_buffer = new FluidSynthIODevice(m_fluidSynth, format, this);
    m_buffer->open(QIODevice::ReadOnly);
    m_audioOutput->setBufferSize(BufferSize);
    m_audioOutput->start(m_buffer);
}

void SynthAudioOutput::close()
{
    if (m_audioOutput)
        m_audioOutput->stop();
    if (m_buffer) {
        m_buffer->close();
        delete m_buffer;
        m_buffer = 0;
    }
}

void SynthAudioOutput::stateChanged(QAudio::State state)
{
    switch (state) {
    case QAudio::ActiveState:
        //Audio Data is being processed
        //after start() has been called and there is audio data to process
        break;
    case QAudio::SuspendedState:
        //only if suspend is called
        break;
    case QAudio::StoppedState:
        //Audio device is closed, and not prococessing any audio data
        if (m_audioOutput->error() != QAudio::NoError) {
            switch (m_audioOutput->error()) {
            case QAudio::OpenError:
                qWarning("QAudio::OpenError");
                break;
            case QAudio::IOError:
                qWarning("QAudio::IOError");
                break;
            case QAudio::UnderrunError:
                qWarning("QAudio::UnderrungError");
                break;
            case QAudio::FatalError:
                qWarning("QAudio::FatalError");
                break;
            default:
                qWarning("QAudio::UnknownError");
                break;
            }
        }
        break;
    case QAudio::IdleState:
        break;
    default:
        break;
    }
}
