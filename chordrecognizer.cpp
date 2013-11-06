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

#include "chordrecognizer.h"
#include "chord.h"

ChordRecognizer::ChordRecognizer(QObject *parent)
    : QObject(parent)
    , m_chordName("")
{
    buildChordTable();
}

const QString ChordRecognizer::chordName()
{
    return m_chordName;
}

void ChordRecognizer::addNote(const QString &keyValue)
{
    m_noteValues.append(keyValue);
    if (m_noteValues.count() == 1) {
        m_chordName = keyValue.toUpper();
        emit chordNameChanged();
    } else {
        update();
    }
}

void ChordRecognizer::removeNote(const QString &keyValue)
{
    m_noteValues.removeOne(keyValue);
    if (m_noteValues.isEmpty()) {
        m_chordName = "";
        emit chordNameChanged();

    } else if (m_noteValues.count() == 1) {
        m_chordName = keyValue.toUpper();
        emit chordNameChanged();
    } else {
        update();
    }
}

void ChordRecognizer::update()
{
    bool foundChord = 0;
    ChordDescriptor chord = recognizeChord(m_noteValues, foundChord);
    if (foundChord) {
        m_chordName = chord.baseName().toUpper() + " " + chord.modifier();
    } else {
        m_chordName = "";
    }
    emit chordNameChanged();
}

void ChordRecognizer::buildChordTable()
{
    // recognize minor second = inverse of major 7th
    QList<QString> minorSecond = QList<QString>() << "c" << "c#";
    registerNewChordType(minorSecond,ChordDescriptor("c","minorsecond","c"));

    // recognize major second = inverse of minor seventh
    QList<QString> majorSecond = QList<QString>() << "c" << "d";
    registerNewChordType(majorSecond,ChordDescriptor("c","majorsecond","c"));

    // recognize minor third
    QList<QString> minorThird = QList<QString>() << "c" << "eb";
    registerNewChordType(minorThird,ChordDescriptor("c","minorthird","c"));

    // recognize major third
    QList<QString> majorThird = QList<QString>() << "c" << "e";
    registerNewChordType(majorThird,ChordDescriptor("c","majorthird","c"));

    // recognize perfect fourth == inverse of perfect fifth
    QList<QString> perfectFourth = QList<QString>() << "c" << "f";
    registerNewChordType(perfectFourth,ChordDescriptor("c","majorfourth","c"));

    // recognize augmented fourth (also known as tritone, or "diabolicus in musica" *shiver*)
    QList<QString> augmentedFourth = QList<QString>() << "c" << "f#";
    registerNewChordType(augmentedFourth,ChordDescriptor("c","augmentedfourth","c"));

    // perfect fifth (also known as PowerChord)
    QList<QString> perfectFifth = QList<QString>() << "c" << "g";
    registerNewChordType(perfectFifth,ChordDescriptor("c","perfectfifth","c"));

    // minor sixth == inverse of major third
    QList<QString> minorSixth = QList<QString>() << "c" << "ab";
    registerNewChordType(minorSixth,ChordDescriptor("c","minorsixth","c"));

    // major sixth == inverse of minor third
    QList<QString> majorSixth = QList<QString>() << "c" << "a";
    registerNewChordType(majorSixth,ChordDescriptor("c","majorsixth","c"));

    // minor seventh == inverse of major second
    QList<QString> minorSeventh = QList<QString>() << "c" << "bb";
    registerNewChordType(minorSeventh,ChordDescriptor("c","minorseventh","c"));

    // major seventh == inverse of minor second
    QList<QString> majorSeventh = QList<QString>() << "c" << "b";
    registerNewChordType(majorSeventh,ChordDescriptor("c","majorseventh","c"));

    // let's make this thing also recognize chords with three and more notes

    // recognize all major chords in all keys and inversions
    QList<QString> majorChord = QList<QString>() << "c" << "e" << "g";
    registerNewChordType(majorChord,ChordDescriptor("c","","c"));

    // recognize all minor chords in all keys and inversions
    QList<QString> minorChord = QList<QString>() << "c" << "eb" << "g";
    registerNewChordType(minorChord,ChordDescriptor("c","m","c"));

    // recognize all augmented triads in all keys and inversions
    QList<QString> augmentedTriad = QList<QString>() << "c" << "e" << "g#";
    registerNewChordType(augmentedTriad,ChordDescriptor("c","aug","c"));

    // recognize all diminished triads in all keys and inversions
    QList<QString> diminishedTriad = QList<QString>() << "c" << "e" << "gb";
    registerNewChordType(diminishedTriad,ChordDescriptor("c","dim5","c"));

    // recognize all suspended second triads in all keys and inversions == inversion of sus4
    QList<QString> sus2 = QList<QString>() << "c" << "d" << "g";
    registerNewChordType(sus2,ChordDescriptor("c","sus2","c"));

    // recognize all suspended fourth triads in all keys and inversions
    QList<QString> sus4 = QList<QString>() << "c" << "f" << "g";
    registerNewChordType(sus4,ChordDescriptor("c","sus4","c"));

    // recognize all seventh omit5 chords
    QList<QString> sevenOmit5 = QList<QString>() << "c" << "e" << "bb";
    registerNewChordType(sevenOmit5,ChordDescriptor("c","7(omit5)","c"));

    // recognize all seventh omit3 chords
    QList<QString> sevenOmit3 = QList<QString>() << "c" << "g" << "bb";
    registerNewChordType(sevenOmit3,ChordDescriptor("c","7(omit3)","c"));

    // recognize all major seventh omit5 chords
    QList<QString> majSevenOmit5 = QList<QString>() << "c" << "e" << "b";
    registerNewChordType(majSevenOmit5,ChordDescriptor("c","Maj7(omit5)","c"));

    // recognize all major seventh omit3 chords
    QList<QString> majSevenOmit3 = QList<QString>() << "c" << "g" << "b";
    registerNewChordType(majSevenOmit3,ChordDescriptor("c","Maj7(omit3)","c"));

    // recognize all minor seventh omit5 chords
    QList<QString> minor7omit5 = QList<QString>() << "c" << "eb" << "bb";
    registerNewChordType(minor7omit5,ChordDescriptor("c","m7(omit5)","c"));

    // recognize all minor seventh omit3 chords == inverse of 7(omit3)
    QList<QString> minor7omit3 = QList<QString>() << "c" << "g" << "bb";
    registerNewChordType(minor7omit3,ChordDescriptor("c","m7(omit3)","c"));

    // recognize all added fourth chords
    QList<QString> add4 = QList<QString>() << "c" << "e" << "f" << "g";
    registerNewChordType(add4,ChordDescriptor("c","add4","c"));

    // recognize all sixth chords == inverse of m7
    QList<QString> sixth = QList<QString>() << "c" << "e" << "g" << "a";
    registerNewChordType(sixth,ChordDescriptor("c","6","c"));

    // recognize all minor sixth chords == inverse of m7(b5)
    QList<QString> mSixth = QList<QString>() << "c" << "eb" << "g" << "a";
    registerNewChordType(mSixth,ChordDescriptor("c","m6","c"));

    // recognize all six/seven chords
    QList<QString> sixSeven = QList<QString>() << "c" << "e" << "g" << "a" << "bb";
    registerNewChordType(sixSeven,ChordDescriptor("c","6/7","c"));

    // recognize all minor six/seven chords
    QList<QString> minorSixSeven = QList<QString>() << "c" << "eb" << "g" << "a" << "bb";
    registerNewChordType(minorSixSeven,ChordDescriptor("c","m6/7","c"));

    // recognize all six/seven suspended chords
    QList<QString> susSixSeven = QList<QString>() << "c" << "f#" << "g" << "a" << "bb";
    registerNewChordType(susSixSeven,ChordDescriptor("c","6/7sus","c"));

    // recognize all minor added 9 chords
    QList<QString> sixSevenAdd9 = QList<QString>() << "c" << "eb" << "g" << "d";
    registerNewChordType(sixSevenAdd9,ChordDescriptor("c","m(9)","c"));

    // recognize all seventh chords
    QList<QString> seven = QList<QString>() << "c" << "e" << "g" << "bb";
    registerNewChordType(seven,ChordDescriptor("c","7","c"));

    // recognize all major 7th chords
    QList<QString> majorSeven = QList<QString>() << "c" << "e" << "g" << "b";
    registerNewChordType(majorSeven,ChordDescriptor("c","Maj7","c"));

    // recognize all minor 7th chords
    QList<QString> minorSeven = QList<QString>() << "c" << "eb" << "g" << "bb";
    registerNewChordType(minorSeven,ChordDescriptor("c","m7","c"));

    // recognize all seven sus4 chords
    QList<QString> minorSevenSus4 = QList<QString>() << "c" << "f" << "g" << "bb";
    registerNewChordType(minorSevenSus4,ChordDescriptor("c","7sus4","c"));

    // recognize all seven aug5 chords
    QList<QString> sevenAug5 = QList<QString>() << "c" << "e" << "g#" << "bb";
    registerNewChordType(sevenAug5,ChordDescriptor("c","7(#5)","c"));

    // recognize all minor seven aug5 chords
    QList<QString> majSevenAug5 = QList<QString>() << "c" << "e" << "g#" << "b";
    registerNewChordType(majSevenAug5,ChordDescriptor("c","Maj7(#5)","c"));

    // recognize all maj 7 dim 5 chords
    QList<QString> majSevenDim5 = QList<QString>() << "c" << "e" << "gb" << "b";
    registerNewChordType(majSevenDim5,ChordDescriptor("c","Maj7(b5)","c"));

    // recognize all seven dim5 chords
    QList<QString> sevenDim5 = QList<QString>() << "c" << "e" << "gb" << "bb";
    registerNewChordType(sevenDim5,ChordDescriptor("c","7(b5)","c"));

    // recognize all diminished 7 chords
    QList<QString> dimSeven = QList<QString>() << "c" << "eb" << "gb" << "a";
    registerNewChordType(dimSeven,ChordDescriptor("c","dim7","c"));

    // recognize all half-diminished 7 chords
    QList<QString> halfDimSeven = QList<QString>() << "c" << "eb" << "gb" << "bb";
    registerNewChordType(halfDimSeven,ChordDescriptor("c","m7(b5)","c"));

    // recognize all minor/major 7 chords
    QList<QString> minMajSeven = QList<QString>() << "c" << "eb" << "g" << "b";
    registerNewChordType(minMajSeven,ChordDescriptor("c","mMaj7","c"));

    // recognize all sixth added 9 chords
    QList<QString> sixAddNine = QList<QString>() << "c" << "d" << "e" << "a";
    registerNewChordType(sixAddNine,ChordDescriptor("c","6(9)","c"));

    // recognize all minor sixth/nine chords
    QList<QString> minorSixAddNine = QList<QString>() << "c" << "eb" << "g" << "a" << "d";
    registerNewChordType(minorSixAddNine,ChordDescriptor("c","m6(9)","c"));

    // recognize all minor sixth/nine omit 5 chords
    QList<QString> minorSixAddNine2 = QList<QString>() << "c" << "eb" << "a" << "d";
    registerNewChordType(minorSixAddNine2,ChordDescriptor("c","m6(omit5)(9)","c"));

    // recognize all seventh added ninth chords
    QList<QString> sevenAddNine = QList<QString>() << "c" << "d" << "e" << "bb";
    registerNewChordType(sevenAddNine,ChordDescriptor("c","7(9)","c"));

    // recognize all seventh augmented ninth chords
    QList<QString> sevenAugNine = QList<QString>() << "c" << "e" << "g" << "bb" << "d#";
    registerNewChordType(sevenAugNine,ChordDescriptor("c","7(#9)","c"));

    // recognize all seventh diminished ninth chords
    QList<QString> sevenDimNine = QList<QString>() << "c" << "c#" << "e" << "bb";
    registerNewChordType(sevenDimNine,ChordDescriptor("c","7(b9)","c"));

    // recognize all seventh aug11 chords == inversion of 7(b5)
    QList<QString> sevenAug11 = QList<QString>() << "c" << "e" << "f#" << "bb";
    registerNewChordType(sevenAug11,ChordDescriptor("c","7(#11)","c"));

    // recognize all major seventh added ninth chords
    QList<QString> majSevenAddNine = QList<QString>() << "c" << "d" << "e" << "b";
    registerNewChordType(majSevenAddNine,ChordDescriptor("c","Maj7(9)","c"));

    // recognize all major seventh added thirteen chords
    QList<QString> majSevenAdd13 = QList<QString>() << "c" << "e" << "g" << "a" << "b";
    registerNewChordType(majSevenAdd13,ChordDescriptor("c","Maj7(13)","c"));

    // recognize all major seventh augm 11th chords == inversion of Maj7(b5)
    QList<QString> majSevenAug11 = QList<QString>() << "c" << "e" << "f#" << "b";
    registerNewChordType(majSevenAug11,ChordDescriptor("c","Maj7(#11)","c"));

    // recognize all minor seven added 9 chords
    QList<QString> minSevenAdd9 = QList<QString>() << "c" << "d" << "eb" << "bb";
    registerNewChordType(minSevenAdd9,ChordDescriptor("c","m7(9)","c"));

    // recognize all minor/Major 7 added 9 chords
    QList<QString> minMajSevenAdd9 = QList<QString>() << "c" << "d" << "eb" << "b";
    registerNewChordType(minMajSevenAdd9,ChordDescriptor("c","mMaj7(9)","c"));

    // recognize all seven added 13 chords
    QList<QString> sevenAdd13 = QList<QString>() << "c" << "e" << "a" << "bb";
    registerNewChordType(sevenAdd13,ChordDescriptor("c","7(13)","c"));

    // recognize all seven diminished 13 chords == inversion of 7(#5)
    QList<QString> sevenDim13 = QList<QString>() << "c" << "e" << "g#" << "bb";
    registerNewChordType(sevenDim13,ChordDescriptor("c","7(b13)","c"));

    // recognize all 6/9 chords
    QList<QString> sixSlashNine = QList<QString>() << "c" << "e" << "g" << "a" << "d";
    registerNewChordType(sixSlashNine,ChordDescriptor("c","6/9","c"));

    // recognize all major 9th chords
    QList<QString> majNine = QList<QString>() << "c" << "e" << "g" << "b" << "d";
    registerNewChordType(majNine,ChordDescriptor("c","Maj9","c"));

    // recognize all 9 chords
    QList<QString> nine = QList<QString>() << "c" << "e" << "g" << "bb" << "d";
    registerNewChordType(nine,ChordDescriptor("c","9","c"));

    // recognize all minor 9 chords
    QList<QString> minorNine = QList<QString>() << "c" << "eb" << "g" << "bb" << "d";
    registerNewChordType(minorNine,ChordDescriptor("c","m9","c"));

    // recognize 9 aug5 chords
    QList<QString> nineAug5 = QList<QString>() << "c" << "e" << "g#" << "bb" << "d";
    registerNewChordType(nineAug5,ChordDescriptor("c","9(#5)","c"));

    // recognize 9 dim5 chords == inversion of 9(#5)
    QList<QString> nineDim5 = QList<QString>() << "c" << "e" << "gb" << "bb" << "d";
    registerNewChordType(nineDim5,ChordDescriptor("c","9(b5)","c"));

    // minor ninth dim5 chords
    QList<QString> minorNineDim5 = QList<QString>() << "c" << "eb" << "gb" << "bb" << "d";
    registerNewChordType(minorNineDim5,ChordDescriptor("c","m9(b5)","c"));

    // minor ninth aug5 chords
    QList<QString> minorNineAug5 = QList<QString>() << "c" << "eb" << "g#" << "bb" << "d";
    registerNewChordType(minorNineAug5,ChordDescriptor("c","m9(#5)","c"));

    // 11th chords
    QList<QString> eleven = QList<QString>() << "c" << "e" << "g" << "bb" << "d" << "f";
    registerNewChordType(eleven,ChordDescriptor("c","11","c"));

    // maj 11th chords
    QList<QString> maj11 = QList<QString>() << "c" << "e" << "g" << "b" << "d" << "f";
    registerNewChordType(maj11,ChordDescriptor("c","Maj11","c"));

    // min 11th chords
    QList<QString> minor11 = QList<QString>() << "c" << "eb" << "g" << "bb" << "d" << "f";
    registerNewChordType(minor11,ChordDescriptor("c","m11","c"));

    // min 11th dim5 chords
    QList<QString> minor11Dim5 = QList<QString>() << "c" << "eb" << "gb" << "bb" << "d" << "f";
    registerNewChordType(minor11Dim5,ChordDescriptor("c","m11(b5)","c"));

    // min 11th aug5 chords == inversion of m11
    QList<QString> minor11Aug5 = QList<QString>() << "c" << "eb" << "g#" << "bb" << "d" << "f";
    registerNewChordType(minor11Aug5,ChordDescriptor("c","m11(#5)","c"));

    // 13th chords
    QList<QString> thirteen = QList<QString>() << "c" << "e" << "g" << "bb" << "d" << "a";
    registerNewChordType(thirteen,ChordDescriptor("c","13","c"));
    QList<QString> thirteen2 = QList<QString>() << "c" << "e" << "g" << "bb" << "d" << "f" << "a";
    registerNewChordType(thirteen2,ChordDescriptor("c","13","c"));

    // maj 13th chord
    QList<QString> maj13 = QList<QString>() << "c" << "e" << "g" << "b" << "d" << "a";
    registerNewChordType(maj13,ChordDescriptor("c","Maj13","c"));

    // min 13 added 11 chord == inversion of 6/9 chord
    QList<QString> maj13Add11 = QList<QString>() << "c" << "eb" << "g" << "bb" << "f";
    registerNewChordType(maj13Add11,ChordDescriptor("c","m7(11)","c"));

    // minor 7th add13 == inversion of m6/7 chord
    QList<QString> min7Add13 = QList<QString>() << "c" << "eb" << "g" << "a" << "bb";
    registerNewChordType(min7Add13,ChordDescriptor("c","m7(13)","c"));

    // min 13th == inversion of 13 chord
    QList<QString> min13 = QList<QString>() << "c" << "eb" << "g" << "bb" << "d" << "f" << "a";
    registerNewChordType(min13,ChordDescriptor("c","m13","c"));

    // minor 9/major 7
    QList<QString> min9Maj7 = QList<QString>() << "c" << "eb" << "g" << "b" << "d";
    registerNewChordType(min9Maj7,ChordDescriptor("c","m9Maj7","c"));

    // major seventh aug11
    QList<QString> maj7Aug11 = QList<QString>() << "c" << "e" << "g" << "b" << "f#";
    registerNewChordType(maj7Aug11,ChordDescriptor("c","Maj7(#11)","c"));

    // major ninth aug11
    QList<QString> maj9Aug11 = QList<QString>() << "c" << "e" << "g" << "b" << "d" << "f#";
    registerNewChordType(maj9Aug11,ChordDescriptor("c","Maj9(#11)","c"));

    // seventh dim9
    QList<QString> sevenDim9 = QList<QString>() << "c" << "e" << "g" << "bb" << "db";
    registerNewChordType(sevenDim9,ChordDescriptor("c","7(b9)","c"));

    // seventh aug5aug9
    QList<QString> sevenAug5Aug9 = QList<QString>() << "c" << "e" << "g#" << "bb" << "d#";
    registerNewChordType(sevenAug5Aug9,ChordDescriptor("c","7(#5)(#9)","c"));

    // seven aug5dim9 == inversion of m9(b5)
    QList<QString> sevenAug5Dim9 = QList<QString>() << "c" << "e" << "g#" << "bb" << "db";
    registerNewChordType(sevenAug5Dim9,ChordDescriptor("c","7(#5)(b9)","c"));

    // seven aug11 == inversion of 7(b5)
    QList<QString> sevenAug11_2 = QList<QString>() << "c" << "e" << "g" << "bb" << "f#";
    registerNewChordType(sevenAug11_2,ChordDescriptor("c","7(#11)","c"));

    // ninth aug11
    QList<QString> nineAug11 = QList<QString>() << "c" << "e" << "g" << "bb" << "d" << "f#";
    registerNewChordType(nineAug11,ChordDescriptor("c","9(#11)","c"));

    // seventh dim9aug11
    QList<QString> sevenDim9Aug11 = QList<QString>() << "c" << "e" << "g" << "bb" << "db" << "f#";
    registerNewChordType(sevenDim9Aug11,ChordDescriptor("c","7(b9)(#11)","c"));

    // seventh aug9aug11
    QList<QString> sevenAug9Aug11 = QList<QString>() << "c" << "e" << "g" << "bb" << "d#" << "f#";
    registerNewChordType(sevenAug9Aug11,ChordDescriptor("c","7(#9)(#11)","c"));

    // thirteen dim5
    QList<QString> thirteenDim5 = QList<QString>() << "c" << "e" << "gb" << "bb" << "d" << "a";
    registerNewChordType(thirteenDim5,ChordDescriptor("c","13(b5)","c"));

    // thirteen dim9
    QList<QString> thirteenDim9 = QList<QString>() << "c" << "e" << "g" << "bb" << "dd" << "a";
    registerNewChordType(thirteenDim9,ChordDescriptor("c","13(b9)","c"));

    // thirteen aug11
    QList<QString> thirteenDim11 = QList<QString>() << "c" << "e" << "g" << "bb" << "d" << "f#" << "a";
    registerNewChordType(thirteenDim11,ChordDescriptor("c","13(#11)","c"));
}

ChordDescriptor ChordRecognizer::recognizeChord(const QList<QString> &theoryNotes, bool &chordRecognized) const
{
    // first try to look up the non-simplified chord (to find specially named inversions)
    QList<QString> noteList = Chord(theoryNotes).withoutDuplicateNotes();
    QList<int> intervalPattern = Chord(noteList).toIntervalPattern();
    QString key = Chord::patternToKey(intervalPattern);
    if (m_patternToDescriptor.contains(key))
    {
        chordRecognized = true;
        QPair<ChordDescriptor, unsigned int> Info = m_patternToDescriptor[key];
        return ChordDescriptor(noteList[Info.second], Info.first.modifier(), theoryNotes[0]);
    }

    // if that failed, look for inversions of simplified chord
    QList<QString> simplifiedChord = Chord(theoryNotes).simplify();
    QList<QList<QString> > inversions = Chord(simplifiedChord).createAllInversions();
    for (QList<QList<QString> >::const_iterator it = inversions.begin(); it != inversions.end(); ++it)
    {
        QList<int> intervalPattern = Chord(*it).toIntervalPattern();
        QString key = Chord::patternToKey(intervalPattern);
        if (m_patternToDescriptor.contains(key))
        {
            chordRecognized = true;
            QPair<ChordDescriptor, unsigned int> info = m_patternToDescriptor[key];
            return ChordDescriptor((*it)[info.second], info.first.modifier(), theoryNotes[0]);
        }
    }

    chordRecognized = false;
    return ChordDescriptor(theoryNotes[0],"major",theoryNotes[0]);
}

void ChordRecognizer::registerNewChordType(const QList<QString> &theoryNotes, const ChordDescriptor &chordDescriptor)
{
    QList<QString> simplifiedChord = Chord(theoryNotes).simplify();
    if (simplifiedChord != theoryNotes)
    {
        // first register the non-simplified chord as it may be a special name for an inversion
        QList<int> intervalPattern = Chord(Chord(theoryNotes).withoutDuplicateNotes()).toIntervalPattern();
        QString key = Chord::patternToKey(intervalPattern);
        unsigned int baseNoteIndex = theoryNotes.indexOf(QString(theoryNotes[0]));

        if (!m_patternToDescriptor.contains(key))
            m_patternToDescriptor[key] = QPair<ChordDescriptor, unsigned int>(chordDescriptor, baseNoteIndex);
    }

    QList<int> intervalPattern = Chord(simplifiedChord).toIntervalPattern();
    QString key = Chord::patternToKey(intervalPattern);
    unsigned int baseNoteIndex = simplifiedChord.indexOf(QString(theoryNotes[0]));

    if (!m_patternToDescriptor.contains(key))
        m_patternToDescriptor[key] = QPair<ChordDescriptor, unsigned int>(chordDescriptor, baseNoteIndex);
}
