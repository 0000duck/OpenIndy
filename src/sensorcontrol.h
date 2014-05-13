#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#include <QMutex>
#include <QDebug>
#include <QTime>
#include "connectionconfig.h"
#include "station.h"
#include "measurementconfig.h"
#include "oiemitter.h"
#include "sensorconfiguration.h"

class SensorListener;
class Station;

/*!
 * \brief The SensorControl class
 * the sensor control object belongs to a station and executes all sensor actions on a seperate thread.
 */
class SensorControl : public QObject
{
    Q_OBJECT

public:

    //constructor
    SensorControl(Station *st);
    ~SensorControl();

    //instrument
    Sensor *instrument;
    QList<Sensor*> usedSensors;
    SensorConfiguration *InstrumentConfig;
    SensorListener *instrumentListener;

    QThread listenerThread;

    bool isReadingStreamActive;
    bool isSensorStatStreamActive;

    //attributes
    double az;
    double ze;
    double dist;
    double x_;
    double y_;
    double z_;
    bool isMoveRelativ;
    bool isMState;
    QString cmd;

    OiEmitter& getOiEmitter();

signals:
    void commandFinished(bool);
    void recalcFeature(Feature*);
    void activateCheckStream();
    void activateStatStream();
    void activateReadingStream();


public slots:

    //measure
    void measure(Geometry *geom,bool isActiveCoordSys);

    //data stream
    void readingStream(Configuration::ReadingTypes streamFormat);
    void sensorStatsStream();

    //sensor actions
    void move(double, double, double, bool);
    void move(double, double, double);
    void initialize();
    void motorState();
    void home();
    void toggleSight();
    void connectSensor(ConnectionConfig *connConfig);
    void disconnectSensor();
    void compensation();
    void doSelfDefinedAction(QString s);


private:

    enum streamType{
        eCheckStream,
        eReadingStream,
        eSenorStats
    };

    Station *myStation;
    QMutex locker;
    OiEmitter myEmitter;

    void storeReadings(QList<Reading*>readings, Geometry* geom, bool isActiveCoordSys);
    void saveReading(Reading* r, Geometry* geom, bool isActiveCoordSys);

    void sendActivateCheckStream(streamType t);
    bool sendDeactivateCheckStream();



};

#endif // SENSORCONTROL_H
