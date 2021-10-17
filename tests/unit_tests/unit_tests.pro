include(../../defaults.pri)

QT -= qt core gui

CONFIG   -= app_bundle
CONFIG += c++17 console

LIBS += -L../../src -lKitsunemimiHanamiPersistence

LIBS += -L../../../libKitsunemimiSqlite/src -lKitsunemimiSqlite
LIBS += -L../../../libKitsunemimiSqlite/src/debug -lKitsunemimiSqlite
LIBS += -L../../../libKitsunemimiSqlite/src/release -lKitsunemimiSqlite
INCLUDEPATH += ../../../libKitsunemimiSqlite/include

LIBS += -L../../../libKitsunemimiCommon/src -lKitsunemimiCommon
LIBS += -L../../../libKitsunemimiCommon/src/debug -lKitsunemimiCommon
LIBS += -L../../../libKitsunemimiCommon/src/release -lKitsunemimiCommon
INCLUDEPATH += ../../../libKitsunemimiCommon/include

LIBS += -lsqlite3

INCLUDEPATH += $$PWD

SOURCES += \
    main.cpp  \
    users_test.cpp

HEADERS += \
    users_test.h
