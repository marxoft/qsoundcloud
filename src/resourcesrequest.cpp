/*
 * Copyright (C) 2015 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "resourcesrequest.h"
#include "request_p.h"
#include "urls.h"

namespace QSoundCloud {

/*!
    \class ResourcesRequest
    \brief Handles requests for SoundCloud resources.
    
    \ingroup requests
    
    The ResourcesRequest class is used for making requests to the SoundCloud Data API that concern 
    SoundCloud resources.
    
    Example usage:
    
    C++
    
    \code
    using namespace QSoundCloud;
    
    ...
    
    ResourcesRequest request;
    request.get("/tracks/TRACK_ID");
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == ResourcesRequest::Ready) {            
            QMapIterator<QString, QVariant> iterator(request.result().toMap());
            
            while (iterator.hasNext()) {
                iterator.next();
                qDebug() << iterator.key() << "=" << iterator.value();
            }
        }
        else {
            qDebug() << request.errorString();
        }
    }
    \endcode
    
    QML
    
    \code
    import QtQuick 1.0
    import QSoundCloud 1.0
    
    ResourcesRequest {
        id: request

        onFinished: {
            if (status == ResourcesRequest.Ready) {
                for (var att in result) {
                    console.log(att + " = " + result[att]);
                }
            }
            else {
                console.log(errorString);
            }
        }
        
        Component.onCompleted: get("/tracks/TRACK_ID")
    }
    \endcode
    
    For more details about SoundCloud resources, see the SoundCloud reference documentation 
    <a target="_blank" href="https://developers.soundcloud.com/docs/api/reference">here</a>.
*/
ResourcesRequest::ResourcesRequest(QObject *parent) :
    Request(parent)
{
}

/*!
    \brief Requests SoundCloud resource(s) from \a resourcePath.
    
    For example, to search tracks:
    
    \code
    ResourcesRequest request;
    QVariantMap filters;
    filters["limit"] = 10;
    filters["q"] = "Qt";
    request.get("/tracks", filters);
    \endcode
*/
void ResourcesRequest::get(const QString &resourcePath, const QVariantMap &filters) {
    if (status() == Loading) {
        return;
    }
    
    QUrl u(QString("%1%2%3").arg(API_URL).arg(resourcePath.startsWith("/") ? QString() : QString("/"))
                            .arg(resourcePath));
#if QT_VERSION >= 0x050000
    if (!filters.isEmpty()) {
        QUrlQuery query(u);
        addUrlQueryItems(&query, filters);
        u.setQuery(query);
    }
#else
    if (!filters.isEmpty()) {
        addUrlQueryItems(&u, filters);
    }
#endif
    setUrl(u);
    setData(QVariant());
    Request::get();
}

/*!
    \brief Inserts a SoundCloud resource into \a resourcePath using a PUT request.
    
    For example, to 'favorite' a track on behalf of the authenticated user:
    
    \code
    ResourcesRequest request;
    request.insert("/me/favorites/TRACK_ID");
    \endcode
*/
void ResourcesRequest::insert(const QString &resourcePath) {
    if (status() == Loading) {
        return;
    }
    
    QUrl u(QString("%1%2%3").arg(API_URL).arg(resourcePath.startsWith("/") ? QString() : QString("/"))
                            .arg(resourcePath));
    setUrl(u);
    setData(QVariant());
    put();
}

/*!
    \brief Inserts a new SoundCloud resource.
    
    For example, to insert a new connection on behalf of the authenticated user:
    
    \code
    ResourcesRequest request;
    QVariantMap connection;
    connection["service"] = "twitter";
    connection["redirect_uri"] = "http://mywebsite.com/soundcloud_connections";
    request.insert(album, "/me/connections");
    \endcode
*/
void ResourcesRequest::insert(const QVariantMap &resource, const QString &resourcePath) {
    if (status() == Loading) {
        return;
    }
    
    QUrl u(QString("%1%2%3").arg(API_URL).arg(resourcePath.startsWith("/") ? QString() : QString("/"))
                            .arg(resourcePath));
    QString body;
    addPostBody(&body, resource);
    setUrl(u);
    setData(body);
    post();
}

/*!
    \brief Updates the SoundCloud resource at \a resourcePath.
*/
void ResourcesRequest::update(const QString &resourcePath, const QVariantMap &resource) {
    if (status() == Loading) {
        return;
    }
    
    QUrl u(QString("%1%2%3").arg(API_URL).arg(resourcePath.startsWith("/") ? QString() : QString("/"))
                            .arg(resourcePath));
    QString body;
    addPostBody(&body, resource);
    setUrl(u);
    setData(body);
    put();
}

/*!
    \brief Deletes the SoundCloud resource at \a resourcePath.
    
    For example, to 'unfavorite' a track on behalf of the authenticated user:
    
    \code
    ResourcesRequest request;
    request.del("/me/favorites/TRACK_ID");
    \endcode
*/
void ResourcesRequest::del(const QString &resourcePath) {
    if (status() == Loading) {
        return;
    }
    
    QUrl u(QString("%1%2%3").arg(API_URL).arg(resourcePath.startsWith("/") ? QString() : QString("/"))
                            .arg(resourcePath));
    setUrl(u);
    setData(QVariant());
    deleteResource();
}

}
