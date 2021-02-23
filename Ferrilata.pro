#-------------------------------------------------
#
# Project created by QtCreator 2019-07-08T14:18:38
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =  Ferrilata
TEMPLATE =  app

CONFIG += C++11

SOURCES += src/main.cpp \
    src/Controllers/client.cpp \
    src/Controllers/server.cpp \
    src/Network/network.cpp \
    src/Network/responces.cpp \
    src/Network/package.cpp \
    src/Cryptography/cryptography.cpp \
    src/Cryptography/math.cpp \
    src/Network/wrappers.cpp \
    src/Settings/settings.cpp \
    src/Storage/contact.cpp \
    src/Storage/storage.cpp \
    src/UI/gui.cpp \
    src/UI/Widgets/dialogpreview.cpp \
    src/UI/MainWindow/mainwindow.cpp \
    src/UI/Authentification/authentification.cpp \
    src/UI/Widgets/message.cpp \
    src/UI/Widgets/dialog.cpp \


HEADERS += \
    src/Controllers/client.h \
    src/Controllers/server.h \
    src/Network/network.h \
    src/Network/responces.h \
    src/Network/package.h \
    src/Cryptography/cryptography.h \
    src/Cryptography/math.h \
    src/Network/wrappers.h \
    src/Settings/settings.h \
    src/Storage/contact.h \
    src/Storage/storage.h \
    src/UI/gui.h \
    src/UI/Widgets/dialogpreview.h \
    src/UI/MainWindow/mainwindow.h \
    src/UI/Authentification/authentification.h \
    src/UI/Widgets/message.h \
    src/UI/Widgets/dialog.h \ \
    src/typedefs.h


FORMS += src/UI/MainWindow/mainwindow.ui \
    src/UI/Authentification/authentification.ui \
    src/UI/Widgets/dialogpreview.ui \
    src/UI/Widgets/message.ui \


RESOURCES += foxes.qrc \
