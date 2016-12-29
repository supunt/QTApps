#-------------------------------------------------
#
# Project created by QtCreator 2016-12-29T11:18:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TransportOrganizer
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


SOURCES += main.cpp\
        mainwindow.cpp \
    formCtrl/addmailclient.cpp \
    Config/config.cpp \
    Config/confighandler.cpp \
    Config/configitem.cpp \
    formCtrl/settingdialog.cpp \
    mailClient/smtpconnection.cpp \
    formCtrl/addnewvehicle.cpp \
    formCtrl/registernewdriver.cpp

HEADERS  += mainwindow.h \
    formCtrl/addmailclient.h \
    defs.h \
    Config/config.h \
    Config/confighandler.h \
    Config/configitem.h \
    formCtrl/settingdialog.h \
    mailClient/smtpconnection.h \
    formCtrl/addnewvehicle.h \
    formCtrl/registernewdriver.h

FORMS    += uiFiles/mainwindow.ui \
    uiFiles/settingdialog.ui \
    uiFiles/addmailclient.ui \
    uiFiles/addnewvehicle.ui \
    uiFiles/registernewdriver.ui

RESOURCES += \
    icons.qrc
