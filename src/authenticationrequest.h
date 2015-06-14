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
 
#ifndef QSOUNDCLOUD_AUTHENTICATIONREQUEST_H
#define QSOUNDCLOUD_AUTHENTICATIONREQUEST_H

#include "request.h"

namespace QSoundCloud {

class AuthenticationRequestPrivate;

class QSOUNDCLOUDSHARED_EXPORT AuthenticationRequest : public Request
{
    Q_OBJECT
    
    Q_PROPERTY(QString redirectUri READ redirectUri WRITE setRedirectUri NOTIFY redirectUriChanged)
    Q_PROPERTY(QStringList scopes READ scopes WRITE setScopes NOTIFY scopesChanged)
    
public:
    explicit AuthenticationRequest(QObject *parent = 0);
    
    QString redirectUri() const;
    void setRedirectUri(const QString &uri);
    
    QStringList scopes() const;
    void setScopes(const QStringList &s);
    
public Q_SLOTS:
    void exchangeCodeForAccessToken(const QString &code);
    
Q_SIGNALS:
    void redirectUriChanged();
    void scopesChanged();

private:
    Q_DECLARE_PRIVATE(AuthenticationRequest)
    Q_DISABLE_COPY(AuthenticationRequest)
};

}

#endif // QSOUNDCLOUD_AUTHENTICATIONREQUEST_H
