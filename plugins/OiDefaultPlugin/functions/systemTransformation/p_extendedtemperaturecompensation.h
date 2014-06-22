#ifndef P_EXTENDEDTEMPERATURECOMPENSATION_H
#define P_EXTENDEDTEMPERATURECOMPENSATION_H

#include "pi_systemtransformation.h"
#include "configuration.h"
#include "pluginmetadata.h"
#include <QtCore/qmath.h>


/*!
 * \brief The ExtendedTemperatureCompensation class is a system transformation with temperature compensation.
 * By specifying a minimum of 3 common points between the systems, a reference temperature and the material, this
 * function calculates the scales (x,y,z) , translation and rotation
 */

using namespace std;

class ExtendedTemperatureCompensation : public SystemTransformation
{
public:
    ExtendedTemperatureCompensation();

    PluginMetaData* getMetaData();
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

    bool exec(TrafoParam &);

    QMap<QString, QStringList> getStringParameter();
    QMap<QString, double> getDoubleParameter();

    QStringList getResultProtocol();

private:

    QStringList protocol;

    bool svdError;

    QList<OiVec> locSystem;
    QList<OiVec> refSystem;

    OiVec translation;
    OiVec rotation;
    OiVec scale;

    double actTemp;
    double refTemp;
    double tempAccuracy;
    Configuration::Materials material;
    double expansionCoefficient;
    bool useTemp;

    bool calc(TrafoParam &tp);
    void init();
    void getExtraParameter();
    OiVec approxTranslation(OiVec scale, OiVec rot);
    //OiVec approxRotation();
    OiVec approxRotation();
    OiVec approxScale();
    OiVec calcCentroidPoint(QList<OiVec> points);
    OiVec fillLVector();
    OiMat fillAMatrix(OiVec x0);
    OiMat getRotationMatrix(OiVec rot);
    void preliminaryTransformation();
    OiMat scaleMatrix(OiVec s);
    OiVec fillL0Vector(OiVec x0);

    //for approx of rotation
    vector<OiVec> centroidReducedCoord(QList<OiVec> input, OiVec centroid);
    vector<OiMat> modelMatrix(vector<OiVec> locC, vector<OiVec> refC);
    OiMat normalEquationMatrix(vector<OiMat>vecA);
    OiVec quaternion(OiMat n);
    OiMat rotationMatrix(OiVec q);
    OiVec getRotationAngles(OiMat r);
};

#endif // P_EXTENDEDTEMPERATURECOMPENSATION_H
