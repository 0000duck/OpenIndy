#include "point.h"

#include "featurewrapper.h"
/*
#include "function.h"

MeasurementConfig Point::defaultMeasurementConfig;

Point::Point(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

Point::Point(const Point &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->xyz = copy.xyz;
    this->isSolved = copy.isSolved;
}

Point::~Point(){

}

OiVec Point::getXYZ() const
{
    return this->xyz;
}

void Point::recalc(){

    //clear results
    this->myStatistic.displayResiduals.clear();

    //execute functions
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

        this->xyz = OiVec(4);
        this->setIsSolved(false);

    }

    //if not solved reset statistic
    if(!this->isSolved){
        this->myStatistic = Statistic();
    }

}

QDomElement Point::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement point = Geometry::toOpenIndyXML(xmlDoc);

    if(point.isNull()){
        return point;
    }

    point.setAttribute("type", getGeometryTypeName(ePointGeometry));

    return point;

}

bool Point::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set point attributes
        QDomElement coordinates = xmlElem.firstChildElement("coordinates");

        if(coordinates.isNull() || !coordinates.hasAttribute("x") || !coordinates.hasAttribute("y") || !coordinates.hasAttribute("z")){
            return false;
        }

        this->xyz.setAt(0, coordinates.attribute("x").toDouble());
        this->xyz.setAt(1, coordinates.attribute("y").toDouble());
        this->xyz.setAt(2, coordinates.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);

    }

    return result;

}

bool Point::saveSimulationData()
{

    this->mySimulationData.addXYZ(this->xyz.getAt(0),this->xyz.getAt(1),this->xyz.getAt(2));

    return true;
}

QString Point::getDisplayX(bool showDiff) const{

    double displayValue = this->xyz.getAt(0);
    this->convertMetricValue(displayValue);
    QString value = QString::number(displayValue, 'f', this->getMetricDigits());

    //QString value = QString::number(this->xyz.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    qDebug() << "value=" << value;

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   displayValue = this->xyz.getAt(0)-g->getXYZ().getAt(0);
                   this->convertMetricValue(displayValue);
                   QString diff = QString::number(displayValue, 'f', this->getMetricDigits());

                   //QString diff = QString::number((this->xyz.getAt(0)-g->getXYZ().getAt(0))*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;

}

QString Point::getDisplayY(bool showDiff) const{

    double displayValue = this->xyz.getAt(1);
    this->convertMetricValue(displayValue);
    QString value = QString::number(displayValue, 'f', this->getMetricDigits());

    //QString value = QString::number(this->xyz.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   displayValue = this->xyz.getAt(1)-g->getXYZ().getAt(1);
                   this->convertMetricValue(displayValue);
                   QString diff = QString::number(displayValue, 'f', this->getMetricDigits());

                   //QString diff = QString::number((this->xyz.getAt(1)-g->getXYZ().getAt(1))*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;
}

QString Point::getDisplayZ(bool showDiff) const{

    double displayValue = this->xyz.getAt(2);
    this->convertMetricValue(displayValue);
    QString value = QString::number(displayValue, 'f', this->getMetricDigits());

    //QString value = QString::number(this->xyz.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);

    if(!showDiff){
        return value;
    }

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   displayValue = this->xyz.getAt(2)-g->getXYZ().getAt(2);
                   this->convertMetricValue(displayValue);
                   QString diff = QString::number(displayValue, 'f', this->getMetricDigits());

                   //QString diff = QString::number((this->xyz.getAt(2)-g->getXYZ().getAt(2))*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;
}

QString Point::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString Point::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}

QString Point::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}

QString Point::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Point::getDisplayMConfig() const{
    return this->activeMeasurementConfig.getDisplayName();
}

QString Point::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        //return QString::number(this->myStatistic.stdev*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
    }
    return "-/-";

}
*/

/*!
 * \brief Point::Point
 * \param isNominal
 * \param parent
 */
Point::Point(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPoint(this);
    }

}

/*!
 * \brief Point::Point
 * \param isNominal
 * \param xyz
 * \param parent
 */
Point::Point(const bool &isNominal, const Position &xyz, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPoint(this);
    }

    this->setPoint(xyz);

}

/*!
 * \brief Point::Point
 * \param copy
 * \param parent
 */
Point::Point(const Point &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPoint(this);
    }

    this->xyz = copy.xyz;

}

/*!
 * \brief Point::operator =
 * \param copy
 * \return
 */
Point &Point::operator=(const Point &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setPoint(this);
    }

    this->xyz = copy.xyz;

    return *this;

}

/*!
 * \brief Point::~Point
 */
Point::~Point(){

}

/*!
 * \brief Point::hasPosition
 * \return
 */
bool Point::hasPosition() const{
    return true;
}

/*!
 * \brief Point::getPosition
 * Returns the point's position
 * \return
 */
const Position &Point::getPosition() const{
    return this->xyz;
}

/*!
 * \brief Point::setPoint
 * \param xyz
 */
void Point::setPoint(const Position &xyz){

    //set the given parameters
    this->xyz = xyz;

}

/*!
 * \brief Point::recalc
 */
void Point::recalc(){

    Geometry::recalc();

    //reset point definition if not solved
    if(!this->isSolved){
        this->xyz.setVector(0.0, 0.0, 0.0);
    }

}

/*!
 * \brief Point::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Point::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement point = Geometry::toOpenIndyXML(xmlDoc);

    if(point.isNull()){
        return point;
    }

    point.setAttribute("type", getGeometryTypeName(ePointGeometry));

    return point;

}

/*!
 * \brief Point::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Point::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){
/*
        //set point attributes
        QDomElement coordinates = xmlElem.firstChildElement("coordinates");

        if(coordinates.isNull() || !coordinates.hasAttribute("x") || !coordinates.hasAttribute("y") || !coordinates.hasAttribute("z")){
            return false;
        }

        this->xyz.setAt(0, coordinates.attribute("x").toDouble());
        this->xyz.setAt(1, coordinates.attribute("y").toDouble());
        this->xyz.setAt(2, coordinates.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);
*/
    }

    return result;

}

/*!
 * \brief Point::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Point::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->xyz.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Point::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Point::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->xyz.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Point::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Point::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->xyz.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}
