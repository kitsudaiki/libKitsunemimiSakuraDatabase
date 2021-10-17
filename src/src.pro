QT -= qt core gui

TARGET = KitsunemimiHanamiPersistence
TEMPLATE = lib
CONFIG += c++17
VERSION = 0.1.0

LIBS += -L../../libKitsunemimiSqlite/src -lKitsunemimiSqlite
LIBS += -L../../libKitsunemimiSqlite/src/debug -lKitsunemimiSqlite
LIBS += -L../../libKitsunemimiSqlite/src/release -lKitsunemimiSqlite
INCLUDEPATH += ../../libKitsunemimiSqlite/include

LIBS += -L../../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../../libKitsunemimiCommon/include

LIBS += -lsqlite3

INCLUDEPATH += $$PWD \
               $$PWD/../include

HEADERS += \
    ../include/libKitsunemimiSakuraDatabase/users.h

SOURCES += \
    users.cpp

