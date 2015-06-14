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

#ifndef QSOUNDCLOUD_MODEL_P_H
#define QSOUNDCLOUD_MODEL_P_H

#include "model.h"

namespace QSoundCloud {

class ModelPrivate
{

public:
    ModelPrivate(Model *parent);
    virtual ~ModelPrivate();
    
    void setRoleNames(const QVariantMap &item);
        
    Model *q_ptr;
    
    QHash<int, QByteArray> roles;
    
    QList<QVariantMap> items;
    
    Q_DECLARE_PUBLIC(Model)
};

}

#endif // QSOUNDCLOUD_MODEL_P_H
