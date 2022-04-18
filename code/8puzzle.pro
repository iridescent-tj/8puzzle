QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# CONFIG += c++11


DEFINES += QT_DEPRECATED_WARNINGS

TARGET = 8puzzle
TEMPLATE = app


SOURCES += main.cpp\
    drawgraph.cpp \
    heuristic_fun.cpp \
    puzzlesolver.cpp \
    test.cpp \
    ui.cpp \
    utils.cpp

HEADERS  += \
    drawgraph.h \
    puzzlesolver.h \
    utils.h

FORMS    += \
    puzzlesolver.ui
