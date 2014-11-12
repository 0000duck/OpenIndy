#include "scalarentitydistance.h"

#include "function.h"

ScalarEntityDistance::ScalarEntityDistance(bool isNominal, QObject *parent) : Geometry(isNominal, parent)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = false;
}

/*!
 * \brief ScalarEntityDistance::ScalarEntityDistance
 * \param copy
 */
ScalarEntityDistance::ScalarEntityDistance(const ScalarEntityDistance &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->distance = copy.getDistance();
    this->isSolved = copy.isSolved;
}

/*!
 * \brief ScalarEntityDistance::recalc
 * Execute alls functions in the specified order
 */
void ScalarEntityDistance::recalc(){

    if(this->functionList.size() > 0){

        bool solved = true;
        foreach(Function *f, this->functionList){

            //execute the function if it exists and if the last function was executed successfully
            if(f != NULL && solved == true){
                solved = f->exec(*this);
            }

        }
        this->setIsSolved(solved);

    }else if(this->isNominal == false){

        this->distance = 0.0;
        this->setIsSolved(false);

    }

}

/*!
 * \brief ScalarEntityDistance::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement ScalarEntityDistance::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement entityDistance = Geometry::toOpenIndyXML(xmlDoc);

    if(entityDistance.isNull()){
        return entityDistance;
    }

    entityDistance.setAttribute("type", Configuration::sEntityDistance);

    //add distance
    QDomElement distance = xmlDoc.createElement("distance");
    if(this->getIsSolved() || this->getIsNominal()){
        distance.setAttribute("value", this->distance);
    }else{
        distance.setAttribute("value", 0.0);
    }
    entityDistance.appendChild(distance);

    return entityDistance;

}

/*!
 * \brief ScalarEntityDistance::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool ScalarEntityDistance::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement distance = xmlElem.firstChildElement("distance");

        if(distance.isNull() || !distance.hasAttribute("value")){
            return false;
        }

        this->distance = distance.attribute("value").toDouble();

    }

    return result;

}

bool ScalarEntityDistance::saveSimulationData()
{
    this->mySimulationData.addScalar(this->distance);
    return true;
}

/*!
 * \brief ScalarEntityDistance::getDistance
 * \return
 */
double ScalarEntityDistance::getDistance() const{
    return this->distance;
}

/*!
 * \brief ScalarEntityDistance::setDistance
 * \param distance
 */
void ScalarEntityDistance::setDistance(double distance){
    this->distance = distance;
}

double ScalarEntityDistance::getScalar() const
{
    return this->distance;
}

QString ScalarEntityDistance::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString ScalarEntityDistance::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
/*QString ScalarEntityDistance::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}*/

QString ScalarEntityDistance::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString ScalarEntityDistance::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString ScalarEntityDistance::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}

QString ScalarEntityDistance::getDisplayScalarDistanceValue() const{
    return QString::number(this->distance*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}
