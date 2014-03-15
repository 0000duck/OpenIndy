#ifndef CYLINDER_H
#define CYLINDER_H

#include "geometry.h"

class Cylinder : public Geometry
{
public:
    Cylinder();
    Cylinder(const Cylinder &copy);
    void recalc();

    bool toOpenIndyXML(QXmlStreamWriter& stream);
    bool fromOpenIndyXML(QXmlStreamReader& xml);

    //TODO implement cylinder
};

#endif // CYLINDER_H
