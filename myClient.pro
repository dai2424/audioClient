QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    pages/fileupload.cpp \
    pages/page.cpp \
    pages\groupmanage.cpp \
    net\dispatcher.cpp \
    net\tcpsession.cpp \
    menu\menu.cpp \
    menu\menubutton.cpp \
    main.cpp




HEADERS += \
    pages/fileupload.h \
    pages/page.h \
    pages\groupmanage.h \
    net\dispatcher.h \
    net\tcpsession.h \
    menu\menubutton.h \
    menu\menu.h



FORMS +=


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    res.qrc
