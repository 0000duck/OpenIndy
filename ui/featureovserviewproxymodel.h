#ifndef FEATUREOVSERVIEWPROXYMODEL_H
#define FEATUREOVSERVIEWPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "featurewrapper.h"
#include "guiconfiguration.h"

/*!
 * \brief The FeatureOvserviewProxyModel class  is a proxy model that filters all relevant feature attributes.
 * This proxy model only displays geometries, stations and coordinate systems with all their relevant attributes.
 */
class FeatureOvserviewProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:

    QList<FeatureWrapper*> &features;

    explicit FeatureOvserviewProxyModel(QList<FeatureWrapper*> &features,QObject *parent = 0);

    QList<FeatureWrapper*> getFeaturesAtIndices(QModelIndexList &indices);

public slots:
    void activeGroupChanged(QString group);
    
protected:
    bool filterAcceptsColumn ( int source_column, const QModelIndex & source_parent ) const;
    bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;

private:
    QString activeGroup;
    void sortNominalToActual();
    
};

#endif // FEATUREOVSERVIEWPROXYMODEL_H
