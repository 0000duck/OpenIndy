#include "oiexchangeascii.h"

oiExchangeASCII::oiExchangeASCII()
{
}

bool oiExchangeASCII::importOiData(oiExchangeObject &data){

    //check element type
    switch (data.typeOfElement) {
    case (Configuration::ePointElement):{
        return importPoint(data);
        break;}
    case (Configuration::ePlaneElement):{
        return importPlane(data);
        break;}
    case (Configuration::eLineElement):{
        return importLine(data);
        break;}
    case (Configuration::eSphereElement):{
        return importSphere(data);
        break;}
    default:
        break;
    }

    return false;
}

 QList<Configuration::ElementTypes> oiExchangeASCII::getSupportedElements(){

     QList<Configuration::ElementTypes> elements;

        elements.append(Configuration::ePointElement);
        elements.append(Configuration::eLineElement);
        elements.append(Configuration::ePlaneElement);
        elements.append(Configuration::eSphereElement);

        return elements;
 }

 QStringList oiExchangeASCII::getElementDescription(Configuration::ElementTypes t)
 {
     QStringList description;

     switch (t) {
     case Configuration::ePointElement:
         description.append("imports points according to the following scheme:");
         description.append("name  x  y  z");
         return description;
         break;
     case Configuration::eLineElement:
         description.append("imports lines according to the following scheme:");
         description.append("name  x  y  z i j k");
         return description;
         break;
     case Configuration::ePlaneElement:
         description.append("imports planes according to the following scheme:");
         description.append("name  x  y  z i j k");
         return description;
         break;
     case Configuration::eSphereElement:
         description.append("imports spheres according to the following scheme:");
         description.append("name  x  y  z radius");
         return description;
         break;
     default:
         return description;
         break;
     }

 }

 QList<UnitConverter::unitType> oiExchangeASCII::getSupportedDistanceUnits()
 {
     QList<UnitConverter::unitType> d;

     d.append(UnitConverter::eMETER);
     d.append(UnitConverter::eMILLIMETER);

     return d;
 }

 QList<UnitConverter::unitType> oiExchangeASCII::getSupportedAngleUnits()
 {
     QList<UnitConverter::unitType> a;
     return a;

 }

 QList<UnitConverter::unitType> oiExchangeASCII::getSupportedTemperatureUnits()
 {
     QList<UnitConverter::unitType> t;
     return t;
 }

bool oiExchangeASCII::exportOiData(oiExchangeObject &data){

    //TODO export feature in ascii format
    return false;
}

bool oiExchangeASCII::importPoint(oiExchangeObject& data){

    //open and check device
    if (!data.device->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }


    QTextStream in(data.device);
    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList list = line.split(QRegExp("\\s+"));

        Point *p = new Point;

        p->setFeatureName(list.at(0));
        p->myNominalCoordSys = data.nominalCoordSys;
        p->xyz.setAt(0,list.at(1).toDouble());
        p->xyz.setAt(1,list.at(2).toDouble());
        p->xyz.setAt(2,list.at(3).toDouble());
        if(data.unit.value(UnitConverter::eMetric) == UnitConverter::eMILLIMETER){
            p->xyz = p->xyz/1000;
        }
        p->isNominal = true;
        p->setIsSolved(true);

        FeatureWrapper *f = new FeatureWrapper();

        f->setPoint(p);

        data.features.append(f);

        //data.nominalCoordSys->nominals.append(p);
    }

    return true;
}

bool oiExchangeASCII::importPlane(oiExchangeObject& data){

    //open and check device
    if (!data.device->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in(data.device);
    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList list = line.split(QRegExp("\\s+"));

        FeatureWrapper *geom = new FeatureWrapper();

            Plane *p = new Plane();
            p->setFeatureName(list.at(0));
            p->myNominalCoordSys = data.nominalCoordSys;
            p->xyz.setAt(0,list.at(1).toDouble());
            p->xyz.setAt(1,list.at(2).toDouble());
            p->xyz.setAt(2,list.at(3).toDouble());
            p->ijk.setAt(0,list.at(4).toDouble());
            p->ijk.setAt(1,list.at(5).toDouble());
            p->ijk.setAt(2,list.at(6).toDouble());
            p->isNominal = true;
            p->setIsSolved(true);


            geom->setPlane(p);


            //data.nominalCoordSys->nominals.append(p);

            data.features.append(geom);
    }

    return true;
}

bool oiExchangeASCII::importLine(oiExchangeObject& data){

    //open and check device
    if (!data.device->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in(data.device);
    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList list = line.split(QRegExp("\\s+"));

        FeatureWrapper *geom = new FeatureWrapper();

            Line *l = new Line();
            l->setFeatureName(list.at(0));
            l->myNominalCoordSys = data.nominalCoordSys;
            l->xyz.setAt(0,list.at(1).toDouble());
            l->xyz.setAt(1,list.at(2).toDouble());
            l->xyz.setAt(2,list.at(3).toDouble());
            l->ijk.setAt(0,list.at(4).toDouble());
            l->ijk.setAt(1,list.at(5).toDouble());
            l->ijk.setAt(2,list.at(6).toDouble());
            l->isNominal = true;
            l->setIsSolved(true);


            geom->setLine(l);


            //data.nominalCoordSys->nominals.append(l);

            data.features.append(geom);
    }

    return true;
}

bool oiExchangeASCII::importSphere(oiExchangeObject& data){

    //open and check device
    if (!data.device->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QTextStream in(data.device);
    while(!in.atEnd())
    {
        QString line = in.readLine();

        QStringList list = line.split(QRegExp("\\s+"));

        FeatureWrapper *geom = new FeatureWrapper();

            Sphere *s = new Sphere();
            s->setFeatureName(list.at(0));
            s->myNominalCoordSys = data.nominalCoordSys;
            s->xyz.setAt(0,list.at(1).toDouble());
            s->xyz.setAt(1,list.at(2).toDouble());
            s->xyz.setAt(2,list.at(3).toDouble());
            s->radius=list.at(4).toDouble();
            s->isNominal = true;
            s->setIsSolved(true);

            geom->setSphere(s);


            //data.nominalCoordSys->nominals.append(s);

            data.features.append(geom);
    }

    return true;
}
