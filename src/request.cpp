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

#include "request_p.h"
#include "urls.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>

namespace QSoundCloud {

/*!
    \class Request
    \brief The base class for making requests to the SoundCloud Data API.
    
    \ingroup requests
    
    Normally, there should be no need to use this class, but it can be useful if you need to extend the range of API 
    requests beyond those provided in the existing subclasses.   
*/
Request::Request(QObject *parent) :
    QObject(parent),
    d_ptr(new RequestPrivate(this))
{
}

Request::Request(RequestPrivate &dd, QObject *parent) :
    QObject(parent),
    d_ptr(&dd)
{
}

Request::~Request() {
    Q_D(Request);
    
    if (d->reply) {
        delete d->reply;
        d->reply = 0;
    }
}

/*!
    \property QString Request::clientId
    \brief The client id used when making requests to the SoundCloud Data API.
*/

/*!
    \fn void Request::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString Request::clientId() const {
    Q_D(const Request);
    
    return d->clientId;
}

void Request::setClientId(const QString &id) {
    Q_D(Request);
    
    if (id != d->clientId) {
        d->clientId = id;
        emit clientIdChanged();
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::setClientId" << id;
#endif
}

/*!
    \property QString Request::clientSecret
    \brief The api key used when making requests to the SoundCloud Data API.
*/

/*!
    \fn void Request::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString Request::clientSecret() const {
    Q_D(const Request);
    
    return d->clientSecret;
}

void Request::setClientSecret(const QString &secret) {
    Q_D(Request);
    
    if (secret != d->clientSecret) {
        d->clientSecret = secret;
        emit clientSecretChanged();
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::setClientSecret" << secret;
#endif
}

/*!
    \property QString Request::accessToken
    \brief The access token used when making requests to the SoundCloud Data API.
*/

/*!
    \fn void Request::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString Request::accessToken() const {
    Q_D(const Request);
    
    return d->accessToken;
}

void Request::setAccessToken(const QString &token) {
    Q_D(Request);
    
    if (token != d->accessToken) {
        d->accessToken = token;
        emit accessTokenChanged(token);
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::setAccessToken" << token;
#endif
}

/*!
    \property QString Request::refreshToken
    \brief The refresh token used when making requests to the SoundCloud Data API.
*/

/*!
    \fn void Request::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString Request::refreshToken() const {
    Q_D(const Request);
    
    return d->refreshToken;
}

void Request::setRefreshToken(const QString &token) {
    Q_D(Request);
    
    if (token != d->refreshToken) {
        d->refreshToken = token;
        emit refreshTokenChanged(token);
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::setRefreshToken" << token;
#endif
}

/*!
    \property QUrl Request::url
    \brief The url used when making requests to the SoundCloud Data API.
*/

/*!
    \fn void Request::urlChanged()
    \brief Emitted when the url changes.
*/
QUrl Request::url() const {
    Q_D(const Request);
    
    return d->url;
}

void Request::setUrl(const QUrl &url) {
    Q_D(Request);
    
    if (url != d->url) {
        d->url = url;
        emit urlChanged();
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::setUrl" << url;
#endif
}

/*!
    \property QVariantMap Request::headers
    \brief The headers used when making requests to the SoundCloud Data API.
*/

/*!
    \fn void Request::headersChanged()
    \brief Emitted when the headers change.
*/
QVariantMap Request::headers() const {
    Q_D(const Request);
    
    return d->headers;
}

void Request::setHeaders(const QVariantMap &headers) {
    Q_D(Request);
    
    d->headers = headers;
    emit headersChanged();
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::setHeaders" << headers;
#endif
}

/*!
    \property QVariant Request::data
    \brief The data used when making HTTP PUT/POST requests to the SoundCloud Data API.
*/

/*!
    \fn void Request::dataChanged()
    \brief Emitted when the data changes.
*/
QVariant Request::data() const {
    Q_D(const Request);
    
    return d->data;
}

void Request::setData(const QVariant &data) {
    Q_D(Request);
    
    if (data != d->data) {
        d->data = data;
        emit dataChanged();
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::setData" << data;
#endif
}

/*!
    \enum Request::Operation
    \brief The operation type of the last HTTP request.
    
    Can be one of the following:
    
    <table>
        <tr>
            <th>Value</th>
            <th>Description</th>
        </tr>
        <tr>
            <td>HeadOperation</td>
            <td>Retrieve headers(created with head()).</td>
        </tr>
        <tr>
            <td>GetOperation</td>
            <td>Retrieve headers and download contents (created with get()).</td>
        </tr>
        <tr>
            <td>PutOperation</td>
            <td>Upload contents operation (created with put()).
        </tr>
        <tr>
            <td>PostOperation</td>
            <td>Send the contents of an HTML form for processing via HTTP POST (created with post()).
        </tr>
        <tr>
            <td>DeleteOperation</td>
            <td>Send the contents of an HTML form for processing via HTTP POST (created with post()).
        </tr>
    </table>
*/

/*!
    \property Operation Request::operation
    \brief The last HTTP operation type.
*/

/*!
    \fn void Request::operationChanged()
    \brief Emitted when the operation changes.
*/
Request::Operation Request::operation() const {
    Q_D(const Request);
    
    return d->operation;
}

/*!
    \enum Request::Status
    \brief The status of the last HTTP request.
    
    Can be one of the following:
    
    <table>
        <tr>
            <th>Value</th>
            <th>Description</th>
        </tr>
        <tr>
            <td>Null</td>
            <td>No request has been made.</td>
        </tr>
        <tr>
            <td>Loading</td>
            <td>A request is in progress.</td>
        </tr>
        <tr>
            <td>Canceled</td>
            <td>The last request was canceled.</td>
        </tr>
        <tr>
            <td>Failed</td>
            <td>The last request failed.</td>
        </tr>
        <tr>
            <td>Ready</td>
            <td>The last request was successful.</td>
        </tr>
    </table>
*/

/*!
    \property Status Request::status
    \brief The status of the last request.
*/

/*!
    \fn void Request::statusChanged()
    \brief Emitted when the status changes.
*/
Request::Status Request::status() const {
    Q_D(const Request);
    
    return d->status;
}

/*!
    \fn void Request::finished()
    \brief Emitted when the request is completed.
    
    \sa status, result
*/

/*!
    \property QVariant Request::result
    \brief The result of the last HTTP request.
*/
QVariant Request::result() const {
    Q_D(const Request);
    
    return d->result;
}

/*!
    \enum Request::Error
    \brief The error resulting from the last HTTP request.
    
    Can be one of the folowing:
    
    <table>
        <tr>
            <th>Value</th>
            <th>Description</th>
        </tr>
        <tr>
            <td>NoError</td>
            <td>No error condition.</td>
        </tr>
        <tr>
            <td>ConnectionRefusedError</td>
            <td>The remote server refused the connection (the server is not accepting requests).</td>
        </tr>
        <tr>
            <td>HostNotFoundError</td>
            <td>The remote host name was not found (invalid hostname).</td>
        </tr>
        <tr>
            <td>TimeoutError</td>
            <td>The connection to the remote server timed out.</td>
        </tr>
        <tr>
            <td>SslHandshakeFailedError</td>
            <td>The SSL/TLS handshake failed and the encrypted channel could not be established.</td>
        </tr>
        <tr>
            <td>TemporaryNetworkFailureError</td>
            <td>
                The connection was broken due to disconnection from the network, however the system has initiated
                roaming to another access point.
            </td>
        </tr>
        <tr>
            <td>ProxyConnectionRefusedError</td>
            <td>
                The proxy server closed the connection prematurely, before the entire reply was received and
                processed.
            </td>
        </tr>
        <tr>
            <td>ProxyConnectionClosedError</td>
            <td>
                The proxy server closed the connection prematurely, before the entire reply was received and
                processed.
            </td>
        </tr>
        <tr>
            <td>ProxyNotFoundError</td>
            <td>The proxy host name was not found (invalid proxy hostname).</td>
        </tr>
        <tr>
            <td>ProxyTimeoutError</td>
            <td>The connection to the proxy timed out or the proxy did not reply in time to the request sent.</td>
        </tr>
        <tr>
            <td>ProxyAuthenticationRequiredError</td>
            <td>
                The proxy requires authentication in order to honour the request but did not accept any credentials 
                offered (if any).
            </td>
        </tr>
        <tr>
            <td>ContentAccessDenied</td>
            <td>The access to the remote content was denied (similar to HTTP error 401).</td>
        </tr>
        <tr>
            <td>ContentOperationNotPermittedError</td>
            <td>The operation requested on the remote content is not permitted.</td>
        </tr>
        <tr>
            <td>ContentNotFoundError</td>
            <td>The remote content was not found at the server (similar to HTTP error 404).</td>
        </tr>
        <tr>
            <td>AuthenticationRequiredError</td>
            <td>
                The remote server requires authentication to serve the content but the credentials provided
                were not accepted (if any).
            </td>
        </tr>
        <tr>
            <td>ContentReSendError</td>
            <td>
                The request needed to be sent again, but this failed for example because the upload data
                could not be read a second time.
            </td>
        </tr>
        <tr>
            <td>ProtocolUnknownError</td>
            <td>The Network Access API cannot honor the request because the protocol is not known.</td>
        </tr>
        <tr>
            <td>ProtocolInvalidOperationError</td>
            <td>The requested operation is invalid for this protocol.</td>
        </tr>
        <tr>
            <td>UnknownNetworkError</td>
            <td>An unknown network-related error was detected.</td>
        </tr>
        <tr>
            <td>UnknownProxyError</td>
            <td>An unknown proxy-related error was detected.</td>
        </tr>
        <tr>
            <td>UnknownContentError</td>
            <td>An unknown error related to the remote content was detected.</td>
        </tr>
        <tr>
            <td>ProtocolFailure</td>
            <td>A breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.).</td>
        </tr>
        <tr>
            <td>ParseError</td>
            <td>There was an error in parsing the server response.</td>
        </tr>
    </table>
*/

/*!
    \property Error Request::error
    \brief The error resulting from the last HTTP request.
*/
Request::Error Request::error() const {
    Q_D(const Request);
    
    return d->error;
}

/*!
    \property QString Request::errorString
    \brief A description of the error resulting from the last HTTP request.
*/
QString Request::errorString() const {
    Q_D(const Request);
    
    return d->errorString;
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used 
    when making requests to the SoundCloud API.
    
    Request does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created 
    when required.    
*/
void Request::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(Request);
    
    if ((d->manager) && (d->ownNetworkAccessManager)) {
        delete d->manager;
    }
    
    d->ownNetworkAccessManager = false;
    d->manager = manager;
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::setNetworkAccessManager" << manager;
#endif
}

/*!
    \brief Performs a HTTP HEAD request.
*/
void Request::head(bool authRequired) {
    Q_D(Request);
    
    if (d->url.isEmpty()) {
        qDebug() << "QSoundCloud::Request::head(): URL is empty";
        return;
    }
    
    d->redirects = 0;
    d->setOperation(HeadOperation);
    d->setStatus(Loading);
    
    if (d->reply) {
        delete d->reply;
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::head" << d->url;
#endif
    d->reply = d->networkAccessManager()->head(d->buildRequest(authRequired));
    connect(d->reply, SIGNAL(finished()), this, SLOT(_q_onReplyFinished()));
}

/*!
    \brief Performs a HTTP GET request.
*/
void Request::get(bool authRequired) {
    Q_D(Request);
    
    if (d->url.isEmpty()) {
        qDebug() << "QSoundCloud::Request::get(): URL is empty";
        return;
    }
    
    d->redirects = 0;
    d->setOperation(GetOperation);
    d->setStatus(Loading);
    
    if (d->reply) {
        delete d->reply;
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::get" << d->url;
#endif
    d->reply = d->networkAccessManager()->get(d->buildRequest(authRequired));
    connect(d->reply, SIGNAL(finished()), this, SLOT(_q_onReplyFinished()));
}

/*!
    \brief Performs a HTTP POST request.
*/
void Request::post(bool authRequired) {
    Q_D(Request);
    
    if (d->url.isEmpty()) {
        qDebug() << "QSoundCloud::Request::post(): URL is empty";
        return;
    }
    
    d->redirects = 0;
    d->setOperation(PostOperation);
    
    bool ok = true;
    QByteArray data;
    
    switch (d->data.type()) {
    case QVariant::String:
    case QVariant::ByteArray:
    case QVariant::Invalid:
        data = d->data.toString().toUtf8();
        break;
    default:
        data = QtJson::Json::serialize(d->data, ok);
        break;
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::post" << d->url << data;
#endif
    if (ok) {
        if (d->reply) {
            delete d->reply;
        }
        
        d->setStatus(Loading);        
        d->reply = d->networkAccessManager()->post(d->buildRequest(authRequired), data);
        connect(d->reply, SIGNAL(finished()), this, SLOT(_q_onReplyFinished()));
    }
    else {
        d->setStatus(Failed);
        d->setError(ParseError);
        d->setErrorString(tr("Unable to serialize the POST data"));
        emit finished();
    }    
}

/*!
    \brief Performs a HTTP PUT request.
*/
void Request::put(bool authRequired) {
    Q_D(Request);
    
    if (d->url.isEmpty()) {
        qDebug() << "QSoundCloud::Request::put(): URL is empty";
        return;
    }
    
    d->redirects = 0;
    d->setOperation(PutOperation);
        
    bool ok = true;
    QByteArray data;
    
    switch (d->data.type()) {
    case QVariant::String:
    case QVariant::ByteArray:
    case QVariant::Invalid:
        data = d->data.toString().toUtf8();
        break;
    default:
        data = QtJson::Json::serialize(d->data, ok);
        break;
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::put" << d->url << data;
#endif
    if (ok) {
        if (d->reply) {
            delete d->reply;
        }
        
        d->setStatus(Loading);        
        d->reply = d->networkAccessManager()->put(d->buildRequest(authRequired), data);
        connect(d->reply, SIGNAL(finished()), this, SLOT(_q_onReplyFinished()));
    }
    else {
        d->setStatus(Failed);
        d->setError(ParseError);
        d->setErrorString(tr("Unable to serialize the PUT data"));
        emit finished();
    }    
}

/*!
    \brief Performs a HTTP DELETE request.
*/
void Request::deleteResource(bool authRequired) {
    Q_D(Request);
    
    if (d->url.isEmpty()) {
        qDebug() << "QSoundCloud::Request::deleteResource(): URL is empty";
        return;
    }
    
    d->redirects = 0;
    d->setOperation(DeleteOperation);
    d->setStatus(Loading);
    
    if (d->reply) {
        delete d->reply;
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::Request::deleteResource" << d->url;
#endif
    d->reply = d->networkAccessManager()->deleteResource(d->buildRequest(authRequired));
    connect(d->reply, SIGNAL(finished()), this, SLOT(_q_onReplyFinished()));
}

/*!
    \brief Cancels the current HTTP request.
*/
void Request::cancel() {
    Q_D(Request);
    
    if (d->reply) {
        d->reply->abort();
    }
}

RequestPrivate::RequestPrivate(Request *parent) :
    q_ptr(parent),
    manager(0),
    reply(0),
    ownNetworkAccessManager(false),
    operation(Request::UnknownOperation),
    status(Request::Null),
    error(Request::NoError),
    redirects(0)
{
}

RequestPrivate::~RequestPrivate() {}

QNetworkAccessManager* RequestPrivate::networkAccessManager() {    
    if (!manager) {
        Q_Q(Request);
        ownNetworkAccessManager = true;
        manager = new QNetworkAccessManager(q);
    }
    
    return manager;
}

void RequestPrivate::setOperation(Request::Operation op) {
    if (op != operation) {
        Q_Q(Request);
        operation = op;
        emit q->operationChanged();
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::RequestPrivate::setOperation " << Request::Operation(op);
#endif
}

void RequestPrivate::setStatus(Request::Status s) {
    if (s != status) {
        Q_Q(Request);
        status = s;
        emit q->statusChanged(s);
    }
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::RequestPrivate::setStatus " << Request::Status(s);
#endif
}

void RequestPrivate::setError(Request::Error e) {
    error = e;
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::RequestPrivate::setError " << Request::Error(e);
#endif
}

void RequestPrivate::setErrorString(const QString &es) {
    errorString = es;
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::RequestPrivate::setErrorString " << es;
#endif
}

void RequestPrivate::setResult(const QVariant &res) {
    result = res;
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::RequestPrivate::setResult " << res;
#endif
}

QNetworkRequest RequestPrivate::buildRequest(bool authRequired) {
    return buildRequest(url, authRequired);
}

QNetworkRequest RequestPrivate::buildRequest(QUrl u, bool authRequired) {
#ifdef QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::RequestPrivate::buildRequest " << u;
#endif
    if (authRequired) {
#if QT_VERSION >= 0x050000
        QUrlQuery query(u);
        
        if (!query.hasQueryItem("client_id")) {
            if (!accessToken.isEmpty()) {
                query.addQueryItem("oauth_token", accessToken);
            }
            else {
                query.addQueryItem("client_id", clientId);
            }
            
            u.setQuery(query);
        }
#else   
        if (!u.hasQueryItem("client_id")) {
            if (!accessToken.isEmpty()) {
                u.addQueryItem("oauth_token", accessToken);
            }
            else {
                u.addQueryItem("client_id", clientId);
            }
        }
#endif
    }
    
    QNetworkRequest request(u);
    
    switch (operation) {
    case Request::PostOperation:
    case Request::PutOperation:
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        break;
    default:
        break;
    }
    
    if (!headers.isEmpty()) {
        addRequestHeaders(&request, headers);
    }
    
    return request;
}

void RequestPrivate::followRedirect(const QUrl &redirect) {
    Q_Q(Request);
    
    redirects++;
    
    if (reply) {
        delete reply;
    }
        
    reply = networkAccessManager()->get(buildRequest(redirect));
    Request::connect(reply, SIGNAL(finished()), q, SLOT(_q_onReplyFinished()));
}

void RequestPrivate::refreshAccessToken() {
    Q_Q(Request);
    
    QUrl tokenUrl(TOKEN_URL);
    QNetworkRequest request(tokenUrl);
    QString body("client_id=" + clientId + "&client_secret=" + clientSecret + "&refresh_token=" + refreshToken +
                 "&grant_type=" + GRANT_TYPE_REFRESH);
                    
    if (reply) {
        delete reply;
    }
    
    reply = networkAccessManager()->post(request, body.toUtf8());
    Request::connect(reply, SIGNAL(finished()), q, SLOT(_q_onAccessTokenRefreshed()));
}

void RequestPrivate::_q_onAccessTokenRefreshed() {
    if (!reply) {
        return;
    }
    
    Q_Q(Request);
        
    bool ok;
    setResult(QtJson::Json::parse(reply->readAll(), ok));
    
    const QNetworkReply::NetworkError e = reply->error();
    const QString es = reply->errorString();
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
    default:
        setStatus(Request::Failed);
        setError(Request::Error(e));
        setErrorString(es);
        emit q->finished();
        return;
    }
        
    if (ok) {
        QString token = result.toMap().value("access_token").toString();
        
        if (token.isEmpty()) {
            setStatus(Request::Failed);
            setError(Request::ContentAccessDenied);
            setErrorString(Request::tr("Unable to refresh access token"));
            emit q->finished();
        }
        else {
            q->setAccessToken(token);
            
            switch (operation) {
            case Request::GetOperation:
                q->get();
                break;
            case Request::PostOperation:
                q->post();
                break;
            case Request::PutOperation:
                q->put();
                break;
            case Request::DeleteOperation:
                q->deleteResource();
                break;
            default:
                break;
            }
        }
    }
    else {
        setStatus(Request::Failed);
        setError(Request::ParseError);
        setErrorString(Request::tr("Unable to parse response"));
        emit q->finished();
    }
}

void RequestPrivate::_q_onReplyFinished() {
    if (!reply) {
        return;
    }
    
    Q_Q(Request);
    
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
    
    bool ok = true;
    const QString response = QString::fromUtf8(reply->readAll());
    setResult(response.isEmpty() ? response : QtJson::Json::parse(response, ok));
    
    const QNetworkReply::NetworkError e = reply->error();
    const QString es = reply->errorString();
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
    
    if (ok) {
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

}

#include "moc_request.cpp"
