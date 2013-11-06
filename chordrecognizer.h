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

#ifndef CHORDRECOGNIZER_H
#define CHORDRECOGNIZER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QPair>
#include <QtCore/QList>

#include "chorddescriptor.h"

class ChordRecognizer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString chordName READ chordName NOTIFY chordNameChanged)
public:
    explicit ChordRecognizer(QObject *parent = 0);

    const QString chordName();

signals:
    void chordNameChanged();

public slots:
    void addNote(const QString &keyValue);
    void removeNote(const QString &keyValue);

private:
    void update();
    void buildChordTable();
    ChordDescriptor recognizeChord(const QList<QString> &theoryNotes, bool &chordRecognized) const;
    void registerNewChordType(const QList<QString> &theoryNotes, const ChordDescriptor &chordDescriptor);

    QString m_chordName;
    QList<QString> m_noteValues;
    QHash<QString, QPair<ChordDescriptor, unsigned int> > m_patternToDescriptor;
    
};

#endif // CHORDRECOGNIZER_H
