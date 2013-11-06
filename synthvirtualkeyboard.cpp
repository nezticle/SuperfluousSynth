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

#include "synthvirtualkeyboard.h"
#include "synthengine.h"

SynthVirtualKeyboard::SynthVirtualKeyboard(QObject *parent) :
    QObject(parent)
{
}
SynthEngine *SynthVirtualKeyboard::engine() const
{
    return m_engine;
}

void SynthVirtualKeyboard::setEngine(SynthEngine *engine)
{
    if (m_engine != engine) {
        m_engine = engine;
        emit engineChanged();
    }
}

bool SynthVirtualKeyboard::noteOn(int channel, int key, int velocity)
{
    //channel 	MIDI channel number (0 to MIDI channel count - 1)
    //key       MIDI note number (0-127)
    //velocity 	MIDI velocity (0-127, 0=noteoff)
    if (m_engine && m_engine->active()) {
        return ::fluid_synth_noteon(m_engine->synthHandle(), channel, key, velocity);
    }
    return false;
}

bool SynthVirtualKeyboard::noteOff(int channel, int key)
{
    //channel 	MIDI channel number (0 to MIDI channel count - 1)
    //key       MIDI note number (0-127)
    if (m_engine && m_engine->active()) {
        return ::fluid_synth_noteoff(m_engine->synthHandle(), channel, key);
    }
    return false;
}
