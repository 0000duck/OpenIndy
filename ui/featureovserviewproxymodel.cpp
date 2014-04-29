#include "featureovserviewproxymodel.h"

/*!
 * \brief FeatureOvserviewProxyModel constructor
 * \param QList<FeatureWrapper*> &features
 * \param parent
 */
FeatureOvserviewProxyModel::FeatureOvserviewProxyModel(QList<FeatureWrapper*> &features,QObject *parent) :
    QSortFilterProxyModel(parent),features(features)
{
}

/*!
 * \brief filterAcceptsRow function that filters all relevant feature types. Transformation parameters will not be displayed
 * \param source_row
 * \param source_parent
 * \return
 */
bool FeatureOvserviewProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
    //return true;
    if(this->features.at(source_row)->getTrafoParam() != NULL){
        return false;
    }else{
        return true;
    }
}

/*!
 * \brief filterAcceptsColumn function that filters all relevant attributes for the displayed feature types.
 * \param source_column
 * \param source_parent
 * \return
 */
bool FeatureOvserviewProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
    if(source_column == 18 || source_column == 19 || source_column == 20 || source_column == 21 || source_column == 22
            || source_column == 23 || source_column == 24 || source_column == 25 || source_column == 26
            || source_column == 27 || source_column == 28){
        return false;
    }else{
        return true;
    }

}
