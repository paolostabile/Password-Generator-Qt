QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    edit_password.cpp \
    main.cpp \
    mainwindow.cpp \
    save_password.cpp

HEADERS += \
    edit_password.h \
    mainwindow.h \
    save_password.h

FORMS += \
    edit_password.ui \
    mainwindow.ui \
    save_password.ui

RC_ICONS = PadLock.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    logo.qrc
