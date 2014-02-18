#ifndef OBSERVATIONMODEL_H
#define OBSERVATIONMODEL_H

#include <QAbstractTableModel>
#include "feature.h"
#include "featurewrapper.h"
#include "QStringList"

class ObservationModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ObservationModel(FeatureWrapper &activeFeature,QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;


    FeatureWrapper &selectedFeature;
    //QStringList m_columns;

signals:

public slots:
    void updateModel();
};

#endif // OBSERVATIONMODEL_H
