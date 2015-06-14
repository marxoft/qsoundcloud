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

#ifndef QSOUNDCLOUD_STREAMSREQUEST_H
#define QSOUNDCLOUD_STREAMSREQUEST_H

#include "request.h"

namespace QSoundCloud {

class StreamsRequestPrivate;

class QSOUNDCLOUDSHARED_EXPORT StreamsRequest : public Request
{
    Q_OBJECT
    
public:
    explicit StreamsRequest(QObject *parent = 0);

public Q_SLOTS:
    void get(const QString &id);
    
private:    
    Q_DECLARE_PRIVATE(StreamsRequest)
    Q_DISABLE_COPY(StreamsRequest)
    
    Q_PRIVATE_SLOT(d_func(), void _q_onDownloadRedirect())
    Q_PRIVATE_SLOT(d_func(), void _q_onStreamRedirect())
};

}

#endif // QSOUNDCLOUD_STREAMSREQUEST_H
