#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T14:10:05
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(QtAwesome/QtAwesome.pri)

TARGET = RaceTimer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dropdowndelegate.cpp \
    competitorsmodel.cpp \
    competitor.cpp \
    category.cpp \
    newcompetitordialog.cpp \
    categoriesmodel.cpp \
    competitoritemdelegate.cpp \
    categoryitemdelegate.cpp \
    racetimingmodel.cpp \
    raceresultsmodel.cpp \
    racetimingitemdelegate.cpp \
    raceresultsitemdelegate.cpp \
    racenumberbox.cpp \
    result.cpp \
    tableview.cpp

HEADERS  += mainwindow.h \
    dropdowndelegate.h \
    datedelegate.h \
    competitorsmodel.h \
    competitor.h \
    category.h \
    newcompetitordialog.h \
    categoriesmodel.h \
    competitoritemdelegate.h \
    categoryitemdelegate.h \
    racetimingmodel.h \
    result.h \
    raceresultsmodel.h \
    raceresultsitemdelegate.h \
    racetimingitemdelegate.h \
    racenumberbox.h \
    tableview.h

FORMS    += mainwindow.ui \
    newcompetitordialog.ui
