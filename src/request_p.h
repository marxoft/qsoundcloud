/*
 * Copyright (C) 2015 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QSOUNDLCOUD_REQUEST_P_H
#define QSOUNDLCOUD_REQUEST_P_H

#include "request.h"
#include "json.h"
#include <QUrl>
#include <QVariantMap>
#include <QNetworkRequest>
#if QT_VERSION >= 0x050000
#include <QUrlQuery>
#endif
#ifdef QSOUNDLCOUD_DEBUG
#include <QDebug>
#endif

class QNetworkReply;

namespace QSoundCloud {

static const int MAX_REDIRECTS = 8;

#if QT_VERSION >= 0x050000
inline void addUrlQueryItems(QUrlQuery *query, const QVariantMap &map) {
#ifdef QSOUNDLCOUD_DEBUG
    qDebug() << "addUrlQueryItems:" << query << map;
#endif
    QMapIterator<QString, QVariant> iterator(map);
        
    while (iterator.hasNext()) {
        iterator.next();
        
        switch (iterator.value().type()) {
        case QVariant::String:
        case QVariant::ByteArray:
            query->addQueryItem(iterator.key(), iterator.value().toString());
            break;
        default:
            query->addQueryItem(iterator.key(), QtJson::Json::serialize(iterator.value()));
            break;
        }
    }
}

inline void addRequestHeaders(QNetworkRequest *request, const QVariantMap &map) {
#ifdef QSOUNDLCOUD_DEBUG
    qDebug() << "addRequestHeaders:" << request->url() << map;
#endif
    QMapIterator<QString, QVariant> iterator(map);
        
    while (iterator.hasNext()) {
        iterator.next();
        
        switch (iterator.value().type()) {
        case QVariant::String:
        case QVariant::ByteArray:
            request->setRawHeader(iterator.key().toUtf8(), iterator.value().toByteArray());
            break;
        default:
            request->setRawHeader(iterator.key().toUtf8(), QtJson::Json::serialize(iterator.value()));
            break;
        }
    }
}

inline void addPostBody(QString *body, const QVariantMap &map) {
#ifdef QSOUNDLCOUD_DEBUG
    qDebug() << "addPostBody:" << body << map;
#endif
    QMapIterator<QString, QVariant> iterator(map);
        
    while (iterator.hasNext()) {
        iterator.next();
        
        switch (iterator.value().type()) {
        case QVariant::String:
        case QVariant::ByteArray:
            body->append(iterator.key() + "=" + iterator.value().toString());
            break;
        default:
            body->append(iterator.key() + "=" + QtJson::Json::serialize(iterator.value()));
            break;
        }        
        
        if (iterator.hasNext()) {
            body->append("&");
        }
    }
}
#else
inline void addUrlQueryItems(QUrl *url, const QVariantMap &map) {
#ifdef QSOUNDLCOUD_DEBUG
    qDebug() << "addUrlQueryItems:" << url << map;
#endif
    QMapIterator<QString, QVariant> iterator(map);
        
    while (iterator.hasNext()) {
        iterator.next();
        
        switch (iterator.value().type()) {
        case QVariant::String:
        case QVariant::ByteArray:
            url->addQueryItem(iterator.key(), iterator.value().toString());
            break;
        case QVariant::Double: // In QtQuick 1.x, integers declared in JS are passed as doubles.
            url->addQueryItem(iterator.key(), QString::number(iterator.value().toInt()));
            break;
        default:
            url->addQueryItem(iterator.key(), QtJson::Json::serialize(iterator.value()));
            break;
        }
    }
}

inline void addRequestHeaders(QNetworkRequest *request, const QVariantMap &map) {
#ifdef QSOUNDLCOUD_DEBUG
    qDebug() << "addRequestHeaders:" << request->url() << map;
#endif
    QMapIterator<QString, QVariant> iterator(map);
        
    while (iterator.hasNext()) {
        iterator.next();
        
        switch (iterator.value().type()) {
        case QVariant::String:
        case QVariant::ByteArray:
            request->setRawHeader(iterator.key().toUtf8(), iterator.value().toByteArray());
            break;
        case QVariant::Double: // In QtQuick 1.x, integers declared in JS are passed as doubles.
            request->setRawHeader(iterator.key().toUtf8(), QByteArray::number(iterator.value().toInt()));
            break;
        default:
            request->setRawHeader(iterator.key().toUtf8(), QtJson::Json::serialize(iterator.value()));
            break;
        }
    }
}

inline void addPostBody(QString *body, const QVariantMap &map) {
#ifdef QSOUNDLCOUD_DEBUG
    qDebug() << "addPostBody:" << body << map;
#endif
    QMapIterator<QString, QVariant> iterator(map);
    
    while (iterator.hasNext()) {
        iterator.next();
        
        switch (iterator.value().type()) {
        case QVariant::String:
        case QVariant::ByteArray:
            body->append(iterator.key() + "=" + iterator.value().toString());
            break;
        case QVariant::Double: // In QtQuick 1.x, integers declared in JS are passed as doubles.
            body->append(iterator.key() + "=" + QString::number(iterator.value().toInt()));
            break;
        default:
            body->append(iterator.key() + "=" + QtJson::Json::serialize(iterator.value()));
            break;
        }        
        
        if (iterator.hasNext()) {
            body->append("&");
        }
    }
}
#endif

class RequestPrivate
{

public:
    RequestPrivate(Request *parent);
    virtual ~RequestPrivate();
    
    QNetworkAccessManager* networkAccessManager();
    
    void setOperation(Request::Operation op);
    
    void setStatus(Request::Status s);
    
    void setError(Request::Error e);
    
    void setErrorString(const QString &es);
    
    void setResult(const QVariant &res);
    
    virtual QNetworkRequest buildRequest(bool authRequired = true);
    virtual QNetworkRequest buildRequest(QUrl u, bool authRequired = true);
    
    virtual void followRedirect(const QUrl &redirect);
        
    virtual void _q_onReplyFinished();
    
    Request *q_ptr;
    
    QNetworkAccessManager *manager;
    
    QNetworkReply *reply;
    
    bool ownNetworkAccessManager;
    
    QString accessToken;
        
    QUrl url;
    
    QVariantMap headers;
    
    QVariant data;
    
    QVariant result;
    
    Request::Operation operation;
    
    Request::Status status;
    
    Request::Error error;
    
    QString errorString;
    
    int redirects;
    
    Q_DECLARE_PUBLIC(Request)
};

}

#endif // QSOUNDLCOUD_REQUEST_P_H
