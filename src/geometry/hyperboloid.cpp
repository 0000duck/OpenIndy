#include "hyperboloid.h"

#include "function.h"

Hyperboloid::Hyperboloid(bool isNominal, QObject *parent) : Geometry(isNominal, parent)
{
    this->id = Configuration::generateID();
    this->myNominalCoordSys = NULL;
    this->isSolved = false;
    this->isUpdated = false;
    this->isDrawn = true;
}

/*!
 * \brief Hyperboloid::Hyperboloid
 * \param copy
 */
Hyperboloid::Hyperboloid(const Hyperboloid &copy) : Geometry(copy.isNominal) {
    this->id = copy.id;
    this->name = copy.name;
    this->isSolved = copy.isSolved;
}

void Hyperboloid::recalc(){

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

/*!
 * \brief Hyperboloid::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Hyperboloid::toOpenIndyXML(QDomDocument &xmlDoc) const{

    QDomElement hyperboloid = Geometry::toOpenIndyXML(xmlDoc);

    if(hyperboloid.isNull()){
        return hyperboloid;
    }

    hyperboloid.setAttribute("type", Configuration::sHyperboloid);

    return hyperboloid;

}

/*!
 * \brief Hyperboloid::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Hyperboloid::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Geometry::fromOpenIndyXML(xmlElem);

    if(result){



    }

    return result;

}

ElementDependencies Hyperboloid::fromOpenIndyXML(QXmlStreamReader &xml){

    ElementDependencies dependencies;

    return dependencies;
}

bool Hyperboloid::saveSimulationData()
{
    return false;
}
