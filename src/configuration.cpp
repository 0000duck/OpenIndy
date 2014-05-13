#include "configuration.h"

Configuration::Configuration(QObject *parent) :
    QObject(parent)
{

}
/*
Configuration::Configuration()
{
}
*/
Configuration::~Configuration()
{
}

QString Configuration::sPlane = "plane";
QString Configuration::sPoint = "point";
QString Configuration::sLine = "line";
QString Configuration::sStation = "station";
QString Configuration::sCoordinatesystem = "coordinatesystem";
QString Configuration::sSphere = "sphere";
QString Configuration::sEntitiyAngle = "sEntitiy angle";
QString Configuration::sEntityDistance = "sEntity distance";
QString Configuration::sTrafoParam = "trafoParam";
QString Configuration::sCircle = "circle";
QString Configuration::sCone = "cone";
QString Configuration::sCylinder = "cylinder";
QString Configuration::sEllipsoid = "ellipsoid";
QString Configuration::sHyperboloid = "hyperboloid";
QString Configuration::sParaboloid = "paraboloid";
QString Configuration::sPointCloud = "pointcloud";
QString Configuration::sNurbs = "nurbs";
QString Configuration::sEntityTemperature = "sEntity temperature";
QString Configuration::sEntityMeasurementSeries = "sEntity measurement series";


QString Configuration::sDistance = "distance";
QString Configuration::sCartesian = "cartesian";
QString Configuration::sPolar = "polar";
QString Configuration::sDirection = "direction";
QString Configuration::sTemperatur = "temperatur";
QString Configuration::sLevel = "level";

QString Configuration::sObservation = "observation";

QStringList Configuration::allAttributes;
QMap<QString,bool> Configuration::featureAttributes;
QMap<QString,bool> Configuration::trafoParamAttributes;

int Configuration::idCount = 0;

int Configuration::generateID(){

    Configuration::idCount  += 1;
    return Configuration::idCount;
}

Configuration::ElementTypes Configuration::getElementTypeEnum(QString s){
    if(s == Configuration::sPlane){
        return Configuration::ePlaneElement;
    }else if(s == Configuration::sPoint){
        return Configuration::ePointElement;
    }else if(s == Configuration::sLine){
        return Configuration::eLineElement;
    }else if(s == Configuration::sStation){
        return Configuration::eStationElement;
    }else if(s == Configuration::sCoordinatesystem){
        return Configuration::eCoordinateSystemElement;
    }else if(s == Configuration::sSphere){
        return Configuration::eSphereElement;
    }else if(s == Configuration::sEntitiyAngle){
        return Configuration::eScalarEntityAngleElement;
    }else if(s == Configuration::sEntityDistance){
        return Configuration::eScalarEntityDistanceElement;
    }else if(s == Configuration::sTrafoParam){
        return Configuration::eTrafoParamElement;
    }else if(s == Configuration::sCircle){
        return Configuration::eCircleElement;
    }else if(s == Configuration::sCone){
        return Configuration::eConeElement;
    }else if(s == Configuration::sCylinder){
        return Configuration::eCylinderElement;
    }else if(s == Configuration::sEllipsoid){
        return Configuration::eEllipsoidElement;
    }else if(s == Configuration::sHyperboloid){
        return Configuration::eHyperboloidElement;
    }else if(s == Configuration::sParaboloid){
        return Configuration::eParaboloidElement;
    }else if(s == Configuration::sNurbs){
        return Configuration::eNurbsElement;
    }else if(s == Configuration::sPointCloud){
        return Configuration::ePointCloudElement;
    }else if(s == Configuration::sObservation){
        return Configuration::eObservationElement;
    }
}

QString Configuration::getElementTypeString(Configuration::ElementTypes e){
    switch (e) {
    case Configuration::ePlaneElement:
        return Configuration::sPlane;
        break;
    case Configuration::ePointElement:
        return Configuration::sPoint ;
        break;
    case Configuration::eLineElement:
        return Configuration::sLine;
        break;
    case Configuration::eStationElement:
        return Configuration::sStation;
        break;
    case Configuration::eCoordinateSystemElement:
        return Configuration::sCoordinatesystem;
        break;
    case Configuration::eSphereElement:
        return Configuration::sSphere;
        break;
    case Configuration::eScalarEntityAngleElement:
        return Configuration::sEntitiyAngle;
        break;
    case Configuration::eScalarEntityDistanceElement:
        return Configuration::sEntityDistance;
         break;
    case Configuration::eTrafoParamElement:
        return Configuration::sTrafoParam;
        break;
    case Configuration::eCircleElement:
        return Configuration::sCircle;
        break;
    case Configuration::eConeElement:
        return Configuration::sCone;
        break;
    case Configuration::eCylinderElement:
        return Configuration::sCylinder;
        break;
    case Configuration::eEllipsoidElement:
        return Configuration::sEllipsoid;
        break;
    case Configuration::eHyperboloidElement:
        return Configuration::sHyperboloid;
        break;
    case Configuration::eParaboloidElement:
        return Configuration::sParaboloid;
        break;
    case Configuration::eNurbsElement:
        return Configuration::sNurbs;
        break;
    case Configuration::ePointCloudElement:
        return Configuration::sPointCloud;
        break;
    case Configuration::eObservationElement:
        return Configuration::sObservation;
        break;
    default:
        return "";
        break;
    }
}

/*!
 * \brief generateAllAttributes fills the QStringList with all existing attribute names.
 * This list is later used for the tableview model.
 * Call this function at project beginning and after every change in unit setting!
 */
void Configuration::generateAllAttributes()
{
    Configuration::allAttributes.append("feature type");
    Configuration::allAttributes.append("feature name");
    Configuration::allAttributes.append("group");
    Configuration::allAttributes.append(QString("x" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append(QString("y" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append(QString("z" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append("observations");
    Configuration::allAttributes.append("solved");
    Configuration::allAttributes.append("measurement config");
    Configuration::allAttributes.append("functions");
    Configuration::allAttributes.append("common point");
    Configuration::allAttributes.append("nominal");
    Configuration::allAttributes.append(QString("stddev" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append("I");
    Configuration::allAttributes.append("J");
    Configuration::allAttributes.append("K");
    Configuration::allAttributes.append(QString("radius" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append("scalar value [m]");
    Configuration::allAttributes.append("scalar value [dec. degree]");
    Configuration::allAttributes.append("start");
    Configuration::allAttributes.append("destination");
    Configuration::allAttributes.append(QString("translation x" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append(QString("translation y" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append(QString("translation z" + UnitConverter::getDistanceUnitString()));
    Configuration::allAttributes.append(QString("rotation x" + UnitConverter::getAngleUnitString()));
    Configuration::allAttributes.append(QString("rotation y" + UnitConverter::getAngleUnitString()));
    Configuration::allAttributes.append(QString("rotation z" + UnitConverter::getAngleUnitString()));
    Configuration::allAttributes.append("scale x");
    Configuration::allAttributes.append("scale y");
    Configuration::allAttributes.append("scale z");
    Configuration::allAttributes.append(QString("temperature" + UnitConverter::getTemperatureUnitString()));
    Configuration::allAttributes.append("measurement series");
    Configuration::allAttributes.append("comment");
}

/*!
 * \brief generateFeatureAttributes fills the QMap with all available attributes for features except trafo parameter.
 * Also includes a boolean that handles if the attributes is displayed or not.
 */
void Configuration::generateFeatureAttributes()
{
    Configuration::featureAttributes.insert("feature type",true);
    Configuration::featureAttributes.insert("feature name",true);
    Configuration::featureAttributes.insert("group",true);
    Configuration::featureAttributes.insert("x",true);
    Configuration::featureAttributes.insert("y",true);
    Configuration::featureAttributes.insert("z",true);
    Configuration::featureAttributes.insert("observation",true);
    Configuration::featureAttributes.insert("solved",true);
    Configuration::featureAttributes.insert("measurement config",true);
    Configuration::featureAttributes.insert("functions",true);
    Configuration::featureAttributes.insert("common point",true);
    Configuration::featureAttributes.insert("nominal",true);
    Configuration::featureAttributes.insert("stddev",true);
    Configuration::featureAttributes.insert("I",true);
    Configuration::featureAttributes.insert("J",true);
    Configuration::featureAttributes.insert("K",true);
    Configuration::featureAttributes.insert("radius",true);
    Configuration::featureAttributes.insert("scalar value [m]",true);
    Configuration::featureAttributes.insert("scalar value [dec. degree]",true);
    Configuration::featureAttributes.insert("temperature",true);
    Configuration::featureAttributes.insert("measurement series",true);
    Configuration::featureAttributes.insert("comment",true);
}

/*!
 * \brief generateTrafoParamAttributes fills the QMap with all available attributes for trafo parameter.
 * Also includes a boolean that handles if the attributes is displayed or not.
 */
void Configuration::generateTrafoParamAttributes()
{
    Configuration::trafoParamAttributes.insert("feature type",true);
    Configuration::trafoParamAttributes.insert("feature name",true);
    Configuration::trafoParamAttributes.insert("group",true);
    Configuration::trafoParamAttributes.insert("solved",true);
    Configuration::trafoParamAttributes.insert("functions",true);
    Configuration::trafoParamAttributes.insert("start",true);
    Configuration::trafoParamAttributes.insert("destination",true);
    Configuration::trafoParamAttributes.insert("translation x",true);
    Configuration::trafoParamAttributes.insert("translation y",true);
    Configuration::trafoParamAttributes.insert("translation z",true);
    Configuration::trafoParamAttributes.insert("rotation x",true);
    Configuration::trafoParamAttributes.insert("rotation y",true);
    Configuration::trafoParamAttributes.insert("rotation z",true);
    Configuration::trafoParamAttributes.insert("scale x",true);
    Configuration::trafoParamAttributes.insert("scale y",true);
    Configuration::trafoParamAttributes.insert("scale z",true);
    Configuration::trafoParamAttributes.insert("comment",true);
}

/*!
 * \brief displayAttributes checks which attributes should be displayed. returns the column indices for this attributes.
 * \param attributes
 * \return
 */
QList<int> Configuration::displayAttributes(QMap<QString, bool> attributes)
{
    QList<int> display;

    QMapIterator<QString,bool>i(attributes);
    while(i.hasNext()){
        i.next();
        int res = Configuration::allAttributes.indexOf(i.key());
        if(res != -1){
            display.append(res);
        }
    }
    return display;
}
