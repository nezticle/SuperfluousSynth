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

#include "interval.h"

Interval::Interval(const QString &theoryNote1, const QString &theoryNote2)
:m_theoryNote1(theoryNote1)
,m_theoryNote2(theoryNote2)
{
    // possible note names
    m_diatonicScaleNotes << "a" << "b" << "c" << "d" << "e" << "f" << "g";

    // having support for enharmonic equivalent notes will help during music theory operations
    // when going back to midi, we can normalize everything back to sharps
    m_chromaticScaleNotes << (QList<QString>() << "c" << "b#" << "dbb")
                          << (QList<QString>() << "c#" << "bx" << "db")
                          << (QList<QString>() << "d" << "cx" << "ebb")
                          << (QList<QString>() << "d#" << "eb" << "fbb")
                          << (QList<QString>() << "e" << "dx" << "fb")
                          << (QList<QString>() << "f" << "e#" << "gbb")
                          << (QList<QString>() << "f#" << "ex" << "gb")
                          << (QList<QString>() << "g" << "fx" << "abb")
                          << (QList<QString>() << "g#" << "ab" )
                          << (QList<QString>() << "a" << "gx" << "bbb")
                          << (QList<QString>() << "a#" << "bb" << "cbb")
                          << (QList<QString>() << "b" << "ax" << "cb");
    unsigned int Index = 0;
    for (QList<QList<QString> >::const_iterator it = m_chromaticScaleNotes.begin();
         it != m_chromaticScaleNotes.end();
         ++it,++Index)
    {
        for (QList<QString>::const_iterator it2 = (*it).begin(); it2 != (*it).end(); ++it2)
        {
            m_chromaticNoteToIndex[*it2] = Index;
        }
    }

    // modifiers required during transposition
    m_modifiers[-12] = "";
    m_modifiers[-11] = "#";
    m_modifiers[-10] = "x";
    m_modifiers[-2] = "bb";
    m_modifiers[-1] = "b";
    m_modifiers[0] = "";
    m_modifiers[1] = "#";
    m_modifiers[2] = "x";
}

QString Interval::getFirstNote() const
{
    return m_theoryNote1;
}

QString Interval::getSecondNote() const
{
    return m_theoryNote2;
}

unsigned int Interval::getNoteNameDistance() const
{
    unsigned int firstNoteIndex = m_diatonicScaleNotes.indexOf(QString(m_theoryNote1[0]));
    unsigned int secondNoteIndex= m_diatonicScaleNotes.indexOf(QString(m_theoryNote2[0]));
    if (firstNoteIndex > secondNoteIndex)
        return secondNoteIndex+7-firstNoteIndex;
    else
        return secondNoteIndex-firstNoteIndex;
}

unsigned int Interval::getNumberOfSemitones() const
{
    unsigned int firstNoteIndex = m_chromaticNoteToIndex[m_theoryNote1];
    unsigned int secondNoteIndex = m_chromaticNoteToIndex[m_theoryNote2];
    int difference = secondNoteIndex - firstNoteIndex;
    if (difference < 0)
        return difference + 12;
    else
        return difference;
}

QList<QString> Interval::transposeTo(const QString &newTheoryNote) const
{
    int newNoteIndex = m_diatonicScaleNotes.indexOf(QString(m_theoryNote2[0])) +
            Interval(m_theoryNote1, newTheoryNote).getNoteNameDistance();
    if (newNoteIndex > 6)
        newNoteIndex -= 7;
    QString secondNote = m_diatonicScaleNotes[newNoteIndex];
    int modifierKey = Interval(m_theoryNote1, m_theoryNote2).getNumberOfSemitones()
            - Interval(newTheoryNote, secondNote).getNumberOfSemitones();
    return QList<QString>() << newTheoryNote << (secondNote + m_modifiers[modifierKey]);
}

QList<int> Interval::toIntervalPattern() const
{
    return QList<int>() << getNoteNameDistance() << getNumberOfSemitones();
}
