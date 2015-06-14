TEMPLATE = lib
TARGET = qsoundcloudplugin
CONFIG += qt plugin
LIBS += -L../lib -lqsoundcloud

lessThan(QT_MAJOR_VERSION, 5) {
    QT += declarative
} else {
    QT += qml
}

INCLUDEPATH += ../src

HEADERS += \
    plugin.h

SOURCES += \
    plugin.cpp
    
contains(DEFINES,QSOUNDCLOUD_STATIC_LIBRARY) {
    CONFIG += link_pkgconfig
} else {
    CONFIG += link_prl
    PKGCONFIG = libqsoundcloud
}

qml.files = qmldir

!isEmpty(INSTALL_QML_PREFIX) {
    qml.path = $$INSTALL_QML_PREFIX/lib/qt4/imports/QSoundCloud
    target.path = $$INSTALL_QML_PREFIX/lib/qt4/imports/QSoundCloud
} else:lessThan(QT_MAJOR_VERSION, 5) {
    qml.path = $$[QT_INSTALL_IMPORTS]/QSoundCloud
    target.path = $$[QT_INSTALL_IMPORTS]/QSoundCloud
} else {
    qml.path = $$[QT_INSTALL_QML]/QSoundCloud
    target.path = $$[QT_INSTALL_QML]/QSoundCloud
}

INSTALLS += qml target
