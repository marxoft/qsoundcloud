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

#include "streamsrequest.h"
#include "request_p.h"
#include "urls.h"
#include <QNetworkReply>
#ifdef QSOUNDCLOUD_DEBUG
#include <QDebug>
#endif

namespace QSoundCloud {

class Format : public QVariantMap
{

public:
    Format() :
        QVariantMap()
    {
    }
    
    Format(const QString &id, const QString &desc, const QString &ext, const QUrl &url) :
        QVariantMap()
    {
        insert("id", id);
        insert("description", desc);
        insert("ext", ext);
        insert("url", url);
    }
};

class StreamsRequestPrivate : public RequestPrivate
{

public:
    StreamsRequestPrivate(StreamsRequest *parent) :
        RequestPrivate(parent)
    {
    }
    
    void getRedirect(const QUrl &u, const char *slot) {
#ifdef QSOUNDCLOUD_DEBUG
        qDebug() << "StreamsRequestPrivate::getRedirect" << u;
#endif
        Q_Q(StreamsRequest);
        
        if (reply) {
            delete reply;
        }
        
        q->setUrl(u);
        reply = networkAccessManager()->head(buildRequest());
        StreamsRequest::connect(reply, SIGNAL(finished()), q, slot);
    }
    
    void _q_onDownloadRedirect() {
        if (!reply) {
            return;
        }
        
        if (reply->error() == QNetworkReply::NoError) {
            QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
            
            if (redirect.isEmpty()) {
                redirect = reply->header(QNetworkRequest::LocationHeader).toString();
            }
            
            if (!redirect.isEmpty()) {
                redirect.setScheme("http");
                QString ext = track.value("original_format").toString();
                formats << Format("original", QString("%1 (%2)").arg(StreamsRequest::tr("Original format")).arg(ext.toUpper()),
                                  ext, redirect);
            }
        }
        
        reply->deleteLater();
        reply = 0;
            
        if (track.value("streamable").toBool()) {
            getRedirect(track.value("stream_url").toString(), SLOT(_q_onStreamRedirect()));
        }
        else {
            Q_Q(StreamsRequest);
            
            setResult(formats);
            setStatus(Request::Ready);
            setError(Request::NoError);
            setErrorString(QString());
            
            emit q->finished();
        }
    }
    
    void _q_onStreamRedirect() {        
        if (!reply) {
            return;
        }
        
        Q_Q(StreamsRequest);
        
        if (reply->error() == QNetworkReply::NoError) {
            QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
            
            if (redirect.isEmpty()) {
                redirect = reply->header(QNetworkRequest::LocationHeader).toString();
            }
            
            if (!redirect.isEmpty()) {
                redirect.setScheme("http");
                formats << Format("mp3", "128kb MP3", "mp3", redirect);
            }
        }
        
        reply->deleteLater();
        reply = 0;
                    
        setResult(formats);
        setStatus(Request::Ready);
        setError(Request::NoError);
        setErrorString(QString());
        
        emit q->finished();
    }
    
    void _q_onReplyFinished() {
        if (!reply) {
            return;
        }
    
        Q_Q(StreamsRequest);
        
        if (redirects < MAX_REDIRECTS) {
            QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
    
            if (redirect.isEmpty()) {
                redirect = reply->header(QNetworkRequest::LocationHeader).toString();
            }
    
            if (!redirect.isEmpty()) {
                reply->deleteLater();
                reply = 0;
                followRedirect(redirect);
                return;
            }
        }
        
        QString response = QString::fromUtf8(reply->readAll());
        QNetworkReply::NetworkError e = reply->error();
        QString es = reply->errorString();
        reply->deleteLater();
        reply = 0;
        
        switch (e) {
        case QNetworkReply::NoError:
            break;
        case QNetworkReply::OperationCanceledError:
            setStatus(Request::Canceled);
            setError(Request::NoError);
            setErrorString(QString());
            emit q->finished();
            return;
        case QNetworkReply::AuthenticationRequiredError:
            if (refreshToken.isEmpty()) {
                setStatus(Request::Failed);
                setError(Request::Error(e));
                setErrorString(es);
                emit q->finished();
            }
            else {
                refreshAccessToken();
            }
        
            return;
        default:
            setStatus(Request::Failed);
            setError(Request::Error(e));
            setErrorString(es);
            emit q->finished();
            return;
        }
        
        bool ok = true;
        track = QtJson::Json::parse(response, ok).toMap();
        formats.clear();
        
        if (ok) {
            if (track.value("downloadable").toBool()) {
                getRedirect(track.value("download_url").toString(), SLOT(_q_onDownloadRedirect()));
                return;
            }
            
            if (track.value("streamable").toBool()) {
                getRedirect(track.value("stream_url").toString(), SLOT(_q_onStreamRedirect()));
                return;
            }
            
            setResult(formats);
            setStatus(Request::Ready);
            setError(Request::NoError);
            setErrorString(QString());
        }
        else {
            setStatus(Request::Failed);
            setError(Request::ParseError);
            setErrorString(Request::tr("Unable to parse response"));
        }

        emit q->finished();
    }
    
    QVariantList formats;
    QVariantMap track;
                    
    Q_DECLARE_PUBLIC(StreamsRequest)
};

/*!
    \class StreamsRequest
    \brief Handles requests for track streams
    
    \ingroup requests
    
    The StreamsRequest class is used for requesting a list of streams for a SoundCloud track.
    
    Example usage:
    
    C++
    
    \code
    using namespace QSoundCloud;
    
    ...
    
    StreamsRequest request;
    request.get(TRACK_ID);
    connect(&request, SIGNAL(finished()), this, SLOT(onRequestFinished()));
    
    ...
    
    void MyClass::onRequestFinished() {
        if (request.status() == StreamsRequest::Ready) {
            foreach (QVariant stream, request.result().toList()) {
                qDebug() << "ID:" << stream.value("id").toString();
                qDebug() << "Description:" << stream.value("description").toString();
                qDebug() << "Extension:" << stream.value("ext").toString();
                qDebug() << "URL:" << stream.value("url").toString();
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
    
    StreamsRequest {
        id: request
        
        onFinished: {
            if (status == StreamsRequest.Ready) {
                for (var i = 0; i < result.length; i++) {
                    console.log("ID: " + result[i].id);
                    console.log("Description: " + result[i].description);
                    console.log("Extension: " + result[i].ext);
                    console.log("URL: " + result[i].url);
                }
            }
            else {
                console.log(errorString);
            }
        }
        
        Component.onCompleted: get(TRACK_ID)
    }
    \endcode
*/
StreamsRequest::StreamsRequest(QObject *parent) :
    Request(*new StreamsRequestPrivate(this), parent)
{
}

/*!
    \brief Requests a list of streams for the track identified by id.
    
    \a id can be either a track id or a URL. If a URL is provided, 
    StreamsRequest will resolve the URL before retrieving the streams.
*/
void StreamsRequest::get(const QString &id) {
    if (id.startsWith("http")) {
        setUrl(API_URL + "/resolve?url=" + id);
    }
    else {
        setUrl(API_URL + "/tracks/" + id);
    }
    
    Request::get();
}

}

#include "moc_streamsrequest.cpp"
