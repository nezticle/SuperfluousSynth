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

#include "synthpresetsmodel.h"
#include "synthengine.h"

SynthPresetsModel::SynthPresetsModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_engine(0)
{
    connect(this, SIGNAL(engineChanged()), this, SLOT(update()));
}

SynthPresetsModel::~SynthPresetsModel()
{
    foreach (SynthPreset *preset, m_presets)
        delete preset;
}

int SynthPresetsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_presets.count();
}

QVariant SynthPresetsModel::data(const QModelIndex &index, int role) const
{
    if ( index.row() < 0 || index.row() > m_presets.count())
            return QVariant();

    if (role == PresetRole) {
        SynthPreset *preset = m_presets.at(index.row());
        return QVariant::fromValue(preset);
    } else {
        return QVariant();
    }
}

QHash<int, QByteArray> SynthPresetsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PresetRole] = "preset";
    return roles;
}

void SynthPresetsModel::update()
{
    if (m_engine && m_engine->synthHandle()) {
        beginResetModel();
        if (m_presets.count()) {
            foreach (SynthPreset *preset, m_presets)
                delete preset;
        }

        //Iterate through all soundfounds, and add each preset to list
        fluid_synth_t *synth = m_engine->synthHandle();
        fluid_preset_t preset;
        int sfCount = ::fluid_synth_sfcount(synth);
        for (int i = 0; i < sfCount; ++i) {
            fluid_sfont_t *soundFont = ::fluid_synth_get_sfont(synth, i);
            if (soundFont) {
                soundFont->iteration_start(soundFont);
                while (soundFont->iteration_next(soundFont, &preset)) {
                    m_presets.append(new SynthPreset(&preset));
                }
            }
        }
        endResetModel();
    }
}

SynthEngine *SynthPresetsModel::engine() const
{
    return m_engine;
}

void SynthPresetsModel::setEngine(SynthEngine *engine)
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

void SynthPresetsModel::engineActivityChanged()
{
    if (m_engine) {
        if (m_engine->active()) {
            update();
        }
    }
}

