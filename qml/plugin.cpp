#include "plugin.h"
#include "authenticationrequest.h"
#include "resourcesmodel.h"
#include "resourcesrequest.h"
#include "streamsmodel.h"
#if QT_VERSION >= 0x050000
#include <qqml.h>
#else
#include <qdeclarative.h>
#endif

namespace QSoundCloud {

void Plugin::registerTypes(const char *uri) {
    Q_ASSERT(uri == QLatin1String("QSoundCloud"));

    qmlRegisterType<AuthenticationRequest>(uri, 1, 0, "AuthenticationRequest");
    qmlRegisterType<ResourcesModel>(uri, 1, 0, "ResourcesModel");
    qmlRegisterType<ResourcesRequest>(uri, 1, 0, "ResourcesRequest");
    qmlRegisterType<StreamsModel>(uri, 1, 0, "StreamsModel");
    qmlRegisterType<StreamsRequest>(uri, 1, 0, "StreamsRequest");
}

}

QML_DECLARE_TYPE(QSoundCloud::AuthenticationRequest)
QML_DECLARE_TYPE(QSoundCloud::ResourcesModel)
QML_DECLARE_TYPE(QSoundCloud::ResourcesRequest)
QML_DECLARE_TYPE(QSoundCloud::StreamsModel)
QML_DECLARE_TYPE(QSoundCloud::StreamsRequest)
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(qsoundcloudplugin, QSoundCloud::Plugin)
#endif
