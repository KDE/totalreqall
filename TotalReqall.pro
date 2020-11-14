QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += ./thirdparty/maddy/include

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Files from this project...
SOURCES += \
    Bible.cpp \
    ChooseReferenceWidget.cpp \
    MainWindow.cpp \
    MarkdownDialog.cpp \
    MemorizeEdit.cpp \
    MemorizeWidget.cpp \
    SettingsDialog.cpp \
    SimpleRefChooser.cpp \
    main.cpp

HEADERS += \
    AppInfo.h \
    Bible.h \
    ChooseReferenceWidget.h \
    MainWindow.h \
    MarkdownDialog.h \
    MemorizeEdit.h \
    MemorizeWidget.h \
    SettingsDialog.h \
    SimpleRefChooser.h

TRANSLATIONS += \
    TotalReqall_en_US.ts

RESOURCES += \
    bible.qrc \
    general.qrc \
    icons.qrc

VERSION = 0.5.0

win32
{
    RC_ICONS += resources/icons/TotalReqall.ico
}

macx
{
    ICON = resources/icons/TotalReqall.icns
}

linux
{
    INCLUDEPATH += \
        /usr/include/sword/
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -lsword
