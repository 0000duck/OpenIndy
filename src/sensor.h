#ifndef SENSOR_H
#define SENSOR_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include "sensorcontrol.h"
#include "oisensoremitter.h"
#include "pluginmetadata.h"
#include "sensorconfiguration.h"

class Reading;
class MeasurementConfig;
class SensorControl;

/*!
 * \brief The Sensor class
 * Father class for all sensor types. The Sensor has four important methodes.
 * The measure methode execute a measurement defined by the MeasurementConfig
 * Object. The accept methode execute an sensor action defined by the Configuration
 * Enumeration.
 *
 */
class Sensor
{
public:
//constructor
    virtual ~Sensor(){}

//attributes
    OiSensorEmitter myEmitter;
    SensorConfiguration myConfiguration;

//methods

    //-----get sensor capabilities-----

    virtual QList<Configuration::ReadingTypes>* getSupportedReadingTypes() const = 0;
    virtual QList<Configuration::SensorFunctionalities> getSupportedSensorActions() const = 0;
    virtual QList<Configuration::ConnectionTypes>* getConnectionType() const = 0;

    //! get meta data
    virtual PluginMetaData* getMetaData() const = 0;

    //individually defined sensor parameter
    virtual QMap<QString,int>* getIntegerParameter() const = 0;
    virtual QMap<QString,double>* getDoubleParameter() const = 0;
    virtual QMap <QString, QStringList>* getStringParameter() const = 0;
    virtual QStringList selfDefinedActions() const = 0;
    virtual bool doSelfDefinedAction(QString a) = 0;

    //individually defined reading type
    virtual QString getUndefinedReadingName() const {return "undefined";}
    virtual QMap<QString,double>* getDefaultAccuracy() const = 0;

    //! sensor configuration
    void setSensorConfiguration(SensorConfiguration sConfig){
        myConfiguration = sConfig;
    }

    //-----sensor actions-----

    /*! checks given sensor functionality and nd calls the appropriate internal method
     * of the instance*/
    virtual bool accept(SensorControl*, Configuration::SensorFunctionalities) = 0;

    //! abort a running action
    virtual void abortAction() = 0;

    //! connect app with sensor
    virtual bool connectSensor(ConnectionConfig*) = 0;

    //! disconnect app with sensor
    virtual bool disconnectSensor() = 0;

    //! sensor measures a entity and returns a list of readings
    virtual QList<Reading*> measure(MeasurementConfig*) = 0;

    //! stream
    virtual QVariantMap readingStream(Configuration::ReadingTypes streamFormat) = 0;

    //! getConnectionState
    virtual bool getConnectionState() = 0;

    //! return ready state of the sensor
    virtual bool isReadyForMeasurement() = 0;

    //!sensor stats
    virtual QMap<QString,QString> getSensorStats()=0;

    //!checks if sensor is busy
    virtual bool isBusy() = 0;

    virtual QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const
    {

        if(xmlDoc.isNull() || this->getMetaData() == NULL){
            return QDomElement();
        }

        QDomElement sensor = xmlDoc.createElement("sensor");

        //add sensor attributes
        sensor.setAttribute("name", this->getMetaData()->name);
        sensor.setAttribute("plugin", this->getMetaData()->pluginName);

        //add reading types
        QDomElement readingTypes = xmlDoc.createElement("readingTypes");
        if(this->getSupportedReadingTypes() != NULL){
            QList<Configuration::ReadingTypes> *types = this->getSupportedReadingTypes();
            for(int i = 0; i < types->size(); i++){
                QDomElement readingType = xmlDoc.createElement("type");
                readingType.setAttribute("name", Configuration::getReadingTypeString(types->at(i)));
                readingTypes.appendChild(readingType);
            }
        }
        sensor.appendChild(readingTypes);

        return sensor;

    }

protected:
    void writeToConsole(QString s){myEmitter.emitSendString(s);}

};

#define Sensor_iidd "de.openIndy.Plugin.Sensor.v001"

#endif // SENSOR_H
