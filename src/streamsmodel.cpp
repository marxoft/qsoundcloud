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

#include "streamsmodel.h"
#include "model_p.h"

namespace QSoundCloud {

class StreamsModelPrivate : public ModelPrivate
{

public:
    StreamsModelPrivate(StreamsModel *parent) :
        ModelPrivate(parent),
        request(0)
    {
    }
        
    void _q_onListRequestFinished() {
        if (!request) {
            return;
        }
    
        Q_Q(StreamsModel);
    
        if (request->status() == StreamsRequest::Ready) {
            QVariantList list = request->result().toList();
        
            if (!list.isEmpty()) {
                q->beginInsertRows(QModelIndex(), items.size(), items.size() + list.size());
                
                foreach (QVariant item, list) {
                    items << item.toMap();
                }
                
                q->endInsertRows();
                emit q->countChanged(q->rowCount());
            }
        }
        
        StreamsModel::disconnect(request, SIGNAL(finished()), q, SLOT(_q_onListRequestFinished()));
    
        emit q->statusChanged(request->status());
    }
    
    StreamsRequest *request;
    
    QString id;
        
    Q_DECLARE_PUBLIC(StreamsModel)
};

/*!
    \class StreamsModel
    \brief A list model for displaying list of streams for a SoundCloud track.
    
    \ingroup models
    
    The StreamsModel is a list model used for displaying SoundCloud track streams in a list view. 
    StreamsModel provides the same methods that are available in StreamsRequest, so it is better to 
    simply use that class if you do not need the additional features provided by a data model.
    
    StreamsModel provides the following roles and role names:
    
    <table>
        <tr>
        <th>Role</th>
        <th>Role name</th>
        </tr>
        <tr>
            <td>IdRole</td>
            <td>id</td>
        </tr>
        <tr>
            <td>DescriptionRole</td>
            <td>description</td>
        </tr>
        <tr>
            <td>ExtensionRole</td>
            <td>ext</td>
        </tr>
        <tr>
            <td>UrlRole</td>
            <td>url</td>
        </tr>
    </table>
        
    Example usage:
    
    C++
    
    \code
    using namespace QSoundCloud;
    
    ...
    
    QListView *view = new QListView(this);
    StreamsModel *model = new StreamsModel(this);
    view->setModel(model);
    model->get(TRACK_ID);
    \endcode
    
    QML
    
    \code
    import QtQuick 1.0
    import QSoundCloud 1.0
    
    ListView {
        id: view
        
        width: 800
        height: 480
        model: StreamsModel {
            id: streamsModel
        }
        delegate: Text {
            width: view.width
            height: 50
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            text: description
        }
        
        Component.onCompleted: streamsModel.list(TRACK_ID)
    }
    \endcode
    
    \sa StreamsRequest
*/

/*!
    \enum StreamsModel::Roles
    \brief The data roles available for accessing the model data.
    
    See the detailed description for the list of available roles.
*/
StreamsModel::StreamsModel(QObject *parent) :
    Model(*new StreamsModelPrivate(this), parent)
{
    Q_D(StreamsModel);
    d->roles[IdRole] = "id";
    d->roles[DescriptionRole] = "description";
    d->roles[ExtensionRole] = "ext";
    d->roles[WidthRole] = "width";
    d->roles[HeightRole] = "height";
    d->roles[UrlRole] = "url";
#if QT_VERSION < 0x050000
    setRoleNames(d->roles);
#endif
    d->request = new StreamsRequest(this);
}

/*!
    \property enum StreamsModel::status
    \brief The current status of the model.
    
    \sa StreamsRequest::status
*/

/*!
    \fn void StreamsModel::statusChanged()
    \brief Emitted when the status changes.
*/
StreamsRequest::Status StreamsModel::status() const {
    Q_D(const StreamsModel);
    
    return d->request->status();
}

/*!
    \property QVariant StreamsModel::result
    \brief The current result of the model.
    
    \sa StreamsRequest::result
*/
QVariant StreamsModel::result() const {
    Q_D(const StreamsModel);
    
    return d->request->result();
}

/*!
    \property enum StreamsModel::error
    \brief The error type of the model.
    
    \sa StreamsRequest::error
*/
StreamsRequest::Error StreamsModel::error() const {
    Q_D(const StreamsModel);
    
    return d->request->error();
}

/*!
    \property enum StreamsModel::errorString
    \brief A description of the error of the model.
    
    \sa StreamsRequest::status
*/
QString StreamsModel::errorString() const {
    Q_D(const StreamsModel);
    
    return d->request->errorString();
}

/*!
    \brief Sets the QNetworkAccessManager instance to be used when making requests.
    
    StreamsModel does not take ownership of \a manager.
    
    If no QNetworkAccessManager is set, one will be created when required.
    
    \sa StreamsRequest::setNetworkAccessManager()
*/
void StreamsModel::setNetworkAccessManager(QNetworkAccessManager *manager) {
    Q_D(StreamsModel);
    
    d->request->setNetworkAccessManager(manager);
}

/*!
    \brief Retrieves a list of streams for a SoundCloud track.
    
    \sa StreamsRequest::get()
*/
void StreamsModel::get(const QString &id) {
    if (status() != StreamsRequest::Loading) {
        Q_D(StreamsModel);
        d->id = id;
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->get(id);
        emit statusChanged(d->request->status());
    }
}

/*!
    \brief Cancels the current request.
    
    \sa StreamsRequest::cancel()
*/
void StreamsModel::cancel() {
    Q_D(StreamsModel);
    
    if (d->request) {
        d->request->cancel();
    }
}

/*!
    \brief Clears any existing data and retreives a new list of streams using the existing parameters.
*/
void StreamsModel::reload() {
    if (status() != StreamsRequest::Loading) {
        Q_D(StreamsModel);
        clear();
        connect(d->request, SIGNAL(finished()), this, SLOT(_q_onListRequestFinished()));
        d->request->get(d->id);
        emit statusChanged(d->request->status());
    }
}

}

#include "moc_streamsmodel.cpp"
