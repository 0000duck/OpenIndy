#ifndef CIRCLE_H
#define CIRCLE_H

#include <QObject>
#include <QtXml>

#include "geometry.h"
#include "position.h"
#include "radius.h"
#include "direction.h"

/*!
 * \brief The Circle class
 */
class Circle : public Geometry
{
    Q_OBJECT

public:
    Circle(const bool &isNominal, QObject *parent = 0);
    Circle(const bool &isNominal, const Position &center, const Direction &normal, const Radius &radius, QObject *parent = 0);

    Circle(const Circle &copy, QObject *parent = 0);

    Circle &operator=(const Circle &copy);

    ~Circle();

    //########################################
    //order of unknown parameters (Qxx-matrix)
    //########################################

    enum CircleUnknowns{
        unknownCenterX = eUnknownX,
        unknownCenterY = eUnknownY,
        unknownCenterZ = eUnknownZ,
        unknownNormalI = eUnknownPrimaryI,
        unknownNormalJ = eUnknownPrimaryJ,
        unknownNormalK = eUnknownPrimaryK,
        unknownRadius = eUnknownRadiusA
    };

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    bool hasDirection() const;
    bool hasPosition() const;
    bool hasRadius() const;

    //############################
    //get or set circle parameters
    //############################

    const Radius &getRadius() const;
    const Direction &getDirection() const;
    const Position &getPosition() const;

    void setCircle(const Position &center, const Direction &normal, const Radius &radius);

    //###########################
    //reexecute the function list
    //###########################

    void recalc();

    //#################
    //save and load XML
    //#################

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    //###############
    //display methods
    //###############

    QString getDisplayX(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayY(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryI(const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryJ(const int &digits, const bool &showDiff = false) const;
    QString getDisplayPrimaryK(const int &digits, const bool &showDiff = false) const;
    QString getDisplayRadiusA(const UnitType &type, const int &digits, const bool &showDiff = false) const;

private:

    //#################
    //circle attributes
    //#################

    Position center; //center
    Direction normal; //normal vector
    Radius radius; //radius





    /*QString getDisplayX(bool showDiff) const;
    QString getDisplayY(bool showDiff) const;
    QString getDisplayZ(bool showDiff) const;
    QString getDisplayRadius(bool showDiff) const;
    QString getDisplayIsCommon() const;
    //QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;
    QString getDisplayI(bool showDiff) const;
    QString getDisplayJ(bool showDiff) const;
    QString getDisplayK(bool showDiff) const;

    bool saveSimulationData();

    static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type
*/
};

#endif // CIRCLE_H
