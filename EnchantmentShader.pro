TEMPLATE = app
CONFIG += c++2a
CONFIG -= gui app_bundle

SOURCES += \
    Main.cpp

# SFML

INCLUDEPATH += D:/SFML-MinGW64/include
DEPENDPATH += D:/SFML-MinGW64/include
LIBS += -LD:/SFML-MinGW64/lib

CONFIG(release, debug|release) {
    DEFINES += SFML_STATIC
    LIBS +=               \
        -lsfml-graphics-s \
        -lsfml-window-s   \
        -lsfml-system-s   \
        -lopengl32        \
        -lfreetype        \
        -lsfml-window-s	  \
        -lsfml-system-s   \
        -lopengl32        \
        -lwinmm           \
        -lgdi32           \
        -lsfml-audio-s	  \
        -lsfml-system-s   \
        -lopenal32        \
        -lflac            \
        -lvorbisenc       \
        -lvorbisfile      \
        -lvorbis          \
        -logg             \
        -lsfml-network-s  \
        -lsfml-system-s   \
        -lws2_32          \
        -lsfml-system-s	  \
        -lwinmm
} else {
    DEFINES += SFML_DYNAMIC
    LIBS +=               \
        -lsfml-system-d   \
        -lsfml-window-d   \
        -lsfml-graphics-d \
        -lsfml-audio-d    \
        -lsfml-network-d
}

DISTFILES += \
    MainShader.frag
