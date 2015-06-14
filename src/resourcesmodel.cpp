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

#include "resourcesmodel.h"
#include "model_p.h"
#ifdef QSOUNDCLOUD_DEBUG
#include <QDebug>
#endif

namespace QSoundCloud {

class ResourcesModelPrivate : public ModelPrivate
{

public:
    ResourcesModelPrivate(ResourcesModel *parent) :
        ModelPrivate(parent),
        request(0),
        hasMore(false)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ResourcesModel);
    
        if (request->status() == ResourcesRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                hasMore = result.value("has_more").toBool();
            
                QVariantList list = result.value("list").toList();
            
                if (!list.isEmpty()) {
                    if (items.isEmpty()) {
                        setRoleNames(list.first().toMap());
                    }
                    
                    q->beginInsertRows(QModelIndex(), items.size(), items.size() + list.size() - 1);
                    
                    foreach (QVariant item, list) {
                        items << item.toMap();
                    }
                    
                    q->endInsertRows();
                    emit q->countChanged(q->rowCount());
                }
            }
        }
        
        ResourcesModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged(request->status());
    }
    
    void _q_onInsertRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ResourcesModel);
    
        if ((request->status() == ResourcesRequest::Ready) && (writeResourcePath == resourcePath)) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                if (items.isEmpty()) {
                    setRoleNames(result);
                }
                q->beginInsertRows(QModelIndex(), 0, 0);
                items.prepend(result);
                q->endInsertRows();
                emit q->countChanged(q->rowCount());
            }
        }
        
        ResourcesModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onInsertRequestFinished()));
    
        emit q->statusChanged(request->status());
    }
    
    void _q_onUpdateRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ResourcesModel);
    
        if (request->status() == ResourcesRequest::Ready) {
            QVariantMap result = request->result().toMap();
        
            if (!result.isEmpty()) {
                QVariant id = result.value("id");
                
                if (!id.isNull()) {
                    for (int i = 0; i < items.size(); i++) {
                        if (items.at(i).value("id") == id) {
                            q->set(i, result);
                            break;
                        }
                    }
                }
            }
        }
        
        ResourcesModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onUpdateRequestFinished()));
    
        emit q->statusChanged(request->status());
    }
    
    void _q_onDeleteRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(ResourcesModel);
    
        if ((request->status() == ResourcesRequest::Ready) &&
            ((writeResourcePath == resourcePath) || (writeResourcePath.isEmpty()))) {
            for (int i = 0; i < items.size(); i++) {
                if (items.at(i).value("id") == delId) {
                    q->beginRemoveRows(QModelIndex(), i, i);
                    items.removeAt(i);
                    q->endRemoveRows();
                    emit q->countChanged(q->rowCount());
                    break;
                }
            }
        }
        
        ResourcesModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onDeleteRequestFinished()));
    
        emit q->statusChanged(request->status());
    }
    
    ResourcesRequest *request;
    
    QString resourcePath;
    QVariantMap filters;
    QString writeResourcePath;
    QString delId;
        
    bool hasMore;
    
    Q_DECLARE_PUBLIC(ResourcesModel)
};

/*!
    \class ResourcesModel
    \brief A list model for displaying SoundCloud resources.
    
    \ingroup models
    
    The ResourcesModel is a list model used for displaying SoundCloud resources in a list view. ResourcesModel 
    provides the same methods that are available in ResourcesRequest, so it is better to simply use that class if you 
    do not need the additional features provided by a data model.
    
    Roles
    
    The roles and role names of ResourcesModel are created dynamically when the model is populated with data. The roles 
    are created by iterating through the keys of the first item in alphabetical order, starting at Qt::UserRole + 1.
    The role names are the keys themselves.
    
    Example usage:
    
    C++
    
    \code
    using namespace QSoundCloud;
    
    ...
    
    QListView *view = new QListView(this);
    ResourcesModel *model = new ResourcesModel(this);
    view->setModel(new ResourcesModel(this));
    
    QVariantMap filters;
    filters["limit"] = 10;
    filters["q"] = "Qt";
    model->get("/tracks", filters);
    \endcode
    
    QML
    
    \code
    import QtQuick 1.0
    import QSoundCloud 1.0
    
    ListView {
        id: view
        
        width: 800
        height: 480
        model: ResourcesModel {
            id: resourcesModel
        }
        delegate: Text {
            width: view.width
            height: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            text: title
        }
        
        Component.onCompleted: resourcesModel.get("/tracks", {limit: 10, search: "Qt"})
    }
    \endcode
    
    \sa ResourcesRequest
*/

ResourcesModel::ResourcesModel(QObject *parent) :
    Model(*new ResourcesModelPrivate(this), parent)
{
    Q_D(ResourcesModel);

    d->request = new ResourcesRequest(this);
    connect(d->request, SIGNAL(clientIdChanged()), this, SIGNAL(clientIdChanged()));
    connect(d->request, SIGNAL(clientSecretChanged()), this, SIGNAL(clientSecretChanged()));
    connect(d->request, SIGNAL(accessTokenChanged(QString)), this, SIGNAL(accessTokenChanged(QString)));
    connect(d->request, SIGNAL(refreshTokenChanged(QString)), this, SIGNAL(refreshTokenChanged(QString)));
}

/*!
    \property QString ResourcesModel::clientId
    \brief The client id to be used when making requests to the SoundCloud Data API.
    
    The client id is used only when the access token needs to be refreshed.
    
    \sa ResourcesRequest::clientId
*/

/*!
    \fn void ResourcesModel::clientIdChanged()
    \brief Emitted when the clientId changes.
*/
QString ResourcesModel::clientId() const {
    Q_D(const ResourcesModel);
    
    return d->request->clientId();
}

void ResourcesModel::setClientId(const QString &id) {
    Q_D(ResourcesModel);
    
    d->request->setClientId(id);
}

/*!
    \property QString ResourcesModel::clientSecret
    \brief The client secret to be used when making requests to the SoundCloud Data API.
    
    The client secret is used only when the access token needs to be refreshed.
    
    \sa ResourcesRequest::clientSecret
*/

/*!
    \fn void ResourcesModel::clientSecretChanged()
    \brief Emitted when the clientSecret changes.
*/
QString ResourcesModel::clientSecret() const {
    Q_D(const ResourcesModel);
    
    return d->request->clientSecret();
}

void ResourcesModel::setClientSecret(const QString &secret) {
    Q_D(ResourcesModel);
    
    d->request->setClientSecret(secret);
}

/*!
    \property QString ResourcesModel::accessToken
    \brief The access token to be used when making requests to the SoundCloud Data API.
    
    The access token is required when accessing a resource's protected resources.
    
    \as ResourcesRequest::accessToken
*/

/*!
    \fn void ResourcesModel::accessTokenChanged()
    \brief Emitted when the accessToken changes.
*/
QString ResourcesModel::accessToken() const {
    Q_D(const ResourcesModel);
    
    return d->request->accessToken();
}

void ResourcesModel::setAccessToken(const QString &token) {
    Q_D(ResourcesModel);
    
    d->request->setAccessToken(token);
}

/*!
    \property QString ResourcesModel::refreshToken
    \brief The refresh token to be used when making requests to the SoundCloud Data API.
    
    The refresh token is used only when the accessToken needs to be refreshed.
    
    \sa ResourcesRequest::refreshToken
*/

/*!
    \fn void ResourcesModel::refreshTokenChanged()
    \brief Emitted when the refreshToken changes.
*/
QString ResourcesModel::refreshToken() const {
    Q_D(const ResourcesModel);
    
    return d->request->refreshToken();
}

void ResourcesModel::setRefreshToken(const QString &token) {
    Q_D(ResourcesModel);
    
    d->request->setRefreshToken(token);
}

/*!
    \property enum ResourcesModel::status
    \brief The current status of the model.
    
    \sa ResourcesRequest::status
*/

/*!
    \fn void ResourcesModel::statusChanged()
    \brief Emitted when the status changes.
*/
ResourcesRequest::Status ResourcesModel::status() const {
    Q_D(const ResourcesModel);
    
    return d->request->status();
}

/*!
    \property QVariant ResourcesModel::result
    \brief The current result of the model.
    
    \sa ResourcesRequest::result
*/
QVariant ResourcesModel::result() const {
    Q_D(const ResourcesModel);
    
    return d->request->result();
}

/*!
    \property enum ResourcesModel::error
    \brief The error type of the model.
    
    \sa ResourcesRequest::error
*/
ResourcesRequest::Error ResourcesModel::error() const {
    Q_D(const ResourcesModel);
    
    return d->request->error();
}

/*!
    \property enum ResourcesModel::errorString
    \brief A description of the error of the model.
    
    \sa ResourcesRequest::status
*/
QString ResourcesModel::errorString() const {
    Q_D(const ResourcesModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests to the SoundCloud Data API.
    
    ResourcesModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa ResourcesRequest::setNetworkAccessManager()
*/
void ResourcesModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(ResourcesModel);
    
    d->request->setNetworkAccessManager(manager);
}

bool ResourcesModel::canFetchMore(const QModelIndex &) const {
    if (status() == ResourcesRequest::Loading) {
        return false;
    }
    
    Q_D(const ResourcesModel);
    
    return d->hasMore;
}

void ResourcesModel::fetchMore(const QModelIndex &) {
    if (canFetchMore()) {
        Q_D(ResourcesModel);
        
        int page = d->filters.value("page").toInt();
        d->filters["page"] = (page > 0 ? page + 1 : 2);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->get(d->resourcePath, d->filters);
        emit statusChanged(d->request->status());
    }
}

/*!
    \brief Retrieves a list of SoundCloud resources belonging to \a resourcePath.
        
    \sa ResourcesRequest::get()
*/
void ResourcesModel::get(const QString &resourcePath, const QVariantMap &filters) {
    if (status() != ResourcesRequest::Loading) {
        Q_D(ResourcesModel);
        clear();
        d->resourcePath = resourcePath;
        d->filters = filters;
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->get(d->resourcePath, d->filters);
        emit statusChanged(d->request->status());
    }
}

/*!
    \brief Inserts a new SoundCloud resource into the current resourcePath.
    
    \sa ResourcesRequest::insert()
*/
void ResourcesModel::insert(const QVariantMap &resource) {
    if (status() != ResourcesRequest::Loading) {
        Q_D(ResourcesModel);
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onInsertRequestFinished()));
        d->request->insert(resource, d->resourcePath);
        emit statusChanged(d->request->status());
    }
}

/*!
    \brief Inserts the SoundCloud resource at \a row into \a resourcePath.
    
    \sa ResourcesRequest::insert()
*/
void ResourcesModel::insert(int row, const QString &resourcePath) {
    if (status() != ResourcesRequest::Loading) {
        Q_D(ResourcesModel);
        d->writeResourcePath = resourcePath;
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onInsertRequestFinished()));
        d->request->insert(QString("%1%2%3").arg(resourcePath)
                                            .arg(resourcePath.endsWith("/") ? QString() : QString("/"))
                                            .arg(Model::get(row).value("id").toString()));
        emit statusChanged(d->request->status());
    }
}

/*!
    \brief Updates the SoundCloud resource at \a row with \a resource.
*/
void ResourcesModel::update(int row, const QVariantMap &resource) {
    if (status() != ResourcesRequest::Loading) {
        Q_D(ResourcesModel);
        d->writeResourcePath = d->resourcePath;
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onUpdateRequestFinished()));
        d->request->update(QString("%1%2%3").arg(d->resourcePath)
                                            .arg(d->resourcePath.endsWith("/") ? QString() : QString("/"))
                                            .arg(Model::get(row).value("id").toString()), resource);
        emit statusChanged(d->request->status());
    }
}

/*!
    \brief Deletes the SoundCloud resource at \a row from the current resourcePath.
*/
void ResourcesModel::del(int row) {
    if (status() != ResourcesRequest::Loading) {
        Q_D(ResourcesModel);
        d->delId = Model::get(row).value("id").toString();
        d->writeResourcePath = d->resourcePath;
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onDeleteRequestFinished()));
        d->request->del(QString("%1%2%3").arg(d->resourcePath).arg(d->resourcePath.endsWith("/") ? QString() : QString("/"))
                                         .arg(d->delId));
        emit statusChanged(d->request->status());
    }
}

/*!
    \brief Deletes the SoundCloud resource at \a row from \a resourcePath.
*/
void ResourcesModel::del(int row, const QString &resourcePath) {
    if (status() != ResourcesRequest::Loading) {
        Q_D(ResourcesModel);
        d->delId = Model::get(row).value("id").toString();
        d->writeResourcePath = resourcePath;
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onDeleteRequestFinished()));
        d->request->del(QString("%1%2%3").arg(resourcePath).arg(resourcePath.endsWith("/") ? QString() : QString("/"))
                                         .arg(d->delId));
        emit statusChanged(d->request->status());
    }
}

/*!
    \brief Cancels the current request.
    
    \sa ResourcesRequest::cancel()
*/
void ResourcesModel::cancel() {
    Q_D(ResourcesModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of SoundCloud resources using the existing parameters.
*/
void ResourcesModel::reload() {
    if (status() != ResourcesRequest::Loading) {
        Q_D(ResourcesModel);
        clear();
        
        if (!d->filters.value("page").isNull()) {
            d->filters["page"] = 1;
        }
        
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->get(d->resourcePath, d->filters);
        emit statusChanged(d->request->status());
    }
}

}

#include "moc_resourcesmodel.cpp"
