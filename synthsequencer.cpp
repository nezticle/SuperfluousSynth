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

#include "synthsequencer.h"
#include "synthengine.h"

SynthSequencer::SynthSequencer(QObject *parent)
    : QObject(parent)
    , m_engine(0)
    , m_synthID(0)
{
    m_sequencer = new_fluid_sequencer2(0);
}

SynthSequencer::~SynthSequencer()
{
    delete_fluid_sequencer(m_sequencer);
}

SynthEngine *SynthSequencer::engine() const
{
    return m_engine;
}

void SynthSequencer::setEngine(SynthEngine *engine)
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

static void sequencer_callback (unsigned int time, fluid_event_t *event, fluid_sequencer_t *seq, void *data)
{
    Q_UNUSED(event)
    Q_UNUSED(seq)
    SynthSequencer *sequencer = (SynthSequencer *)data;
    QMetaObject::invokeMethod(sequencer, "sequencerCallback", Qt::QueuedConnection, Q_ARG(int, time));
}

void SynthSequencer::engineActivityChanged()
{
    if (m_engine) {
        if (m_engine->active()) {
            //create
            m_synthID = fluid_sequencer_register_fluidsynth(m_sequencer, m_engine->synthHandle());
            m_clientID = fluid_sequencer_register_client(m_sequencer, "SynthSequencer", sequencer_callback, this);
            qDebug("created sequencer");
        } else {
            //destroy
            m_synthID = 0;
        }
    }
}
