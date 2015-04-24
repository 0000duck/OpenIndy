#include "torus.h"

#include "featurewrapper.h"

/*!
 * \brief Torus::Torus
 * \param isNominal
 * \param parent
 */
Torus::Torus(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTorus(this);
    }

}

/*!
 * \brief Torus::Torus
 * \param isNominal
 * \param center
 * \param normal
 * \param radiusA
 * \param radiusB
 * \param parent
 */
Torus::Torus(const bool &isNominal, const Position &center, const Direction &normal, const Radius &radiusA, const Radius &radiusB, QObject *parent) : Geometry(isNominal, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTorus(this);
    }

    this->setTorus(center, normal, radiusA, radiusB);

}

/*!
 * \brief Torus::Torus
 * \param copy
 * \param parent
 */
Torus::Torus(const Torus &copy, QObject *parent) : Geometry(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTorus(this);
    }

    this->center = copy.center;
    this->normal = copy.normal;
    this->radiusA = copy.radiusA;
    this->radiusB = copy.radiusB;

}

/*!
 * \brief Torus::operator =
 * \param copy
 * \return
 */
Torus &Torus::operator=(const Torus &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setTorus(this);
    }

    this->center = copy.center;
    this->normal = copy.normal;
    this->radiusA = copy.radiusA;
    this->radiusB = copy.radiusB;

    return *this;

}

/*!
 * \brief Torus::~Torus
 */
Torus::~Torus(){

}

/*!
 * \brief Torus::hasDirection
 * \return
 */
bool Torus::hasDirection() const{
    return true;
}

/*!
 * \brief Torus::hasPosition
 * \return
 */
bool Torus::hasPosition() const{
    return true;
}

/*!
 * \brief Torus::hasRadius
 * \return
 */
bool Torus::hasRadius() const{
    return true;
}

/*!
 * \brief Torus::getRadius
 * Returns the distance of the center to the center curve of the torus
 * \return
 */
const Radius &Torus::getRadius() const{
    return this->radiusA;
}

/*!
 * \brief Torus::getDirection
 * Returns the normal vector of the torus
 * \return
 */
const Direction &Torus::getDirection() const{
    return this->normal;
}

/*!
 * \brief Torus::getPosition
 * Returns the center of the torus
 * \return
 */
const Position &Torus::getPosition() const{
    return this->center;
}

/*!
 * \brief Torus::getSmallRadius
 * Returns the distance of the center curve to the torus surface (radiusA > radiusB)
 * \return
 */
const Radius &Torus::getSmallRadius() const{
    return this->radiusB;
}

/*!
 * \brief Torus::setTorus
 * \param center
 * \param normal
 * \param radiusA
 * \param radiusB
 */
void Torus::setTorus(const Position &center, const Direction &normal, const Radius &radiusA, const Radius &radiusB){

    //set the given parameters
    this->center = center;
    this->normal = normal;
    this->radiusA = radiusA;
    this->radiusB = radiusB;

}

/*!
 * \brief Torus::recalc
 */
void Torus::recalc(){

    Geometry::recalc();

    //reset torus definition if not solved
    if(!this->isSolved){
        this->center.setVector(0.0, 0.0, 0.0);
        this->normal.setVector(0.0, 0.0, 0.0);
        this->radiusA.setRadius(0.0);
        this->radiusB.setRadius(0.0);
    }

}

/*!
 * \brief Torus::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Torus::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement torus = Geometry::toOpenIndyXML(xmlDoc);

    if(torus.isNull()){
        return torus;
    }

    torus.setAttribute("type", getGeometryTypeName(eCircleGeometry));

    return torus;

}

/*!
 * \brief Torus::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Torus::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){


    }

    return result;

}

/*!
 * \brief Torus::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Torus::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->center.getVector().getAt(0), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Torus::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Torus::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->center.getVector().getAt(1), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Torus::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Torus::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->center.getVector().getAt(2), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Torus::getDisplayPrimaryI
 * \param digits
 * \param showDiff
 * \return
 */
QString Torus::getDisplayPrimaryI(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(0), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Torus::getDisplayPrimaryJ
 * \param digits
 * \param showDiff
 * \return
 */
QString Torus::getDisplayPrimaryJ(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(1), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Torus::getDisplayPrimaryK
 * \param digits
 * \param showDiff
 * \return
 */
QString Torus::getDisplayPrimaryK(const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(this->normal.getVector().getAt(2), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Torus::getDisplayRadiusA
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Torus::getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->radiusA.getRadius(), type), 'f', digits);
    }
    return QString("");
}

/*!
 * \brief Torus::getDisplayRadiusB
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Torus::getDisplayRadiusB(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->isSolved){
        return QString::number(convertToDefault(this->radiusB.getRadius(), type), 'f', digits);
    }
    return QString("");
}
