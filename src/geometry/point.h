#ifndef POINT_H
#define POINT_H

#include "oivec.h"
#include "geometry.h"
#include <QDebug>

/*!
 * \brief The Point class
 * Specific point class with xyz vector.
 */
class Point : public Geometry
{
    Q_OBJECT
public:
    enum PointUnknowns{
        unknownX,
        unknownY,
        unknownZ
    };

    Point(bool isNominal, QObject *parent = 0);
    Point(const Point &copy);
    ~Point();

    OiVec xyz;

    OiVec getXYZ() const;

    void recalc();

    QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    bool fromOpenIndyXML(QDomElement &xmlElem);

    bool saveSimulationData();

    QString getDisplayX(bool showDiff) const;
    QString getDisplayY(bool showDiff) const;
    QString getDisplayZ(bool showDiff) const;
    QString getDisplayIsCommon() const;
    //QString getDisplayIsNominal() const;
    //QString getDisplayObs() const;
    QString getDisplaySolved() const;
    QString getDisplayMConfig() const;
    QString getDisplayStdDev() const;

    static MeasurementConfig defaultMeasurementConfig; //used as a default for this geometry type
};

#endif // POINT_H
