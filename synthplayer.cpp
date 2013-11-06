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

#include "synthplayer.h"
#include "synthengine.h"

#include <QtCore/QStringListIterator>
#include <QtCore/QDebug>

SynthPlayer::SynthPlayer(QObject *parent)
    : QObject(parent)
    , m_fluidPlayer(0)
    , m_engine(0)
    , m_status(Done)
{
    m_pollStatusTimer.setInterval(1000);
    m_pollStatusTimer.stop();

    connect(&m_pollStatusTimer, SIGNAL(timeout()), this, SLOT(updateStatus()));
}

SynthPlayer::~SynthPlayer()
{
    if (m_fluidPlayer)
        ::delete_fluid_player(m_fluidPlayer);
}

SynthEngine *SynthPlayer::engine() const
{
    return m_engine;
}

void SynthPlayer::setEngine(SynthEngine *engine)
{
    if (m_engine != engine) {
        m_engine = engine;
        emit engineChanged();

        if (m_engine) {
            connect(m_engine, SIGNAL(activeChanged()), this, SLOT(engineActivityChanged()));
            if (m_engine->active())
                engineActivityChanged();
        } else {
            disconnect(m_engine, SIGNAL(activeChanged()), this, SLOT(engineActivityChanged()));
        }
    }

}
QStringList SynthPlayer::midifiles() const
{
    return m_midifiles;
}

void SynthPlayer::setMidifiles(const QStringList &midifiles)
{
    m_midifiles = midifiles;
    emit midifilesChanged();
    updateMidiFiles();
}
int SynthPlayer::loop() const
{
    return m_loop;
}

void SynthPlayer::setLoop(int loop)
{
    m_loop = loop;
    emit loopChanged();
    if (m_fluidPlayer) {
        ::fluid_player_set_loop(m_fluidPlayer, loop);
    }
}

SynthPlayer::Status SynthPlayer::status() const
{
    return m_status;
}

bool SynthPlayer::play()
{
    if (m_fluidPlayer) {
        //End by calling play
        return ::fluid_player_play(m_fluidPlayer);
    }
    return false;
}

void SynthPlayer::stop()
{
    if (m_fluidPlayer) {
        ::fluid_player_stop(m_fluidPlayer);
    }
}

void SynthPlayer::engineActivityChanged()
{
    if (m_engine) {
        if (m_engine->active()) {
            createPlayer();
        } else {
            destroyPlayer();
        }
    }
}

void SynthPlayer::updateStatus()
{
    if (m_fluidPlayer) {
        int status = ::fluid_player_get_status(m_fluidPlayer);
        if (m_status != status) {
            m_status = (SynthPlayer::Status)status;
            emit statusChanged();
        }
    }
}

void SynthPlayer::createPlayer()
{
    //When the engine starts, we should create a new player
    if (m_engine->synthHandle()) {
        if (m_fluidPlayer) {
            ::delete_fluid_player(m_fluidPlayer);
        }
        m_fluidPlayer = ::new_fluid_player(m_engine->synthHandle());
        updateMidiFiles();
        m_pollStatusTimer.start();
        //TODO:Get the default property values
    }

}

void SynthPlayer::destroyPlayer()
{
    if (m_fluidPlayer) {
        ::delete_fluid_player(m_fluidPlayer);
        m_fluidPlayer = 0;
        m_pollStatusTimer.stop();
    }
}

int SynthPlayer::updateMidiFiles()
{
    QStringListIterator midiIterator(m_midifiles);
    int midiFiles = 0;
    if (m_fluidPlayer) {
        while (midiIterator.hasNext()) {
            const QString &filename = midiIterator.next();
            if (::fluid_is_midifile(filename.toLocal8Bit().data())) {
                if (::fluid_player_add(m_fluidPlayer, filename.toLocal8Bit().data()) >= 0) {
                    midiFiles++;
                } else {
                    qWarning() << "Failed to play MIDI file: " << filename;
                }
            }
        }
    }

    return midiFiles;
}
int SynthPlayer::bpm() const
{
    return m_bpm;
}

void SynthPlayer::setBpm(int bpm)
{
    m_bpm = bpm;
    m_midiTempo = (int) ((double) 60 * 1e6 / bpm);
    emit midiTempoChanged();
    emit bpmChanged();
    if (m_fluidPlayer) {
        ::fluid_player_set_bpm(m_fluidPlayer, bpm);
    }
}

int SynthPlayer::midiTempo() const
{
    return m_midiTempo;
}

void SynthPlayer::setMidiTempo(int midiTempo)
{
    m_midiTempo = midiTempo;
    emit midiTempoChanged();
    //TODO: calculate the new value of bpm and change the value
    if (m_fluidPlayer) {
        ::fluid_player_set_midi_tempo(m_fluidPlayer, midiTempo);
    }
}
