#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QPointer>
#include <QAbstractTableModel>

#include "feature.h"
#include "featurewrapper.h"
#include "guiconfiguration.h"
#include "oijob.h"

class FeatureTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FeatureTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

signals:
    void groupNameChanged(QString oldValue, QString nweValue);
    void resizeTable();
    
public slots:
    void updateModel();

private:

    
};

#endif // TABLEMODEL_H
