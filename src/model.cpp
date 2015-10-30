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

#include "model_p.h"

namespace QSoundCloud {

Model::Model(QObject *parent) :
    QAbstractListModel(parent),
    d_ptr(new ModelPrivate(this))
{
}

/*!
    \class Model
    \brief The base class for data models providing representations of SoundCloud Data API resources.
    
    \ingroup models
    
    Normally, you should not need to use this class.
*/
Model::Model(ModelPrivate &dd, QObject *parent) :
    QAbstractListModel(parent),
    d_ptr(&dd)
{
}

Model::~Model() {}

#if QT_VERSION >= 0x050000
/*!
    \brief The role names for use with QML.
*/
QHash<int, QByteArray> Model::roleNames() const {
    Q_D(const Model);
    
    return d->roles;
}
#endif

/*!
    \property int Model::count
    \brief The number of items in the model.
*/

/*!
    \fn void Model::countChanged()
    \brief Emitted when items are added/removed.
*/

/*!
    \brief Returns the number of items in the model.
*/
int Model::rowCount(const QModelIndex &) const {
    Q_D(const Model);
    
    return d->items.size();
}

/*!
    \brief Re-implemented from QAbstractListModel::data()
*/
QVariant Model::data(const QModelIndex &index, int role) const {
    Q_D(const Model);
    
    return get(index.row()).value(d->roles.value(role));
}

/*!
    \brief Re-implemented from QAbstractListModel::itemData()
*/
QMap<int, QVariant> Model::itemData(const QModelIndex &index) const {
    Q_D(const Model);
    
    QMap<int, QVariant> map;
    QVariantMap item = get(index.row());
    
    if (!item.isEmpty()) {
        QHashIterator<int, QByteArray> iterator(d->roles);
    
        while (iterator.hasNext()) {
            iterator.next();
            map[iterator.key()] = item.value(iterator.value());
        }
    }
    
    return map;
}

/*!
    \brief Re-implemented from QAbstractListModel::setData()
*/
bool Model::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }
    
    Q_D(Model);
    
    d->items[index.row()][d->roles.value(role)] = value;
    emit dataChanged(index, index);
    
    return true;
}

/*!
    \brief Re-implemented from QAbstractListModel::setItemData()
*/
bool Model::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles) {
    if (!index.isValid()) {
        return false;
    }
    
    Q_D(Model);
    
    QMapIterator<int, QVariant> iterator(roles);
    
    while (iterator.hasNext()) {
        iterator.next();
        d->items[index.row()][d->roles.value(iterator.key())] = iterator.value();
    }
    
    emit dataChanged(index, index);
    
    return true;
}

/*!
    \brief Appends an item using the data in \a roles.
*/
void Model::append(const QMap<int, QVariant> &roles) {
    Q_D(Model);
    
    QVariantMap item;
    QMapIterator<int, QVariant> iterator(roles);
    
    while (iterator.hasNext()) {
        iterator.next();
        item[d->roles.value(iterator.key())] = iterator.value();
    }
    
    beginInsertRows(QModelIndex(), d->items.size(), d->items.size());
    d->items << item;
    endInsertRows();
    emit countChanged(rowCount());
}

/*!
    \brief Inserts an item before \a index using the data in \a roles.
    
    The item is appended if \a index is invalid.
*/
void Model::insert(const QModelIndex &index, const QMap<int, QVariant> &roles) {
    if (!index.isValid()) {
        append(roles);
        return;
    }
    
    Q_D(Model);
    
    QVariantMap item;
    QMapIterator<int, QVariant> iterator(roles);
    
    while (iterator.hasNext()) {
        iterator.next();
        item[d->roles.value(iterator.key())] = iterator.value();
    }
    
    beginInsertRows(QModelIndex(), index.row(), index.row());
    d->items.insert(index.row(), item);
    endInsertRows();
    emit countChanged(rowCount());
}

/*!
    \brief Removes the item at \a index.
    
    Returns true if succesful.
*/
bool Model::remove(const QModelIndex &index) {
    if (!index.isValid()) {
        return false;
    }
    
    Q_D(Model);
    
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    d->items.removeAt(index.row());
    endRemoveRows();
    emit countChanged(rowCount());
    
    return true;
}

/*!
    \brief Returns the item at \a row.
*/
QVariantMap Model::get(int row) const {
    Q_D(const Model);
    
    return (row >= 0) && (row < d->items.size()) ? d->items.at(row) : QVariantMap();
}

/*!
    \brief Sets the \a property of the item at \a row to \a value.
    
    Returns true if successful.
*/
bool Model::setProperty(int row, const QString &property, const QVariant &value) {
    Q_D(Model);
    
    if ((row < 0) || (row >= d->items.size())) {
        return false;
    }
    
    d->items[row][property] = value;
    QModelIndex i = index(row);
    emit dataChanged(i, i);
    
    return true;
}

/*!
    \brief Sets the \a properties of the item at \a row to \a properties.
    
    Returns true if successful.
*/
bool Model::set(int row, const QVariantMap &properties) {
    Q_D(Model);
    
    if ((row < 0) || (row >= d->items.size())) {
        return false;
    }
    
    QMapIterator<QString, QVariant> iterator(properties);
    
    while (iterator.hasNext()) {
        iterator.next();
        d->items[row][iterator.key()] = iterator.value();
    }
    
    QModelIndex i = index(row);
    emit dataChanged(i, i);
    
    return true;
}

/*!
    \brief Appends an item to the model using \a properties.
*/
void Model::append(const QVariantMap &properties) {
    Q_D(Model);
    
    if (d->items.isEmpty()) {
        d->setRoleNames(properties);
    }
    
    beginInsertRows(QModelIndex(), d->items.size(), d->items.size());
    d->items << properties;
    endInsertRows();
    emit countChanged(rowCount());
}

/*!
    \brief Inserts an item before \a row to the model using \a properties.
    
    If \a row is out of range, the item is appended.
*/
void Model::insert(int row, const QVariantMap &properties) {
    Q_D(Model);
    
    if ((row < 0) || (row >= d->items.size())) {
        append(properties);
        return;
    }
    
    if (d->items.isEmpty()) {
        d->setRoleNames(properties);
    }
    
    beginInsertRows(QModelIndex(), row, row);
    d->items.insert(row, properties);
    endInsertRows();
    emit countChanged(rowCount());
}

/*!
    \brief Removes the item at \a row.
    
    Returns true if successful.
*/
bool Model::remove(int row) {
    Q_D(Model);
    
    if ((row < 0) || (row >= d->items.size())) {
        return false;
    }
    
    beginRemoveRows(QModelIndex(), row, row);
    d->items.removeAt(row);
    endRemoveRows();
    emit countChanged(rowCount());
    
    return true;
}

/*!
    \brief Removes all items.
*/
void Model::clear() {
    Q_D(Model);
    
    if (!d->items.isEmpty()) {
        beginResetModel();
        d->items.clear();
        endResetModel();
        emit countChanged(rowCount());
    }
}

ModelPrivate::ModelPrivate(Model *parent) :
    q_ptr(parent)
{
}

ModelPrivate::~ModelPrivate() {}

/*!
    \internal
    \brief Set the role names of the model using the unique keys of \a item.
*/
void ModelPrivate::setRoleNames(const QVariantMap &item) {    
    roles.clear();
    int role = Qt::UserRole + 1;
    
    foreach (QString key, item.uniqueKeys()) {
        roles[role] = key.toUtf8();
        role++;
    }
#if QT_VERSION < 0x050000
    Q_Q(Model);
    
    q->setRoleNames(roles);
#endif
}

}

#include "moc_model.cpp"
