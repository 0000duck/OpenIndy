#include "trafocontroller.h"

TrafoController::TrafoController(QObject *parent) :
    QObject(parent)
{
}

/*!
 * \brief addObservation corrects and transforms new observations and adds them afterwards in the list of observations.
 * \param obs
 */
void TrafoController::addObservation(Observation *obs)
{
    if(obs->myReading->typeofReading == Configuration::ePolar || obs->myReading->typeofReading == Configuration::eCartesian){
        applyMovements(obs);
        transformNewObservations(obs);
        //OiFeatureState::getActiveStation()->coordSys->observations.append(obs);
    }
}

/*!
 * \brief applyMovements applys the valid movement transformation on the observation, if one exists.
 * \param obs
 */
void TrafoController::applyMovements(Observation *obs)
{
    //find movements for that observation
    QList<TrafoParam*> movements;
    for(int i=0;i<OiFeatureState::getFeatureCount();i++){
        if(OiFeatureState::getFeatures().at(i)->getTrafoParam() != NULL && OiFeatureState::getFeatures().at(i)->getTrafoParam()->getIsMovement()){
            if(OiFeatureState::getFeatures().at(i)->getTrafoParam()->getDestinationSystem() == obs->myStation->coordSys){
                movements.append(OiFeatureState::getFeatures().at(i)->getTrafoParam());
            }
        }
    }
    if(movements.size()>0){
        //sort list on valid time attributes
        QMap<QDateTime,TrafoParam*> map;
        for(int k=0;k<movements.size();k++){
            map.insert(movements.at(k)->getValidTime(),movements.at(k));
        }
        movements = map.values();

        //apply movement
        for(int i=0;i<movements.size();i++){
            if(obs->myReading->measuredAt<movements.at(i)->getValidTime() && i != 0){
                OiMat t= movements.at(i)->getHomogenMatrix();
                obs->myXyz = t * obs->myOriginalXyz;
                obs->myStatistic.qxx = t * obs->myOriginalStatistic.qxx;
            }
        }
    }else{
        obs->myXyz = obs->myOriginalXyz;
        obs->myStatistic = obs->myOriginalStatistic;
    }
    obs->isValid = true;
}

/*!
 * \brief transformNewObservations transforms new observations directly into the active coordinate system if possible.
 * \param obs
 */
void TrafoController::transformNewObservations(Observation *obs)
{
    if(obs->myStation->coordSys != OiFeatureState::getActiveCoordinateSystem()){
        TrafoParam *tp = findTrafoParam(obs->myStation->coordSys,OiFeatureState::getActiveCoordinateSystem());
        if(tp != NULL){
            OiMat t;
            if(tp->getDestinationSystem() == OiFeatureState::getActiveCoordinateSystem()){
                t = tp->getHomogenMatrix();
            }else{
                t = tp->getHomogenMatrix().inv();
            }
            obs->myXyz = t * obs->myXyz;
            obs->myStatistic.qxx = t * obs->myStatistic.qxx;
        }
    }
}

/*!
 * \brief initPointers gets actual lists
 * \param dlh
 */
void TrafoController::initPointers(DataListHandler dlh)
{
    /*TrafoController::features = dlh.features;
    TrafoController::activeFeature = dlh.activeFeature;
    TrafoController::coordSys = dlh.coordSys;
    TrafoController::stations = dlh.stations;
    TrafoController::activeStation = dlh.activeStation;
    TrafoController::activeCoordinateSystem = dlh.activeCoordinateSystem;
    TrafoController::availableGroups = dlh.availableGroups;*/
}

/*!
 * \brief transformObservations transforms the observations of the specified coordinatesystem.
 * \param from
 * \return
 */
bool TrafoController::transformObservations(CoordinateSystem *from)
{
    if(from != NULL){
        //first apply movements
        foreach (Observation *obs, from->getObservations()) {
            applyMovements(obs);
        }
        if(from == OiFeatureState::getActiveCoordinateSystem()){
            return true;
        }
        //then transform
        TrafoParam *tp = findTrafoParam(from,OiFeatureState::getActiveCoordinateSystem());
        if(tp != NULL){
            OiMat t;
            if(tp->getStartSystem() == from){
                t = tp->getHomogenMatrix();
            }else{
                t = tp->getHomogenMatrix().inv();
            }
            foreach (Observation *obs, from->getObservations()) {
                obs->myXyz = t*obs->myXyz;
                obs->myStatistic.qxx = t*obs->myStatistic.qxx;
                obs->isValid = true;
            }
            return true;
        }else{ //no trafo params available
            foreach(Observation *obs, from->getObservations()){
                obs->isValid = false;
            }
            return false;
        }
    }
    return false;
}

/*!
 * \brief setObservationState
 * \param cs
 * \param valid
 */
void TrafoController::setObservationState(CoordinateSystem *cs, bool valid)
{
    foreach(Observation *obs, cs->getObservations()){
        if(valid == true){
            applyMovements(obs);
            //obs->myXyz = obs->myOriginalXyz;
        }
        obs->isValid = valid;
    }
}

/*!
 * \brief MovementsChanged need to recalc movements and correct all observations after a time of a movement or the state changed, a movement was added or a movement was deleted.
 */
void TrafoController::MovementsChanged()
{
    recalcObservations();
    recalcAllMovements();
    recalcObservations();
}

/*!
 * \brief :recalcAllMovements recalculates all movement transformations.
 */
void TrafoController::recalcAllMovements()
{
    for(int i=0;i<OiFeatureState::getFeatures().size();i++){
        if(OiFeatureState::getFeatures().at(i)->getTrafoParam() != NULL){
            OiFeatureState::getFeatures().at(i)->getTrafoParam()->recalc();
        }
    }
}

/*!
 * \brief recalcObservations with the new movement transformations
 */
void TrafoController::recalcObservations()
{
    for(int i=0;i<OiFeatureState::getStations().size();i++){
        foreach (Observation *obs, OiFeatureState::getStations().at(i)->coordSys->getObservations()) {
            applyMovements(obs);
        }
    }for(int i=0;i<OiFeatureState::getCoordinateSystems().size();i++){
        foreach (Observation *obs, OiFeatureState::getCoordinateSystems().at(i)->getObservations()) {
            applyMovements(obs);
        }
    }
}

/*!
 * \brief findTrafoParam for the specified coordinate system.
 * \param from
 * \param to
 * \return
 */
TrafoParam *TrafoController::findTrafoParam(CoordinateSystem *from, CoordinateSystem *to)
{
    foreach(TrafoParam *tp, from->getTransformationParameters()){
        if(tp->getDestinationSystem() != NULL && tp->getStartSystem() != NULL){
            if(tp->getDestinationSystem() == to || tp->getStartSystem() == to){
                if(tp->getIsUsed()){
                    return tp;
                }
            }
        }
    }
    return NULL;
}
