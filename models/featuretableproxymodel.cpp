#include "featuretableproxymodel.h"

FeatureTableProxyModel::FeatureTableProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

/*!
 * \brief FeatureTableProxyModel::getFeatureTableColumnConfig
 * \return
 */
const FeatureTableColumnConfig &FeatureTableProxyModel::getFeatureTableColumnConfig() const{
    return this->featureTableColumnConfig;
}

/*!
 * \brief FeatureTableProxyModel::setFeatureTableColumnConfig
 * \param config
 */
void FeatureTableProxyModel::setFeatureTableColumnConfig(const FeatureTableColumnConfig &config){
    this->featureTableColumnConfig = config;
    this->invalidateFilter();
}

/*!
 * \brief FeatureTableProxyModel::getFeatureIdAtIndex
 * \param index
 * \return
 */
/*int FeatureTableProxyModel::getFeatureIdAtIndex(const QModelIndex &index){

    //check index
    if(!index.isValid()){
        return -1;
    }

    //get and cast source model
    FeatureTableModel *source_model = dynamic_cast<FeatureTableModel *>(this->sourceModel());
    if(source_model == NULL){
        return -1;
    }

    return source_model->getFeatureIdAtIndex(this->mapToSource(index));

}*/

/*!
 * \brief FeatureTableProxyModel::filterAcceptsRow
 * Filter features by group name and feature type
 * \param source_row
 * \param source_parent
 * \return
 */
bool FeatureTableProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{

    //get and cast source model
    FeatureTableModel *source_model = dynamic_cast<FeatureTableModel *>(this->sourceModel());
    if(source_model == NULL){
        return false;
    }

    //check if job is set
    if(source_model->getCurrentJob().isNull()){
        return false;
    }

    //check if the feature at index source_row exists
    if(source_model->getCurrentJob()->getFeatureCount() <= source_row){
        return false;
    }

    //get and check the feature
    QPointer<FeatureWrapper> feature = source_model->getCurrentJob()->getFeaturesList().at(source_row);
    if(feature.isNull() || feature->getFeature().isNull()){
        return false;
    }

    //if the feature is a trafo param reject it
    if(!feature->getTrafoParam().isNull()){
        return false;
    }

    //filter by group name
    if(source_model->getCurrentJob()->getActiveGroup().compare("All Groups") == 0){
        return true;
    }else{
        if(feature->getFeature()->getGroupName().compare(source_model->getCurrentJob()->getActiveGroup()) == 0){
            return true;
        }
        return false;
    }

    return false;

}

/*!
 * \brief FeatureTableProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool FeatureTableProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{

    //get and check display attribute
    if(getDisplayAttributes().size() < source_column){
        return false;
    }
    int attr = getDisplayAttributes().at(source_column);

    //check if the attribute belongs to a non-trafo param feature
    if(!getIsFeatureDisplayAttribute(attr)){
        return false;
    }

    //check visibility of the attribute
    if(this->featureTableColumnConfig.getColumnVisibility((FeatureDisplayAttributes)attr)){
        return true;
    }
    return false;

}

/*!
 * \brief FeatureTableProxyModel::lessThan
 * Sort features by id and put nominals to actuals
 * \param left
 * \param right
 * \return
 */
bool FeatureTableProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const{

    //get and cast source model
    FeatureTableModel *source_model = dynamic_cast<FeatureTableModel *>(this->sourceModel());
    if(source_model == NULL || source_model->getCurrentJob().isNull()){
        return false;
    }

    //check indexes
    if(!left.isValid() || !right.isValid()){
        return false;
    }

    //get and check row index for each model index
    int leftIndex = left.row();
    int rightIndex = right.row();
    if(source_model->getCurrentJob()->getFeaturesList().size() <= leftIndex || source_model->getCurrentJob()->getFeaturesList().size() <= rightIndex){
        return false;
    }

    //get relevant properties of left and right feature
    QString leftName = "", rightName = "";
    int leftId = 0, rightId = 0;
    bool leftNominal = false, rightNominal = false;
    QPointer<FeatureWrapper> leftFeature = source_model->getCurrentJob()->getFeaturesList().at(leftIndex);
    QPointer<FeatureWrapper> rightFeature = source_model->getCurrentJob()->getFeaturesList().at(rightIndex);
    if(!leftFeature.isNull() && !leftFeature->getFeature().isNull()
            && !rightFeature.isNull() && !rightFeature->getFeature().isNull()){
        leftName = leftFeature->getFeature()->getFeatureName();
        rightName = rightFeature->getFeature()->getFeatureName();
        leftId = leftFeature->getFeature()->getId();
        rightId = rightFeature->getFeature()->getId();
        if(!leftFeature->getGeometry().isNull()){
            leftNominal = leftFeature->getGeometry()->getIsNominal();
        }
        if(!rightFeature->getGeometry().isNull()){
            rightNominal = rightFeature->getGeometry()->getIsNominal();
        }
    }

    //compare the properties of left and right feature
    if(leftName.compare(rightName) == 0){ //if feature names are equal

        if(leftNominal != rightNominal){ //if one is actual and the other one is nominal
            return rightNominal;
        }

        return leftId < rightId;

    }else{ //if feature names are not equal

        //use the actual of a nominal for sorting
        if(!leftFeature->getGeometry().isNull() && leftFeature->getGeometry()->getIsNominal() && !leftFeature->getGeometry()->getActual().isNull()){
            leftId = leftFeature->getGeometry()->getActual()->getId();
        }
        if(!rightFeature->getGeometry().isNull() && rightFeature->getGeometry()->getIsNominal() && !rightFeature->getGeometry()->getActual().isNull()){
            rightId = rightFeature->getGeometry()->getActual()->getId();
        }

        return leftId < rightId;

    }

}
