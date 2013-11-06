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

#include "chord.h"
#include "interval.h"

#include <QtCore/QSet>

Chord::Chord(const QList<QString> &theoryNotes, const QString &rootNote, unsigned int inversion)
: m_theoryNotes(theoryNotes)
, m_rootNote(rootNote)
, m_inversion(inversion)
{
    if (rootNote.isEmpty()) {
        m_rootNote = theoryNotes[0];
    }
}

QList<QString> Chord::transposeTo(const QString &newNote) const
{
    QList<QString> result;
    result << newNote;
    for (QList<QString>::const_iterator it = m_theoryNotes.begin()+1; it != m_theoryNotes.end(); ++it)
    {
        result << (Interval(m_theoryNotes[0], *it).transposeTo(newNote)[1]);
    }
    return result;
}

QList<int> Chord::toIntervalPattern() const
{
  // example ['c','e','g'] to interval pattern: [4,3]
  // because between c and e are 4 semitones,
  // and between e and g are 3 semitones
  if (m_theoryNotes.size() == 0)
      return QList<int>();
  else if (m_theoryNotes.size() == 1)
      return QList<int>() << 0;
  else
  {
      int index = 0;
      QList<int> result;
      while ((index+1) < m_theoryNotes.size())
      {
          result << Interval(m_theoryNotes[index], m_theoryNotes[index+1]).getNumberOfSemitones();
          index++;
      }
      return result;
  }
}



QList<QString> Chord::withoutDuplicateNotes() const
{
    // example: ['a','c','e','a','c'] without duplicate notes => ['a','c','e']
    // keep the ordering intact
    QSet<QString> alreadyEncounteredNotes;
    QList<QString> newListOfNotes;
    for (QList<QString>::const_iterator it = m_theoryNotes.begin(); it != m_theoryNotes.end(); ++it)
    {
        if (!alreadyEncounteredNotes.contains(*it))
        {
            newListOfNotes << *it;
            alreadyEncounteredNotes << *it;
        }
    }
    return newListOfNotes;
}



QList<QString> Chord::sortedRelativeToFirstNote() const
{
    // Example: sorting ['e','c','g','e'] relative to first note => ['e','g','c']
    if (m_theoryNotes.size() < 3)
        return m_theoryNotes; // already sorted

    // step 1: sort alphabetically; this allows to use inversions instead of permutations
    //         avoiding combinatorial explosion
    QList<QString> copyNotes = m_theoryNotes;
    qSort(copyNotes.begin(), copyNotes.end());

    // step 2: sort musically
    qSort(copyNotes.begin(), copyNotes.end(), Chord::musicSort);

    // step 3: make sure original first note appears first
    int IndexFirst = copyNotes.indexOf(m_theoryNotes[0]);
    QList<QString> resultPart1 = copyNotes.mid(IndexFirst,-1);
    QList<QString> resultPart2 = copyNotes.mid(0, IndexFirst);
    resultPart1.append(resultPart2);
    return resultPart1;
}


QList<QList<QString> > Chord::createAllInversions() const
{
    // example: ['c','e','g'] -> [ ['c','e',g'], ['e','g','c'], ['g','c','e'] ]
    QList<QList<QString> > result;
    for (int Index=0; Index < m_theoryNotes.size(); ++Index)
    {
        QList<QString> partialResult = m_theoryNotes.mid(Index,-1);
        QList<QString> partialResult2= m_theoryNotes.mid(0, Index);
        partialResult.append(partialResult2);
        result << partialResult;
    }

    return result;
}


QList<QString> Chord::simplify() const
{
    // the simplified chord is the chord with all notes rearranged so that they are spaced as closely as possible
    // e.g. ['a','e','c'] simplified => ['a','c','e']
    QList<QString> simplerChord = withoutDuplicateNotes();
    simplerChord = Chord(simplerChord).sortedRelativeToFirstNote();
    QList<QList<QString> > inversions = Chord(simplerChord).createAllInversions();
    QHash< QString, QList<QString> > minimumEnergyChordList; // map from interval pattern to list of theorynote
    QList< QList<int> > patList;
    int minimum = -1;
    for (QList<QList<QString> >::const_iterator it = inversions.begin(); it != inversions.end(); ++it)
    {
        QList<int> pat = Chord(*it).toIntervalPattern();
        int sum = 0;
        QListIterator<int> i(pat);
        while (i.hasNext())
            sum += i.next();
        if (sum < minimum || minimum == -1)
        {
            minimum = sum;
            minimumEnergyChordList[patternToKey(pat)] = *it;
            patList << pat;
        }
        else if (sum == minimum)
        {
            minimumEnergyChordList[patternToKey(pat)] = *it;
            patList << pat;
        }
    }
    QList<QList<int> > sortedPatList = patList;
    qSort(sortedPatList.begin(), sortedPatList.end(), patternSort);
    return minimumEnergyChordList[patternToKey(sortedPatList[0])];
}

bool Chord::musicSort(const QString &note1, const QString &note2)
{
    // sort based on interval size (#semitones between the notes)
    int d1 = Interval(note1,note2).getNumberOfSemitones();
    int d2 = Interval(note2,note1).getNumberOfSemitones();
    return (d1 < d2);
}

bool Chord::patternSort(const QList<int> &list1, const QList<int> &list2)
{
    // one list shorter than the other? sort based on length
    if (list1.length() < list2.length())
        return true;
    if (list1.length() > list2.length())
        return false;

    // both lists have same number of elements: perform lexicographic sorting
    QList<int>::const_iterator it1 = list1.begin();
    QList<int>::const_iterator it2 = list2.begin();
    for (;it1 != list1.end();++it1,++it2)
    {
        if (*it1 < *it2)
            return true;
        if (*it1 > *it2)
            return false;
        return patternSort(list1.mid(1), list2.mid(1));
    }
    return false;
}

QString Chord::patternToKey(const QList<int> &pattern)
{
    // convert interval pattern to QString
    QString key = "";
    for (QList<int>::const_iterator it = pattern.begin(); it != pattern.end(); ++it)
    {
        key += QString(*it);
    }
    return key;
}

bool Chord::operator ==(const Chord &other) const
{
    return m_theoryNotes == other.m_theoryNotes && m_rootNote == other.m_rootNote && m_inversion == other.m_inversion;
}
