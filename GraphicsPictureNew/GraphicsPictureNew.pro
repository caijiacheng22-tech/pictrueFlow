QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicsPictureNew
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

# 所有源文件，确保一个都不少
SOURCES += \
    GraphicsPictrue.cpp \
    main.cpp \
    mainwindow.cpp \
    pictruebutton.cpp \
    pictruecntroller.cpp \
    pictrueitem.cpp \
    pictrueview.cpp \
    pictruewidget.cpp

# 所有头文件
HEADERS += \
    GraphicsPictrue.h \
    graphicspictrue_global.h \
    mainwindow.h \
    pictruebutton.h \
    pictruecntroller.h \
    pictrueitem.h \
    pictrueview.h \
    pictruewidget.h

FORMS += \
    mainwindow.ui \
    mainwindow1.ui

RESOURCES += \
    mainwindow.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
