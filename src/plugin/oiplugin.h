#ifndef PI_OIPLUGIN_H
#define PI_OIPLUGIN_H

#include "function.h"
#include "sensor.h"
#include "simulationmodel.h"
#include "networkadjustment.h"
#include "oitool.h"
#include "exchangesimpleascii.h"
#include "exchangedefinedformat.h"

class OiPlugin
{

public:
    virtual ~OiPlugin(){}

    virtual QList<Sensor*> createSensors() = 0;
    virtual QList<Function*> createFunctions() = 0;
    virtual QList<NetworkAdjustment*> createNetworkAdjustments() = 0;
    virtual QList<SimulationModel*> createSimulations() = 0;
    virtual QList<OiTool*> createTools() = 0;
    virtual QList<ExchangeSimpleAscii*> createSimpleAsciiExchanges() = 0;
    virtual QList<ExchangeDefinedFormat*> createDefinedFormatExchanges() = 0;

    virtual Sensor* createSensor(QString name) = 0;
    virtual Function* createFunction(QString name) = 0;
    virtual NetworkAdjustment* createNetworkAdjustment(QString name) = 0;
    virtual SimulationModel* createSimulation(QString name) = 0;
    virtual OiTool* createTool(QString name) = 0;
    virtual ExchangeSimpleAscii* createSimpleAsciiExchange(QString name) = 0;
    virtual ExchangeDefinedFormat* createDefinedFormatExchange(QString name) = 0;

};

#define OiPlugin_iidd "de.openIndy.Plugin.v001"

Q_DECLARE_INTERFACE(OiPlugin, OiPlugin_iidd)

#endif // PI_OIPLUGIN_H