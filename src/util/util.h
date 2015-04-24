#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QStringList>
#include <QMap>

#include "types.h"

//#######################################
//global functions to query element types
//#######################################

const QString &getElementTypeName(const ElementTypes &type);
ElementTypes getElementTypeEnum(const QString &name);

const QString &getFeatureTypeName(const FeatureTypes &type);
FeatureTypes getFeatureTypeEnum(const QString &name);

const QString &getGeometryTypeName(const GeometryTypes &type);
GeometryTypes getGeometryTypeEnum(const QString &name);

bool getIsGeometry(const FeatureTypes &type);

//#######################################
//global functions to query reading types
//#######################################

const QString &getReadingTypeName(const ReadingTypes &type);
ReadingTypes getReadingTypeEnum(const QString &name);

//####################################################
//global functions to query feature display attributes
//####################################################

const QList<int> &getDisplayAttributes();

bool getIsFeatureDisplayAttribute(const int &attr);
bool getIsTrafoParamDisplayAttribute(const int &attr);

const QString &getDisplayAttributeName(const int &attr);
const QString &getDisplayAttributeName(const FeatureDisplayAttributes &attr);
const QString &getDisplayAttributeName(const TrafoParamDisplayAttributes &attr);

//##################################################
//global functions to convert units to default units
//##################################################

double convertToDefault(const double &value, const UnitType &type);

#endif // UTIL_H
