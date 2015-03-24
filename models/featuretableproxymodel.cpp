#include "featuretableproxymodel.h"

FeatureTableProxyModel::FeatureTableProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

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

    //if the feature is a trafo param reject it
    if(source_model->getCurrentJob()->getFeatures().at(source_row)->getTrafoParam() != NULL){
        return false;
    }

    //filter by group name
    if(source_model->getCurrentJob()->getActiveGroup().compare("") == 0){
        return true;
    }else{
        if(source_model->getCurrentJob()->getFeatures().at(source_row)->getFeature()->getGroupName().compare(source_model->getCurrentJob()->getActiveGroup()) == 0){
            return true;
        }
        return false;
    }

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
    if(source_model == NULL){
        return false;
    }

    //check indexes
    if(!left.isValid() || !right.isValid()){
        return false;
    }

    //get row index for each model index
    int leftIndex = left.row();
    int rightIndex = right.row();

    //get relevant properties of left and right feature
    QString leftName = "", rightName = "";
    int leftId = 0, rightId = 0;
    bool leftNominal = false, rightNominal = false;
    FeatureWrapper *leftFeature = source_model->getCurrentJob()->getFeatures().at(leftIndex);
    FeatureWrapper *rightFeature = source_model->getCurrentJob()->getFeatures().at(rightIndex);
    if(leftFeature != NULL && leftFeature->getFeature() != NULL
            && rightFeature != NULL && rightFeature->getFeature() != NULL){
        leftName = leftFeature->getFeature()->getFeatureName();
        rightName = rightFeature->getFeature()->getFeatureName();
        leftId = leftFeature->getFeature()->getId();
        rightId = rightFeature->getFeature()->getId();
        if(leftFeature->getGeometry() != NULL){
            leftNominal = leftFeature->getGeometry()->getIsNominal();
        }
        if(rightFeature->getGeometry() != NULL){
            rightNominal = rightFeature->getGeometry()->getIsNominal();
        }
    }

    //compare the properties of left and right feature
    if(leftName.compare(rightName) == 0){ //if feature names are equal

        if(leftNominal != rightNominal){ //if one is actual and the other one is nominal
            return leftNominal;
        }

        return leftId > rightId;

    }else{ //if feature names are not equal

        //get smallest id of features with equal name
        int leftSmallestId = -1;
        int rightSmallestId = -1;
        QList<FeatureWrapper *> leftFeatures = source_model->getCurrentJob()->getFeaturesByName(leftName);
        QList<FeatureWrapper *> rightFeatures = source_model->getCurrentJob()->getFeaturesByName(rightName);
        for(int i = 0; i < leftFeatures.size(); i++){
            if(leftSmallestId == -1 || leftFeatures.at(i)->getFeature()->getId() < leftSmallestId){
                leftSmallestId = leftFeatures.at(i)->getFeature()->getId();
            }
        }
        for(int i = 0; i < rightFeatures.size(); i++){
            if(rightSmallestId == -1 || rightFeatures.at(i)->getFeature()->getId() < rightSmallestId){
                rightSmallestId = rightFeatures.at(i)->getFeature()->getId();
            }
        }

        return leftSmallestId > rightSmallestId;

    }

}

/*!
 * \brief FeatureTableProxyModel::filterAcceptsColumn
 * \param source_column
 * \param source_parent
 * \return
 */
bool FeatureTableProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const{
    /*QList<int> displayColumns = GUIConfiguration::displayAttributes(GUIConfiguration::featureAttributes,GUIConfiguration::allAttributes);
    if(displayColumns.contains(source_column)){
        return true;
    }else{
        return false;
    }*/
    return true;
}
