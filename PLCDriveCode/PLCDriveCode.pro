#-------------------------------------------------
#
# Project created by QtCreator 2020-05-26T13:33:39
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PLCDriveCode
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
INCLUDEPATH +=. SDK/include
LIBS += -L../PLCDriveCode/SDK/lib -lec_control -lLogManager
CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        serialportcom.cpp \
    commandOp/commandfunop.cpp \
    ConfigfileRW/readhwconfigfile.cpp \
    ConfigfileRW/analysisdatalib.cpp \
    CommandFun/G00_Pulgin/g00_pulgin.cpp \
    commandOp/execommandsfun.cpp \
    commandOp/sendcommandsclass.cpp \
    commandOp/taskmangerclass.cpp


HEADERS += \
        mainwindow.h \
        serialdataframe.h \
        serialportcom.h \
    commandOp/commandfunop.h \
    ConfigfileRW/readhwconfigfile.h \
    ConfigfileRW/readhwconfigfile_global.h \
    ConfigfileRW/analysisdatalib.h \
    ConfigfileRW/analysisdatalib_global.h \
    myshareconfig.h \
    CommandFun/G00_Pulgin/g00_pulgin.h \
    commandOp/execommandsfun.h \
    commandOp/sendcommandsclass.h \
    commandOp/taskmangerclass.h


FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
