#ifndef FEATUREATTRIBUTES_H
#define FEATUREATTRIBUTES_H

#include <QString>
#include <QPair>

#include "types.h"

//! Helper class that is used when creating new features to specify their attributes
class FeatureAttributes
{
public:
    FeatureAttributes();

    //general attributes
    FeatureTypes typeOfFeature;
    QString name;
    QString group;
    int count;

    //only when typeOfFeature specifies a geometry
    bool isActual;
    bool isNominal;
    bool isCommon;
    QString nominalSystem;
    QPair<QString, QString> functionPlugin; //T1 = plugin, T2 = function

    //only when typeOfFeature specifies a trafo param
    QString startSystem;
    QString destinationSystem;
    bool isMovement;

};

#endif // FEATUREATTRIBUTES_H
