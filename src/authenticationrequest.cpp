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

#include "authenticationrequest.h"
#include "request_p.h"
#include "urls.h"
#include <QNetworkReply>
#include <QStringList>
#ifdef QSOUNDCLOUD_DEBUG
#include <QDebug>
#endif

namespace QSoundCloud {

class AuthenticationRequestPrivate : public RequestPrivate
{

public:
    AuthenticationRequestPrivate(AuthenticationRequest *parent) :
        RequestPrivate(parent)
    {
    }
    
    void _q_onReplyFinished() {
        if (!reply) {
            return;
        }
    
        Q_Q(AuthenticationRequest);
    
        bool ok;
        setResult(QtJson::Json::parse(reply->readAll(), ok));
        
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
        
    QString redirectUri;
    
    QStringList scopes;
    
    Q_DECLARE_PUBLIC(AuthenticationRequest)
};

/*!
    \class AuthenticationRequest
    \brief Handles OAuth 2.0 authentication requests.
    
    \ingroup requests
    
    The AuthenticationRequest class is used for obtaining and revoking access tokens for use with the SoundCloud Data 
    API.
     
    For more details on SoundCloud authentication, see 
    <a target="_blank" href="https://developers.soundcloud.com/docs/api/reference">here</a>.
*/
AuthenticationRequest::AuthenticationRequest(QObject *parent) :
    Request(*new AuthenticationRequestPrivate(this), parent)
{
}

/*!
    \property QString AuthenticationRequest::redirectUri
    \brief The uri that the web view will be redirected to during authentication.
*/
QString AuthenticationRequest::redirectUri() const {
    Q_D(const AuthenticationRequest);
    
    return d->redirectUri;
}

void AuthenticationRequest::setRedirectUri(const QString &uri) {
    Q_D(AuthenticationRequest);
    
    if (uri != d->redirectUri) {
        d->redirectUri = uri;
        emit redirectUriChanged();
    }
#if QSOUNDCLOUD_DEBUG
    qDebug() << "QSoundCloud::AuthenticationRequest::setRedirectUri" << uri;
#endif
}

/*!
    \property QStringList AuthenticationRequest::scopes
    \brief The list of scopes for which to request permission.
*/
QStringList AuthenticationRequest::scopes() const {
    Q_D(const AuthenticationRequest);
    
    return d->scopes;
}

void AuthenticationRequest::setScopes(const QStringList &scopes) {
    Q_D(AuthenticationRequest);
    
    d->scopes = scopes;
    emit scopesChanged();
#if QSOUNDCLOUD_DEBUG
    qDebug() << "AuthenticationRequest::setScopes" << scopes;
#endif
}

/*!
    \brief Submits \a code in exchange for a SoundCloud access token.
*/
void AuthenticationRequest::exchangeCodeForAccessToken(const QString &code) {
    setUrl(TOKEN_URL);
    setData(QString("client_id=" + clientId() + "&client_secret=" + clientSecret() + "&grant_type=" + GRANT_TYPE_CODE
                    + "&code=" + code + "&redirect_uri=" + redirectUri()));
    post(false);
}

}

#include "moc_authenticationrequest.cpp"
