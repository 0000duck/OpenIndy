#ifndef CONE_H
#define CONE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "direction.h"

/*!
 * \brief The Cone class
 * Defines a right circular cone
 */
class Cone : public Geometry
{
    Q_OBJECT

public:
    Cone(const bool &isNominal, QObject *parent = 0);
    Cone(const bool &isNominal, const Position &apex, const Direction &axis, const double &aperture, QObject *parent = 0);

    Cone(const Cone &copy, QObject *parent = 0);

    Cone &operator=(const Cone &copy);

    ~Cone();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum ConeUnknowns{
        unknownApexX = eUnknownX,
        unknownApexY = eUnknownY,
        unknownApexZ = eUnknownZ,
        unknownAxisI = eUnknownPrimaryI,
        unknownAxisJ = eUnknownPrimaryJ,
        unknownAxisK = eUnknownPrimaryK,
        unknownAperture = eUnknownAperture
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;

    //##########################
    //get or set cone parameters
    //##########################

    const Direction &getDirection() const;
    const Position &getPosition() const;

    const double &getAperture() const;

    void setCone(const Position &apex, const Direction &axis, const double &aperture);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //bool saveSimulationData();

    //static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type

private:

    //###############
    //cone attributes
    //###############

    Position apex; //the peak of the cone
    Direction axis; //vector pointing from the apex inside the cone
    double aperture; //opening angle = 2 * angle between surface line and r

};

#endif // CONE_H
