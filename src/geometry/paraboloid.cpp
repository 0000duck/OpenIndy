#include "paraboloid.h"

#include "featurewrapper.h"
/*
#include "function.h"

MeasurementConfig Paraboloid::defaultMeasurementConfig;

Paraboloid::Paraboloid(bool isNominal, QObject *parent) : Geometry(isNominal, parent)
{
    //this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

Paraboloid::Paraboloid(const Paraboloid &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Paraboloid::recalc(){

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

        this->setIsSolved(false);

    }

}

QDomElement Paraboloid::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement paraboloid = Geometry::toOpenIndyXML(xmlDoc);

    if(paraboloid.isNull()){
        return paraboloid;
    }

    paraboloid.setAttribute("type", getGeometryTypeName(eParaboloidGeometry));

    return paraboloid;

}

bool Paraboloid::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}

bool Paraboloid::saveSimulationData()
{
    return false;
}
*/

/*!
 * \brief Paraboloid::Paraboloid
 * \param isNominal
 * \param parent
 */
Paraboloid::Paraboloid(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

}

/*!
 * \brief Paraboloid::Paraboloid
 * \param isNominal
 * \param apex
 * \param axis
 * \param a
 * \param parent
 */
Paraboloid::Paraboloid(const bool &isNominal, const Position &apex, const Direction &axis, const double &a, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

    this->setParaboloid(apex, axis, a);

}

/*!
 * \brief Paraboloid::Paraboloid
 * \param copy
 * \param parent
 */
Paraboloid::Paraboloid(const Paraboloid &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

    this->apex = copy.apex;
    this->axis = copy.axis;

}

/*!
 * \brief Paraboloid::operator =
 * \param copy
 * \return
 */
Paraboloid &Paraboloid::operator=(const Paraboloid &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setParaboloid(this);
    }

    this->apex = copy.apex;
    this->axis = copy.axis;

    return *this;

}

/*!
 * \brief Paraboloid::~Paraboloid
 */
Paraboloid::~Paraboloid(){

}

/*!
 * \brief Paraboloid::hasDirection
 * \return
 */
bool Paraboloid::hasDirection() const{
    return true;
}

/*!
 * \brief Paraboloid::hasPosition
 * \return
 */
bool Paraboloid::hasPosition() const{
    return true;
}

/*!
 * \brief Paraboloid::getDirection
 * Returns the axis vector that points from the apex inside the paraboloid (rotation axis)
 * \return
 */
const Direction &Paraboloid::getDirection() const{
    return this->axis;
}

/*!
 * \brief Paraboloid::getPosition
 * Returns the peak of the paraboloid
 * \return
 */
const Position &Paraboloid::getPosition() const{
    return this->apex;
}

/*!
 * \brief Paraboloid::getA
 * Returns the compression or streatch parameter
 * \return
 */
const double &Paraboloid::getA() const{
    return this->a;
}

/*!
 * \brief Paraboloid::setParaboloid
 * \param apex
 * \param axis
 * \param a
 */
void Paraboloid::setParaboloid(const Position &apex, const Direction &axis, const double &a){

    //set the given parameters
    this->apex = apex;
    this->axis = axis;
    this->a = a;

}

/*!
 * \brief Paraboloid::recalc
 */
void Paraboloid::recalc(){

    Geometry::recalc();

    //reset paraboloid definition if not solved
    if(!this->isSolved){
        this->apex.setVector(0.0, 0.0, 0.0);
        this->axis.setVector(0.0, 0.0, 0.0);
        this->a = 0.0;
    }

}

/*!
 * \brief Paraboloid::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Paraboloid::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement paraboloid = Geometry::toOpenIndyXML(xmlDoc);

    if(paraboloid.isNull()){
        return paraboloid;
    }

    paraboloid.setAttribute("type", getGeometryTypeName(eParaboloidGeometry));

    return paraboloid;

}

/*!
 * \brief Paraboloid::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Paraboloid::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}

/*!
 * \brief Paraboloid::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->apex.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->apex.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->apex.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->axis.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Paraboloid::getDisplayA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Paraboloid::getDisplayA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->a, type), 'f', digits);
    }
    return QString("");
}
