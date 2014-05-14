#ifndef P_LEICATACHYMETER_H
#define P_LEICATACHYMETER_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QRegExp>
#include <QThread>
#include <QStringList>
#include <QVariantMap>
#include "pi_totalstation.h"


class LeicaTachymeter : public TotalStation
{

    
public:
    LeicaTachymeter();

public:

    QList<Configuration::ReadingTypes>* getSupportedReadingTypes();
    QList<Configuration::SensorFunctionalities> getSupportedSensorActions();
    QList<Configuration::ConnectionTypes>* getConnectionType();

    PluginMetaData* getMetaData();

    QMap<QString,int>* getIntegerParameter();
    QMap<QString,double>* getDoubleParameter();
    QMap <QString, QStringList>* getStringParameter();
    QStringList selfDefinedActions();
    bool doSelfDefinedAction(QString a);

    /*default accuracy
     *keys:
     * sigmaAzimuth sigmaAzimuth sigmaDistance
     * sigmaXyz
     * sigmaTempDeg
     * sigmaAngleXZ
     * sigmaAngleYZ
     */
    QMap<QString,double>* getDefaultAccuracy();

    //! abort a running action
    void abortAction();

    //! connect to sensor
    virtual bool connectSensor(ConnectionConfig* connConfig);

    //! disconnect to sensor
    virtual bool disconnectSensor();

    //! measure
    QList<Reading*> measure(MeasurementConfig* mc);

    //! stream
    QVariantMap readingStream(Configuration::ReadingTypes streamFormat);

    //! getConnectionState
    bool getConnectionState();

    //! return ready state of the sensor
    bool isReadyForMeasurement();

    //!sensor stats
    QMap<QString,QString> getSensorStats();

    //!checks if sensor is busy
    bool isBusy();

protected:

    //! move totalstation to specified position
    bool move(double azimuth, double zenith, double distance,bool isrelativ);

    //! toggle between frontside and backside
    bool toggleSightOrientation();

private:

    QSerialPort *serial;


    QList<Reading*> measurePolar(MeasurementConfig*);
    QList<Reading*> measureDistance(MeasurementConfig*);
    QList<Reading*> measureDirection(MeasurementConfig*);
    QList<Reading*> measureCartesian(MeasurementConfig*);

    QSerialPort::BaudRate myBaudRate;
    QSerialPort::DataBits myDataBits;
    QSerialPort::Parity myParity;
    QSerialPort::StopBits myStopBits;
    QSerialPort::FlowControl myFlowControl;

    QString receive();
    void getError(QSerialPort::SerialPortError);
    bool executeEDM();
    bool executeCommand(QString);

};

#endif // P_LEICATACHYMETER_H
