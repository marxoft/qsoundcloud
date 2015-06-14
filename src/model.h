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

#ifndef QSOUNDCLOUD_MODEL_H
#define QSOUNDCLOUD_MODEL_H

#include "qsoundcloud_global.h"
#include <QAbstractListModel>

namespace QSoundCloud {

class ModelPrivate;

class QSOUNDCLOUDSHARED_EXPORT Model : public QAbstractListModel
{
    Q_OBJECT
    
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
                
public:
    explicit Model(QObject *parent = 0);
    ~Model();
    
#if QT_VERSION >= 0x050000
    QHash<int, QByteArray> roleNames() const;
#endif
    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    
    QVariant data(const QModelIndex &index, int role) const;
    QMap<int, QVariant> itemData(const QModelIndex &index) const;
    
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles);
    
    void append(const QMap<int, QVariant> &roles);
    void insert(const QModelIndex &index, const QMap<int, QVariant> &roles);
    bool remove(const QModelIndex &index);
    
    Q_INVOKABLE QVariantMap get(int row) const;
    Q_INVOKABLE bool setProperty(int row, const QString &property, const QVariant &value);
    Q_INVOKABLE bool set(int row, const QVariantMap &properties);    
    
    Q_INVOKABLE void append(const QVariantMap &properties);
    Q_INVOKABLE void insert(int row, const QVariantMap &properties);
    Q_INVOKABLE bool remove(int row);

public Q_SLOTS:
    void clear();
    
Q_SIGNALS:
    void countChanged(int c);
    
protected:
    Model(ModelPrivate &dd, QObject *parent = 0);
    
    QScopedPointer<ModelPrivate> d_ptr;
    
    Q_DECLARE_PRIVATE(Model)
    
private:
    Q_DISABLE_COPY(Model)
};

}

#endif // QSOUNDCLOUD_MODEL_H
