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

#ifndef QSOUNDCLOUD_REQUEST_H
#define QSOUNDCLOUD_REQUEST_H

#include "qsoundcloud_global.h"
#include <QObject>
#include <QVariantMap>

class QUrl;
class QString;
class QNetworkAccessManager;

namespace QSoundCloud {

class RequestPrivate;

class QSOUNDCLOUDSHARED_EXPORT Request : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY clientIdChanged)
    Q_PROPERTY(QString clientSecret READ clientSecret WRITE setClientSecret NOTIFY clientSecretChanged)
    Q_PROPERTY(QString accessToken READ accessToken WRITE setAccessToken NOTIFY accessTokenChanged)
    Q_PROPERTY(QString refreshToken READ refreshToken WRITE setRefreshToken NOTIFY refreshTokenChanged)
    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)
    Q_PROPERTY(QVariantMap headers READ headers NOTIFY headersChanged)
    Q_PROPERTY(QVariant data READ data NOTIFY dataChanged)
    Q_PROPERTY(Operation operation READ operation NOTIFY operationChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(QVariant result READ result NOTIFY finished)
    Q_PROPERTY(Error error READ error NOTIFY finished)
    Q_PROPERTY(QString errorString READ errorString NOTIFY finished)
    
    Q_ENUMS(Operation Status Error)
    
public:
    enum Operation {
        HeadOperation = 1,
        GetOperation,
        PutOperation,
        PostOperation,
        DeleteOperation,

        UnknownOperation = 0
    };
    
    enum Status {
        Null = 0,
        Loading,
        Canceled,
        Ready,
        Failed
    };
    
    enum Error {
        NoError = 0,

        // network layer errors [relating to the destination server] (1-99):
        ConnectionRefusedError = 1,
        RemoteHostClosedError,
        HostNotFoundError,
        TimeoutError,
        OperationCanceledError,
        SslHandshakeFailedError,
        TemporaryNetworkFailureError,
        UnknownNetworkError = 99,

        // proxy errors (101-199):
        ProxyConnectionRefusedError = 101,
        ProxyConnectionClosedError,
        ProxyNotFoundError,
        ProxyTimeoutError,
        ProxyAuthenticationRequiredError,
        UnknownProxyError = 199,

        // content errors (201-299):
        ContentAccessDenied = 201,
        ContentOperationNotPermittedError,
        ContentNotFoundError,
        AuthenticationRequiredError,
        ContentReSendError,
        UnknownContentError = 299,

        // protocol errors
        ProtocolUnknownError = 301,
        ProtocolInvalidOperationError,
        ProtocolFailure = 399,
        
        // Json parser error
        ParseError = 401
    };
    
    explicit Request(QObject *parent = 0);
    ~Request();
    
    QString clientId() const;
    void setClientId(const QString &id);
    
    QString clientSecret() const;
    void setClientSecret(const QString &secret);
    
    QString accessToken() const;
    void setAccessToken(const QString &token);
    
    QString refreshToken() const;
    void setRefreshToken(const QString &token);
    
    QUrl url() const;
    
    QVariantMap headers() const;
    
    QVariant data() const;
    
    Operation operation() const;
    
    Status status() const;
    
    QVariant result() const;
    
    Error error() const;
    QString errorString() const;
    
    void setNetworkAccessManager(QNetworkAccessManager *manager);
    
public Q_SLOTS:
    void cancel();
    
protected:
    void setUrl(const QUrl &url);
    void setHeaders(const QVariantMap &headers);
    void setData(const QVariant &data);
    
protected Q_SLOTS:
    void head(bool authRequired = true);
    void get(bool authRequired = true);
    void post(bool authRequired = true);
    void put(bool authRequired = true);
    void deleteResource(bool authRequired = true);
    
Q_SIGNALS:
    void clientIdChanged();
    void clientSecretChanged();
    void accessTokenChanged(const QString &token);
    void refreshTokenChanged(const QString &token);
    void urlChanged();
    void dataChanged();
    void headersChanged();
    void operationChanged();
    void statusChanged(Status s);
    void finished();
    
protected:
    Request(RequestPrivate &dd, QObject *parent = 0);
    
    QScopedPointer<RequestPrivate> d_ptr;
    
    Q_DECLARE_PRIVATE(Request)
    
    Q_PRIVATE_SLOT(d_func(), void _q_onAccessTokenRefreshed())
    Q_PRIVATE_SLOT(d_func(), void _q_onReplyFinished())
    
private:
    Q_DISABLE_COPY(Request)
};

}

#endif // QSOUNDCLOUD_REQUEST_H
