TEMPLATE = lib

#DEFINES += QSOUNDCLOUD_DEBUG
#DEFINES += QSOUNDCLOUD_STATIC_LIBRARY

QT += network
QT -= gui

TARGET = qsoundcloud
DESTDIR = ../lib

HEADERS += \
    authenticationrequest.h \
    json.h \
    model.h \
    model_p.h \
    qsoundcloud_global.h \
    request.h \
    request_p.h \
    resourcesmodel.h \
    resourcesrequest.h \
    streamsmodel.h \
    streamsrequest.h \
    urls.h

SOURCES += \
    authenticationrequest.cpp \
    json.cpp \
    model.cpp \
    request.cpp \
    resourcesmodel.cpp \
    resourcesrequest.cpp \
    streamsmodel.cpp \
    streamsrequest.cpp
    
headers.files += \
    authenticationrequest.h \
    model.h \
    qsoundcloud_global.h \
    request.h \
    resourcesmodel.h \
    resourcesrequest.h \
    streamsmodel.h \
    streamsrequest.h \
    urls.h
    
symbian {
    TARGET.CAPABILITY += NetworkServices ReadUserData WriteUserData
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.EPOCHEAPSIZE = 0x20000 0x8000000
    TARGET.EPOCSTACKSIZE = 0x14000
}

contains(DEFINES,QSOUNDCLOUD_STATIC_LIBRARY) {
    CONFIG += staticlib
    headers.path = ../include/qsoundcloud
    QMAKE_POST_LINK = mkdir -p $$headers.path && cp $$headers.files $$headers.path
} else {
    CONFIG += create_prl
    INSTALLS += target headers

    !isEmpty(INSTALL_SRC_PREFIX) {
        target.path = $$INSTALL_SRC_PREFIX/lib
        headers.path = $$INSTALL_SRC_PREFIX/include/qsoundcloud
    } else {
        target.path = /usr/lib
        headers.path = /usr/include/qsoundcloud
    }
}
