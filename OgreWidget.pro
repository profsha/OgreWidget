#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T22:47:08
#
#-------------------------------------------------

QT       += core gui

TARGET = OgreWidget
TEMPLATE = app

unix {
    # You may need to change this include directory
    INCLUDEPATH += /usr/include/OGRE
    CONFIG += link_pkgconfig
    PKGCONFIG += OGRE
}
win32 {
    # You may need to change this include directory
    OGREHOME = D:\\cthugha\\SDK\\WIN32\\vc10\\sdk
    LIBS += -L$$OGREHOME\\boost_1_44\\lib
    Release:LIBS += -L$$OGREHOME\\lib\\release
    Debug:LIBS += -L$$OGREHOME\\lib\\debug
    INCLUDEPATH += $$OGREHOME\\include
    INCLUDEPATH += $$OGREHOME\\include\\OIS
    INCLUDEPATH += $$OGREHOME\\include\\OGRE
    INCLUDEPATH += $$OGREHOME\\boost_1_44
}
CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
    LIBS *= -lOgreMain_d -lOIS_d -lOgreTerrain_d
}
CONFIG(release, debug|release):LIBS *= -lOgreMain -lOIS -lOgreTerrain

SOURCES += main.cpp \
        ogrewidget.cpp


HEADERS  += ogrewidget.h
