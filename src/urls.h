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

#ifndef QSOUNDCLOUD_URLS_H
#define QSOUNDCLOUD_URLS_H

#include <QString>

namespace QSoundCloud {

// API
static const QString API_URL("https://api.soundcloud.com");

// Authentication
static const QString AUTH_URL("https://soundcloud.com/connect");
static const QString TOKEN_URL("https://api.soundcloud.com/oauth2/token");

static const QString GRANT_TYPE_CLIENT("client_credentials");
static const QString GRANT_TYPE_CODE("authorization_code");
static const QString GRANT_TYPE_OAUTH1("oauth1_token");
static const QString GRANT_TYPE_PASSWORD("password");
static const QString GRANT_TYPE_REFRESH("refresh_token");

static const QString NON_EXPIRING_SCOPE("non-expiring");
static const QString WILDCARD_SCOPE("*");

}

#endif // QSOUNDCLOUD_URLS_H
