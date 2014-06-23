#ifndef P_SIMPLETEMPERATURECOMPENSATION_H
#define P_SIMPLETEMPERATURECOMPENSATION_H

#include "pi_systemtransformation.h"
#include "configuration.h"
#include "pluginmetadata.h"

/*!
 * \brief The SimpleTemperatureCompensation class is a system transformation for temperature compensation.
 * By giving it a reference temperature, a actual temperature and a material it will calculate the compensation values.
 * They are equal for x,y and z component.
 */
class SimpleTemperatureCompensation : public SystemTransformation
{
public:
    SimpleTemperatureCompensation();

    PluginMetaData* getMetaData();
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

    bool exec(TrafoParam &);

    QMap<QString, QStringList> getStringParameter();
    QMap<QString, double> getDoubleParameter();

    void calcExpansion(TrafoParam &tp, ScalarEntityTemperature* SET);
    void calcAccuracy(TrafoParam &tp, double tempAccuracy, double expansion);

    QStringList getResultProtocol();

    ScalarEntityTemperature* getScalarEntityTemperature();

private:
    QString protActTemp;
    QString protRefTemp;
    QString protMaterial;
    QString protExpansionCoeff;
    QString protExpansion;
    QString protTempAccuracy;
    QString protSTDDEV;
};

#endif // P_SIMPLETEMPERATURECOMPENSATION_H
