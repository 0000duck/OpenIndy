#include "circle.h"

#include "function.h"

Circle::Circle(bool isNominal, QObject *parent) : Geometry(isNominal, parent), xyz(4), ijk(4), radius(0.0)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Circle::Circle
 * \param copy
 */
Circle::Circle(const Circle &copy) : Geometry(copy.isNominal){
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

/*!
 * \brief Circle::getXYZ returns xyz vector
 * \return
 */
OiVec Circle::getXYZ() const{
    return this->xyz;
}

/*!
 * \brief Circle::getIJK returns ijk vector
 * \return
 */
OiVec Circle::getIJK() const
{
    return this->ijk;
}

double Circle::getRadius() const
{
    return this->radius;
}

/*!
 * \brief Circle::recalc
 */
void Circle::recalc(){

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
        this->radius = 0.0;
        this->dist2origin = 0.0;
        this->setIsSolved(false);

    }

}

/*!
 * \brief Circle::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Circle::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement circle = Geometry::toOpenIndyXML(xmlDoc);

    if(circle.isNull()){
        return circle;
    }

    circle.setAttribute("type", Configuration::sCircle);

    //add radius
    QDomElement radius = xmlDoc.createElement("radius");
    if(this->getIsSolved() || this->getIsNominal()){
        radius.setAttribute("value", this->radius);
    }else{
        radius.setAttribute("value", 0.0);
    }
    circle.appendChild(radius);

    //add normal vector of the circle
    QDomElement ijk = xmlDoc.createElement("spatialDirection");
    if(this->ijk.getSize() >= 3 && (this->getIsSolved() || this->getIsNominal())){
        ijk.setAttribute("i", this->ijk.getAt(0));
        ijk.setAttribute("j", this->ijk.getAt(1));
        ijk.setAttribute("k", this->ijk.getAt(2));
    }else{
        ijk.setAttribute("i", 0.0);
        ijk.setAttribute("j", 0.0);
        ijk.setAttribute("k", 0.0);
    }
    circle.appendChild(ijk);

    return circle;

}

/*!
 * \brief Circle::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Circle::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){

        //set circle attributes
        QDomElement radius = xmlElem.firstChildElement("radius");
        QDomElement normalVector = xmlElem.firstChildElement("spatialDirection");
        QDomElement center = xmlElem.firstChildElement("coordinates");

        if(radius.isNull() || normalVector.isNull() || center.isNull() || !radius.hasAttribute("value")
                || !normalVector.hasAttribute("i") || !normalVector.hasAttribute("j") || !normalVector.hasAttribute("k")
                || !center.hasAttribute("x") || !center.hasAttribute("y") || !center.hasAttribute("z")){
            return false;
        }

        this->radius = radius.attribute("value").toDouble();
        this->ijk.setAt(0, normalVector.attribute("i").toDouble());
        this->ijk.setAt(1, normalVector.attribute("j").toDouble());
        this->ijk.setAt(2, normalVector.attribute("k").toDouble());
        this->ijk.setAt(3, 1.0);
        this->xyz.setAt(0, center.attribute("x").toDouble());
        this->xyz.setAt(1, center.attribute("y").toDouble());
        this->xyz.setAt(2, center.attribute("z").toDouble());
        this->xyz.setAt(3, 1.0);

    }

    return result;

}

ElementDependencies Circle::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    return dependencies;
}

QString Circle::getDisplayX() const{

    QString value = QString::number(this->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(0)-g->getXYZ().getAt(0))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;

}

QString Circle::getDisplayY() const{

    QString value = QString::number(this->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(1)-g->getXYZ().getAt(1))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;
}

QString Circle::getDisplayZ() const{
    QString value = QString::number(this->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

    if(!this->isNominal){
        if(this->isSolved){
           foreach(Geometry *g, this->nominals){
               if(g != NULL && g->getIsSolved()){

                   QString diff = QString::number((this->xyz.getAt(2)-g->getXYZ().getAt(2))*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
                   return QString(value + " (" + diff + ")");
               }
           }
       }
    }

    return value;
}

QString Circle::getDisplayRadius() const{
    return QString::number(this->radius*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
}

QString Circle::getDisplayIsCommon() const{
    return QString(isCommon?"true":"false");
}

QString Circle::getDisplayIsNominal() const{
    return QString(isNominal?"true":"false");
}
/*QString Circle::getDisplayObs() const{
    return QString::number(this->myObservations.size());
}*/

QString Circle::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Circle::getDisplayMConfig() const{
    return this->mConfig.name;
}

QString Circle::getDisplayStdDev() const{

    if(this->myStatistic.isValid){
        return QString::number(this->myStatistic.stdev*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    }else{
        return "-/-";
    }
}

QString Circle::getDisplayI() const{
    QString value = QString::number(this->ijk.getAt(0),'f',6);

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->ijk.getAt(0)-g->getIJK().getAt(0)),'f',6);
                    return QString(value + " (" + diff + ")");
                }
            }
        }
    }

    return value;
}

QString Circle::getDisplayJ() const{
    QString value = QString::number(this->ijk.getAt(1),'f',6);

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->ijk.getAt(1)-g->getIJK().getAt(1)),'f',6);
                    return QString(value + " (" + diff + ")");
                }
            }
        }
    }

    return value;
}

QString Circle::getDisplayK() const{
    QString value = QString::number(this->ijk.getAt(2),'f',6);

    if(!this->isNominal){
        if(this->isSolved){
            foreach (Geometry *g, this->nominals) {
                if(g != NULL && g->getIsSolved()){

                    QString diff = QString::number((this->ijk.getAt(2)-g->getIJK().getAt(2)),'f',6);
                    return QString(value + " (" + diff + ")");
                }
            }
        }
    }

    return value;
}

bool Circle::saveSimulationData()
{
    this->mySimulationData.addXYZ(this->xyz.getAt(0),this->xyz.getAt(1),this->xyz.getAt(2));

    this->mySimulationData.addIJK(this->ijk.getAt(0),this->ijk.getAt(1),this->ijk.getAt(2));

    this->mySimulationData.addRadius(this->radius);

    return true;
}
