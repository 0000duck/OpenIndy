#include "ellipse.h"

/*!
 * \brief Ellipse::Ellipse
 * \param isNominal
 * \param parent
 */
Ellipse::Ellipse(const bool &isNominal, QObject *parent) : Geometry(isNominal, parent){

}

/*!
 * \brief Ellipse::Ellipse
 * \param isNominal
 * \param center
 * \param normal
 * \param a
 * \param b
 * \param semiMajorAxis
 * \param parent
 */
Ellipse::Ellipse(const bool &isNominal, const Position &center, const Direction &normal, const double &a, const double &b, const Direction &semiMajorAxis, QObject *parent) : Geometry(isNominal, parent){
    this->setEllipse(center, normal, a, b, semiMajorAxis);
}

/*!
 * \brief Ellipse::Ellipse
 * \param copy
 * \param parent
 */
Ellipse::Ellipse(const Ellipse &copy, QObject *parent) : Geometry(copy, parent){

    this->center = copy.center;
    this->normal = copy.normal;
    this->a = copy.a;
    this->b = copy.b;
    this->semiMajorAxis = copy.semiMajorAxis;

}

/*!
 * \brief Ellipse::operator =
 * \param copy
 * \return
 */
Ellipse &Ellipse::operator=(const Ellipse &copy){

    this->center = copy.center;
    this->normal = copy.normal;
    this->a = copy.a;
    this->b = copy.b;
    this->semiMajorAxis = copy.semiMajorAxis;

    return *this;

}

/*!
 * \brief Ellipse::~Ellipse
 */
Ellipse::~Ellipse(){

}

/*!
 * \brief Ellipse::hasDirection
 * \return
 */
bool Ellipse::hasDirection() const{
    return true;
}

/*!
 * \brief Ellipse::hasPosition
 * \return
 */
bool Ellipse::hasPosition() const{
    return true;
}

/*!
 * \brief Ellipse::getDirection
 * Returns the normal vector of the ellipse
 * \return
 */
const Direction &Ellipse::getDirection() const{
    return this->normal;
}

/*!
 * \brief Ellipse::getPosition
 * Returns the center of the ellipse
 * \return
 */
const Position &Ellipse::getPosition() const{
    return this->center;
}

/*!
 * \brief Ellipse::getA
 * Returns the semi-major axis of the ellipse
 * \return
 */
const double &Ellipse::getA() const{
    return this->a;
}

/*!
 * \brief Ellipse::getB
 * Returns the semi-minor axis of the ellipse
 * \return
 */
const double &Ellipse::getB() const{
    return this->b;
}

/*!
 * \brief Ellipse::getSemiMajorAxisDirection
 * Returns the direction of the semi-major axis
 * \return
 */
const Direction &Ellipse::getSemiMajorAxisDirection() const{
    return this->semiMajorAxis;
}

/*!
 * \brief Ellipse::setEllipse
 * \param center
 * \param normal
 * \param a
 * \param b
 * \param semiMajorAxis
 */
void Ellipse::setEllipse(const Position &center, const Direction &normal, const double &a, const double &b, const Direction &semiMajorAxis){

    //set the given parameters
    this->center = center;
    this->normal = normal;
    this->a = a;
    this->b = b;
    this->semiMajorAxis = semiMajorAxis;

}

/*!
 * \brief Ellipse::recalc
 */
void Ellipse::recalc(){

    Geometry::recalc();

    //reset ellipse definition if not solved
    if(!this->isSolved){
        this->center.setVector(0.0, 0.0, 0.0);
        this->normal.setVector(0.0, 0.0, 0.0);
        this->a = 0.0;
        this->b = 0.0;
        this->semiMajorAxis.setVector(0.0, 0.0, 0.0);
    }

}

/*!
 * \brief Ellipse::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Ellipse::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement ellipse = Geometry::toOpenIndyXML(xmlDoc);

    if(ellipse.isNull()){
        return ellipse;
    }

    ellipse.setAttribute("type", getGeometryTypeName(eEllipseGeometry));

    return ellipse;

}

/*!
 * \brief Ellipse::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Ellipse::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){


    }

    return result;

}
