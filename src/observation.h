#ifndef OBSERVATION_H
#define OBSERVATION_H

#include <QList>
#include <math.h>

#include "element.h"
#include "geometry.h"
#include "oivec.h"
#include "statistic.h"

class Geometry;
class Reading;
class Station;

class Observation : public Element
{
public:

    Observation(Reading*, Station*, bool isActiveCoordSys);
    Observation();
    ~Observation();

    Station *myStation;
    QList<Geometry*> myTargetGeometries;
    Reading *myReading;

    OiVec myXyz;
    OiVec myOriginalXyz;

    Statistic myStatistic;
    Statistic myOriginalStatistic;


    OiVec sigmaXyz;

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    void calcFromReading();

    void setIsValid(const bool &isValid);
    bool getIsValid();

    void setIsSolved(const bool &isSolved);
    bool getIsSolved();

    void setIsUsed(const bool &use);
    bool getIsUsed();

    bool getUseState();

private:

    bool isActiveCoordSys; //indicates if the observation coord system is the current
    //coordinate system at time of creation

    bool isValid; //defines if the reading can be transformed, without any function, to a xyz observation

    bool isSolved; //defines wether this observation is valid in current coordinate system

    bool isUsed; //defines if the observation is and should be used for functions
};

#endif // OBSERVATION_H
