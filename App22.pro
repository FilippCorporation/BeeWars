#-------------------------------------------------
#
# Project created by QtCreator 2018-12-18T17:25:45
#
#-------------------------------------------------

QT       += core gui websockets
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = App22
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    bugserver.cpp \
    gobee.cpp \
    players.cpp \
    bugclient.cpp \
    play_table.cpp \
    make_bee.cpp \
    game_plane.cpp \
    bombs.cpp \
    buferbee.cpp \
    decal.cpp

HEADERS += \
        mainwindow.h \
    bugserver.h \
    gobee.h \
    players.h \
    bugclient.h \
    play_table.h \
    make_bee.h \
    game_plane.h \
    bombs.h \
    buferbee.h \
    decal.h
FORMS += \
        mainwindow.ui
RESOURCES += \
    img.qrc\
../Chat/chat.qrc


#QMAKE_LFLAGS += -Wl,-rpath,"$ORIGIN/../build-Chat-Desktop_Qt_5_12_2_GCC_64bit-Debug/"






LIBS+= -L/home/itsmart/Project/the_bees/new_bee/build-Chat-Desktop_Qt_5_12_2_GCC_64bit-Debug/  -lChat

#unix{
#    Release{
#LIBS+= -L$$OUT_PWD/../build-Chat-Desktop_Qt_5_9_1_MinGW_32bit-Release/release -lChat
#    }
#    Debug{
    #LIBS+= -L"/home/itsmart/Project/пчелки/new bee/build-Chat-Desktop_Qt_5_12_2_GCC_64bit-Debug"  -lChat
    #LIBS+= -L$$OUT_PWD/../build-Chat-Desktop_Qt_5_12_2_GCC_64bit-Debug -lChat
#    }
#}

