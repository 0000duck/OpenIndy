#ifndef FEATURETABLEPROXYMODEL_H
#define FEATURETABLEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QPointer>

#include "oijob.h"
#include "featuretablemodel.h"
#include "featuretablecolumnconfig.h"

using namespace oi;

/*!
 * \brief The FeatureTableProxyModel class
 */
class FeatureTableProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit FeatureTableProxyModel(QObject *parent = 0);

    //#########################
    //get or set display config
    //#########################

    const FeatureTableColumnConfig &getFeatureTableColumnConfig() const;
    void setFeatureTableColumnConfig(const FeatureTableColumnConfig &config);

    //###########################################
    //override methods of sort filter proxy model
    //###########################################

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

signals:

    //#########################
    //send messages to OpenIndy
    //#########################

    void sendMessage(const QString &msg, const MessageTypes &msgType, const MessageDestinations &msgDest = eConsoleMessage);
    
protected:

    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent ) const;
    bool filterAcceptsColumn (int source_column, const QModelIndex &source_parent ) const;

    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:

    //##############
    //display config
    //##############

    FeatureTableColumnConfig featureTableColumnConfig;
    
};

#endif // FEATURETABLEPROXYMODEL_H
