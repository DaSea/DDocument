#-------------------------------------------------
#
# Project created by QtCreator 2018-06-08T20:43:53
#
#-------------------------------------------------

# if you want use sql, add qt += sql
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DDocument
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += qt warn_on release

SOURCES += \
        main.cpp \
        dmainwidget.cpp \
    dsettingdialog.cpp \
    ddocsetting.cpp \
    ddocmentsinfodialog.cpp \
    ddocmentdisplaydialog.cpp \
    dinfotableview.cpp \
    ddocinfosqlite.cpp \
    dsearchinfodialog.cpp \
    daboutdialog.cpp

HEADERS += \
        dmainwidget.h \
    dsettingdialog.h \
    ddocsetting.h \
    ddocmentsinfodialog.h \
    ddocmentdisplaydialog.h \
    dinfotableview.h \
    ddocinfosqlite.h \
    dsearchinfodialog.h \
    daboutdialog.h

FORMS += \
        dmainwidget.ui \
    dsettingdialog.ui \
    ddocmentsinfodialog.ui \
    ddocmentdisplaydialog.ui \
    dsearchinfodialog.ui \
    daboutdialog.ui

RESOURCES += \
    res/res.qrc

#LIBS += -lsqlite3

isEmpty(BINDIR):BINDIR=/opt/DDocument
isEmpty(APPDIR):APPDIR=/usr/share/applications
isEmpty(DSRDIR):DSRDIR=/opt/DDocument

target.path = $$INSTROOT$$BINDIR
icon.path = $$INSTROOT$$BINDIR
icon.files = $$PWD/res/ddocument.svg
desktop.path = $$INSTROOT$$APPDIR
desktop.files = $$PWD/ddocument.desktop

INSTALLS += target desktop icon
