QT += qml quick multimedia

linux {
    INCLUDEPATH += $$FLUIDSYNTH_INCLUDEPATH
    LIBS += -L$$FLUIDSYNTH_LIBDIR -lfluidsynth
    target.path = /opt/SuperfluousSynth/
    soundfonts.files = FluidR3_GM.sf2
    soundfonts.path = /usr/share/sounds/sf2/

    INSTALLS += target
}

SOURCES += \
    main.cpp \
    synthengine.cpp \
    synthsettings.cpp \
    synthplayer.cpp \
    synthsequencer.cpp \
    synthvirtualkeyboard.cpp \
    synthaudiooutput.cpp \
    chordrecognizer.cpp \
    chord.cpp \
    interval.cpp \
    chorddescriptor.cpp \
    synthpresetsmodel.cpp \
    synthpreset.cpp

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    main.qml \
    todo.txt \
    DebugTab.qml \
    Channels.qml \
    FluidR3_GM.sf2 \
    NoteViewer.qml \
    VexFlow.js \
    VexFlow/header.js \
    VexFlow/vex.js \
    VexFlow/flow.js \
    VexFlow/fraction.js \
    VexFlow/fonts/vexflow_font.js \
    VexFlow/glyph.js \
    VexFlow/tables.js \
    VexFlow/stave.js \
    VexFlow/staveconnector.js \
    VexFlow/tabstave.js \
    VexFlow/voice.js \
    VexFlow/voicegroup.js \
    VexFlow/modifier.js \
    VexFlow/modifiercontext.js \
    VexFlow/accidental.js \
    VexFlow/dot.js \
    VexFlow/tickcontext.js \
    VexFlow/tickable.js \
    VexFlow/note.js \
    VexFlow/bend.js \
    VexFlow/stavenote.js \
    VexFlow/tabnote.js \
    VexFlow/barnote.js \
    VexFlow/ghostnote.js \
    VexFlow/formatter.js \
    VexFlow/stavetie.js \
    VexFlow/stavehairpin.js \
    VexFlow/tabtie.js \
    VexFlow/tabslide.js \
    VexFlow/beam.js \
    VexFlow/vibrato.js \
    VexFlow/annotation.js \
    VexFlow/tuning.js \
    VexFlow/stavemodifier.js \
    VexFlow/keysignature.js \
    VexFlow/timesignature.js \
    VexFlow/clef.js \
    VexFlow/music.js \
    VexFlow/keymanager.js \
    VexFlow/renderer.js \
    VexFlow/stavebarline.js \
    VexFlow/stavevolta.js \
    VexFlow/staverepetition.js \
    VexFlow/stavesection.js \
    VexFlow/stavetempo.js \
    VexFlow/articulation.js \
    VexFlow/tremolo.js \
    VexFlow/raphaelcontext.js \
    VexFlow/tuplet.js \
    VexFlow/boundingbox.js \
    VexFlow/textnote.js \
    VexFlow/strokes.js \
    VexFlow/stringnumber.js \
    VexFlow/frethandfinger.js \
    notevalues.js \
    README \
    LICENSE \
    MainView.qml \
    SplashScreen.qml \
    InstrumentSelectionPage.qml \
    ChordViewerPage.qml \
    InfoMenuItem.qml

HEADERS += \
    synthengine.h \
    synthsettings.h \
    synthplayer.h \
    synthsequencer.h \
    synthvirtualkeyboard.h \
    synthaudiooutput.h \
    chordrecognizer.h \
    chord.h \
    interval.h \
    chorddescriptor.h \
    synthpresetsmodel.h \
    synthpreset.h
