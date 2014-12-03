#include "tablemodel.h"

/*!
 * \brief TableModel::TableModel
 * \param features
 * \param coordSys
 * \param parent
 */
TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{

}

/*!
 * \brief TableModel::rowCount
 * \return
 */
int TableModel::rowCount(const QModelIndex& ) const{
    if(OiFeatureState::getFeatureCount() > 0){
        return OiFeatureState::getFeatureCount();
    }
    return 0;
}

/*!
 * \brief TableModel::columnCount
 * \param parent
 * \return
 */
int TableModel::columnCount(const QModelIndex &parent) const{
    return GUIConfiguration::allAttributes.size();
    //return 33;
}

/*!
 * \brief TableModel::data
 * Displays the in the model specified data of all existing features in the tableview.
 * \param index
 * \param role
 * \return
 */
QVariant TableModel::data(const QModelIndex &index, int role) const{

    try{

        if(!index.isValid())
            return QVariant();

        QString functions = "";

        FeatureWrapper *currentFeature = NULL;
        if(OiFeatureState::getFeatureCount() > index.row()){
            currentFeature = OiFeatureState::getFeatures().at(index.row());
        }

        if(currentFeature != NULL && currentFeature->getFeature() != NULL){

            //neu
            if(Qt::DisplayRole == role){

                switch (index.column()) {
                case 0://type
                    return currentFeature->returnFeatureType();
                case 1://act/nom
                    return currentFeature->getFeature()->getDisplayIsNominal();
                case 2://group
                    return currentFeature->getFeature()->getGroupName();
                case 3://name
                    return currentFeature->getFeature()->getFeatureName();
                case 4://x
                    if(currentFeature->getStation() != NULL && currentFeature->getStation()->coordSys->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayX();
                    }else if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayX();
                    }else{
                        return QVariant();
                    }
                case 5://y
                    if(currentFeature->getStation() != NULL && currentFeature->getStation()->coordSys->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayY();
                    }else if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayY();
                    }else{
                        return QVariant();
                    }
                case 6://z
                    if(currentFeature->getStation() != NULL && currentFeature->getStation()->coordSys->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayZ();
                    }else if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayZ();
                    }else{
                        return QVariant();
                    }
                case 7://stddev
                    return currentFeature->getFeature()->getDisplayStdDev();
                case 8://obs
                    return currentFeature->getFeature()->getDisplayObs();
                case 9://mconfig
                    return currentFeature->getFeature()->getDisplayMConfig();
                case 10://function
                    if(currentFeature->getFeature()->getFunctions().size() == 0){
                        return "no function set";
                    }else{
                        functions += currentFeature->getFeature()->getFunctions().at(0)->getMetaData()->name;
                        for(int i=1;i<currentFeature->getFeature()->getFunctions().size();i++){
                            functions += "," + currentFeature->getFeature()->getFunctions().at(i)->getMetaData()->name;
                        }
                        return functions;
                    }
                case 11://solved
                    return currentFeature->getFeature()->getDisplaySolved();
                case 12://comment
                    return currentFeature->getFeature()->getComment();
                case 13://radius
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayRadius();
                    }else{
                        return QVariant();
                    }
                case 14://i
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayI();
                    }else{
                        return QVariant();
                    }
                case 15://j
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayJ();
                    }else{
                        return QVariant();
                    }
                case 16://k
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayK();
                    }else{
                        return QVariant();
                    }
                case 17://com point
                    return currentFeature->getFeature()->getDisplayIsCommon();
                case 18://scalar value dist
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayScalarDistanceValue();
                    }else{
                        return QVariant();
                    }
                case 19://scalar value rad
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayScalarAngleValue();
                    }else{
                        return QVariant();
                    }
                case 20://temperature
                    return currentFeature->getFeature()->getDisplayScalarTemperatureValue();
                case 21://measurement series
                    return currentFeature->getFeature()->getDisplayScalarMeasurementSeriesValue();
                case 22://expansion origin x
                   return currentFeature->getFeature()->getDisplayExpansionOriginX();
                case 23://expansion origin y
                    return currentFeature->getFeature()->getDisplayExpansionOriginY();
                case 24://expansion origin z
                    return currentFeature->getFeature()->getDisplayExpansionOriginZ();
                case 25://use
                    if(currentFeature->getTrafoParam() != NULL){
                        return currentFeature->getTrafoParam()->getIsUsed();
                    }
                    return QVariant();
                case 26://datum trafo
                    if(currentFeature->getTrafoParam() != NULL){
                        return currentFeature->getTrafoParam()->getisDatumTrafo();
                    }
                    return QVariant();
                case 27://start system
                    return currentFeature->getFeature()->getDisplayStartSystem();
                case 28://dest system
                    return currentFeature->getFeature()->getDisplayDestinationSystem();
                case 29://tx
                    return currentFeature->getFeature()->getDisplayTranslationX();
                case 30://ty
                    return currentFeature->getFeature()->getDisplayTranslationY();
                case 31://tz
                    return currentFeature->getFeature()->getDisplayTranslationZ();
                case 32://rx
                    return currentFeature->getFeature()->getDisplayRotationX();
                case 33://ry
                    return currentFeature->getFeature()->getDisplayRotationY();
                case 34://rz
                    return currentFeature->getFeature()->getDisplayRotationZ();
                case 35://sx
                    return currentFeature->getFeature()->getDisplayScaleX();
                case 36://sy
                    return currentFeature->getFeature()->getDisplayScaleY();
                case 37://sz
                    return currentFeature->getFeature()->getDisplayScaleZ();
                case 38://time
                    if(currentFeature->getTrafoParam() != NULL){
                        return currentFeature->getTrafoParam()->getValidTime();
                    }
                    return QVariant();
                default:
                    break;
                }

            }
            /*
            //alt
            if(Qt::DisplayRole == role){

                switch (index.column()) {
                case 0:
                    return currentFeature->returnFeatureType();
                case 1:
                    return currentFeature->getFeature()->getFeatureName();
                case 2:
                    return currentFeature->getFeature()->getGroupName();
                case 3://special case for stations, because they use the xyz origin of their coordinate system
                    if(currentFeature->getStation() != NULL && currentFeature->getStation()->coordSys->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayX();
                    }else if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayX();
                    }else{
                        return QVariant();
                    }
                case 4://special case for stations, because they use the xyz origin of their coordinate system
                    if(currentFeature->getStation() != NULL && currentFeature->getStation()->coordSys->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayY();
                    }else if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayY();
                    }else{
                        return QVariant();
                    }
                case 5://special case for stations, because they use the xyz origin of their coordinate system
                    if(currentFeature->getStation() != NULL && currentFeature->getStation()->coordSys->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayZ();
                    }else if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayZ();
                    }else{
                        return QVariant();
                    }
                case 6:
                    return currentFeature->getFeature()->getDisplayObs();
                case 7:
                    return currentFeature->getFeature()->getDisplaySolved();
                case 8:
                    return currentFeature->getFeature()->getDisplayMConfig();
                case 9:
                    if(currentFeature->getFeature()->getFunctions().size() == 0){
                        return "no function set";
                    }else{
                        functions += currentFeature->getFeature()->getFunctions().at(0)->getMetaData()->name;
                        for(int i=1;i<currentFeature->getFeature()->getFunctions().size();i++){
                            functions += "," + currentFeature->getFeature()->getFunctions().at(i)->getMetaData()->name;
                        }
                        return functions;
                    }
                case 10:
                    return currentFeature->getFeature()->getDisplayIsCommon();
                case 11:
                    return currentFeature->getFeature()->getDisplayIsNominal();
                case 12:
                    return currentFeature->getFeature()->getDisplayStdDev();
                case 13:
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayI();
                    }else{
                        return QVariant();
                    }
                case 14:
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayJ();
                    }else{
                        return QVariant();
                    }
                case 15:
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayK();
                    }else{
                        return QVariant();
                    }
                case 16:
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayRadius();
                    }else{
                        return QVariant();
                    }
                case 17:
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayScalarDistanceValue();
                    }else{
                        return QVariant();
                    }
                case 18:
                    if(currentFeature->getFeature()->getIsSolved()){
                        return currentFeature->getFeature()->getDisplayScalarAngleValue();
                    }else{
                        return QVariant();
                    }
                case 19:
                    return currentFeature->getFeature()->getDisplayStartSystem();
                case 20:
                    return currentFeature->getFeature()->getDisplayDestinationSystem();
                case 21:
                    return currentFeature->getFeature()->getDisplayTranslationX();
                case 22:
                    return currentFeature->getFeature()->getDisplayTranslationY();
                case 23:
                    return currentFeature->getFeature()->getDisplayTranslationZ();
                case 24:
                    return currentFeature->getFeature()->getDisplayRotationX();
                case 25:
                    return currentFeature->getFeature()->getDisplayRotationY();
                case 26:
                    return currentFeature->getFeature()->getDisplayRotationZ();
                case 27:
                    return currentFeature->getFeature()->getDisplayScaleX();
                case 28:
                    return currentFeature->getFeature()->getDisplayScaleY();
                case 29:
                    return currentFeature->getFeature()->getDisplayScaleZ();
                case 30:
                    return currentFeature->getFeature()->getDisplayScalarTemperatureValue();
                case 31:
                    return currentFeature->getFeature()->getDisplayScalarMeasurementSeriesValue();
                case 32:
                    return currentFeature->getFeature()->getComment();
                case 33:
                    if(currentFeature->getTrafoParam() != NULL){
                        return currentFeature->getTrafoParam()->getIsUsed();
                    }
                    return QVariant();
                case 34:
                    if(currentFeature->getTrafoParam() != NULL){
                        return currentFeature->getTrafoParam()->getValidTime();
                    }
                    return QVariant();
                case 35:
                    if(currentFeature->getTrafoParam() != NULL){
                        return currentFeature->getTrafoParam()->getisDatumTrafo();
                    }
                    return QVariant();
                case 36:
                    return currentFeature->getFeature()->getDisplayExpansionOriginX();
                case 37:
                    return currentFeature->getFeature()->getDisplayExpansionOriginY();
                case 38:
                    return currentFeature->getFeature()->getDisplayExpansionOriginZ();
                default:
                    break;
                }

            }*/

            if (role == Qt::BackgroundRole){

                /*if(currentFeature->getGeometry() != NULL && currentFeature->getGeometry()->getMeasurementConfig().typeOfReading == -1){
                    if(index.column() == 8){
                        return QColor(Qt::red);
                    }
                }

                if(currentFeature->getStation() != NULL && currentFeature->getStation()->position->getMeasurementConfig().typeOfReading == -1){
                    if(index.column() == 8){
                        return QColor(Qt::red);
                    }
                }*/

                if (OiFeatureState::getActiveFeature() != NULL && currentFeature->getFeature() == OiFeatureState::getActiveFeature()->getFeature()){
                    return QColor(QColor::fromCmykF(0.59,0.40,0.10,0.10).lighter());
                }

                if(OiFeatureState::getFeatures().at(index.row())->getStation() != NULL){
                    if(OiFeatureState::getFeatures().at(index.row())->getStation() == OiFeatureState::getActiveStation()){
                        return QColor(Qt::darkGray);
                    }
                }

                if (OiFeatureState::getFeatures().at(index.row())->getStation()!=NULL){
                    return QColor(Qt::lightGray);
                }

                //if feature is not solved
                if( (index.column() == 4 || index.column() == 5 || index.column() == 6
                        || index.column() == 13 || index.column() == 14
                        || index.column() == 15 || index.column() == 16
                        || index.column() == 18 || index.column() == 19
                        || index.column() == 20 || index.column() == 21)
                        && !currentFeature->getFeature()->getIsSolved()){
                    return QColor(Qt::yellow);
                }
                if(OiFeatureState::getFeatures().at(index.row())->getGeometry() != NULL &&
                        OiFeatureState::getFeatures().at(index.row())->getGeometry()->getIsNominal()){
                    return QColor(QColor::fromRgb(230,230,180));
                }

            }

            if (role == Qt::ForegroundRole ) {

                if(currentFeature->getStation() != NULL && currentFeature->getStation()->getIsActiveStation()){
                    return QColor(Qt::white);
                }
            }

        }
        return QVariant();

    }catch(exception &e){
        qDebug() << e.what();
        return QVariant();
    }


}

/*!
 * \brief TableModel::headerData
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const{

    QStringList m_columns = GUIConfiguration::allAttributes;

    if((Qt::DisplayRole == role) &&
            (Qt::Horizontal == orientation) &&
            (0 <= section) &&
            (section < columnCount())){

        return m_columns.at(section);
    }
    return QVariant();
}

/*!
 * \brief TableModel::updateModel
 */
void TableModel::updateModel(){
    emit layoutAboutToBeChanged();
    emit layoutChanged();
    //emit resizeTable();
}

/*!
 * \brief TableModel::flags
 * Add edit-functionality to some cells of tablemodel
 * \param index
 * \return
 */
Qt::ItemFlags TableModel::flags(const QModelIndex & index) const{
    Qt::ItemFlags myFlags = QAbstractTableModel::flags(index);
    return (myFlags | Qt::ItemIsEditable);
}

/*!
 * \brief TableModel::setData
 * Update the edited features
 * \param index
 * \param value
 * \param role
 * \return
 */
bool TableModel::setData(const QModelIndex & index, const QVariant & value, int role){
    if(OiFeatureState::getActiveFeature() != NULL){
        if(index.column() == 3){ //feature name

            FeatureAttributesExchange myExchange;
            if(OiFeatureState::getActiveFeature()->getGeometry() != NULL){
                myExchange.actual = !OiFeatureState::getActiveFeature()->getGeometry()->getIsNominal();
                myExchange.nominal = OiFeatureState::getActiveFeature()->getGeometry()->getIsNominal();
                myExchange.nominalSystem = OiFeatureState::getActiveFeature()->getGeometry()->getNominalSystem();
            }else if(OiFeatureState::getActiveFeature()->getTrafoParam() != NULL){
                myExchange.startSystem = OiFeatureState::getActiveFeature()->getTrafoParam()->getStartSystem();
                myExchange.destSystem = OiFeatureState::getActiveFeature()->getTrafoParam()->getDestinationSystem();
            }
            myExchange.featureType = OiFeatureState::getActiveFeature()->getTypeOfFeature();
            myExchange.name = OiFeatureState::getActiveFeature()->getFeature()->getFeatureName();

            if(FeatureUpdater::validateFeatureName(value.toString(), myExchange)){

                if(OiFeatureState::getActiveFeature()->getGeometry() != NULL){ //if active feature is geometry then corresponding nominals have to be taken in account
                    Geometry *myGeom = OiFeatureState::getActiveFeature()->getGeometry();
                    if(myGeom->getIsNominal() && myGeom->getMyActual() != NULL){
                        myGeom->getMyActual()->setFeatureName(value.toString());
                        foreach(Geometry *nomGeom, myGeom->getMyActual()->getMyNominals()){
                            if(nomGeom != NULL){
                                nomGeom->setFeatureName(value.toString());
                            }
                        }
                    }else{
                        myGeom->setFeatureName(value.toString());
                        foreach(Geometry *nomGeom, myGeom->getMyNominals()){
                            if(nomGeom != NULL){
                                nomGeom->setFeatureName(value.toString());
                            }
                        }
                    }
                }else{
                    OiFeatureState::getActiveFeature()->getFeature()->setFeatureName(value.toString());
                }

                FeatureUpdater::checkForNominals(OiFeatureState::getActiveFeature());
                FeatureUpdater::addNominalToActual(OiFeatureState::getActiveFeature());
				
                OiFeatureState::sortFeatures();

            }

        }else if(index.column() == 2){ //feature group
            QString oldValue = OiFeatureState::getActiveFeature()->getFeature()->getGroupName();
            OiFeatureState::getActiveFeature()->getFeature()->setGroupName(value.toString());
            emit this->groupNameChanged(oldValue, value.toString());
        }else if(index.column() == 12){ //feature comment
            OiFeatureState::getActiveFeature()->getFeature()->setComment(value.toString());
        }else if(index.column() == 25){//trafo param use
            OiFeatureState::getActiveFeature()->getTrafoParam()->setIsUsed(value.toBool());
        }else if(index.column() == 38){//trafo param time
            OiFeatureState::getActiveFeature()->getTrafoParam()->setValidTime(value.toDateTime());
        }else if(index.column() == 26){//trafo param datum transformation
            OiFeatureState::getActiveFeature()->getTrafoParam()->setisDatumTrafo(value.toBool());
        }else if(index.column() == 22){
            OiFeatureState::getActiveFeature()->getCoordinateSystem()->setExpansionOriginX(value.toDouble()/UnitConverter::getDistanceMultiplier());
        }else if(index.column() == 23){
            OiFeatureState::getActiveFeature()->getCoordinateSystem()->setExpansionOriginY(value.toDouble()/UnitConverter::getDistanceMultiplier());
        }else if(index.column() == 24){
            OiFeatureState::getActiveFeature()->getCoordinateSystem()->setExpansionOriginZ(value.toDouble()/UnitConverter::getDistanceMultiplier());
        }


        this->updateModel();

        return true;
    }
    return false;
}
