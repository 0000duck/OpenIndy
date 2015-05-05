#include "station.h"

#include "coordinatesystem.h"
#include "sensorcontrol.h"
#include "reading.h"
#include "sensor.h"
#include "oijob.h"
/*

Station::Station(QString name)
{
    this->name = name;

    //this->id = Configuration::generateID();
    position = new Point(false);
    position->setFeatureName(name);

    //ini member
    coordSys = new CoordinateSystem();
    coordSys->setFeatureName(this->name);
    coordSys->isStationSystem = true;

    sensorPad = new SensorControl(this);
    connect(&sensorPad->getOiEmitter(), SIGNAL(sendString(QString)), this, SLOT(writeToConsole(QString)));

    //move controller to the station thread
    sensorPad->moveToThread(&stationThread);

    //connections between station and sensorcontrol
    connect(this->sensorPad,SIGNAL(commandFinished(bool)),this,SLOT(emitActionFinished(bool)));

    connect(this,SIGNAL(startMeasure(Geometry*,bool)),this->sensorPad,SLOT(measure(Geometry*,bool)));
    connect(this,SIGNAL(startMove(double,double,double,bool)),this->sensorPad,SLOT(move(double,double,double,bool)));
    connect(this,SIGNAL(startMove(double,double,double)),this->sensorPad,SLOT(move(double,double,double)));
    connect(this,SIGNAL(startInitialize()),this->sensorPad,SLOT(initialize()));
    connect(this,SIGNAL(startMotorState()),this->sensorPad,SLOT(motorState()));
    connect(this,SIGNAL(startHome()),this->sensorPad,SLOT(home()));
    connect(this,SIGNAL(startToggleSight()),this->sensorPad,SLOT(toggleSight()));
    connect(this,SIGNAL(startCompensation()),this->sensorPad,SLOT(compensation()));
    connect(this,SIGNAL(startConnect(ConnectionConfig*)),this->sensorPad,SLOT(connectSensor(ConnectionConfig*)));
    connect(this,SIGNAL(startDisconnect()),this->sensorPad,SLOT(disconnectSensor()));
    connect(this,SIGNAL(startReadingStream(int)),this->sensorPad,SLOT(readingStream(int)));
    connect(this,SIGNAL(startSensorStatsStream()),this->sensorPad,SLOT(sensorStatsStream()));
    connect(this,SIGNAL(startSelfDefinedAction(QString)),this->sensorPad,SLOT(doSelfDefinedAction(QString)));
    connect(this,SIGNAL(stopReadingStream()),this->sensorPad,SLOT(stopReadingStream()));
    connect(this,SIGNAL(stopSensorStatsStream()),this->sensorPad,SLOT(stopStatStream()));

    //start the station thread
    stationThread.start();

    this->isUpdated = false;
    this->isDrawn = true;
}

Station::Station(){

    //initialize position
    this->position = new Point(false);

    //initialize sensor control
    sensorPad = new SensorControl(this);
    connect(&sensorPad->getOiEmitter(), SIGNAL(sendString(QString)), this, SLOT(writeToConsole(QString)));

    //move controller to the station thread
    sensorPad->moveToThread(&stationThread);

    //connections between station and sensorcontrol
    connect(this->sensorPad,SIGNAL(commandFinished(bool)),this,SLOT(emitActionFinished(bool)));

    connect(this,SIGNAL(startMeasure(Geometry*,bool)),this->sensorPad,SLOT(measure(Geometry*,bool)));
    connect(this,SIGNAL(startMove(double,double,double,bool)),this->sensorPad,SLOT(move(double,double,double,bool)));
    connect(this,SIGNAL(startMove(double,double,double)),this->sensorPad,SLOT(move(double,double,double)));
    connect(this,SIGNAL(startInitialize()),this->sensorPad,SLOT(initialize()));
    connect(this,SIGNAL(startMotorState()),this->sensorPad,SLOT(motorState()));
    connect(this,SIGNAL(startHome()),this->sensorPad,SLOT(home()));
    connect(this,SIGNAL(startToggleSight()),this->sensorPad,SLOT(toggleSight()));
    connect(this,SIGNAL(startCompensation()),this->sensorPad,SLOT(compensation()));
    connect(this,SIGNAL(startConnect(ConnectionConfig*)),this->sensorPad,SLOT(connectSensor(ConnectionConfig*)));
    connect(this,SIGNAL(startDisconnect()),this->sensorPad,SLOT(disconnectSensor()));
    connect(this,SIGNAL(startReadingStream(int)),this->sensorPad,SLOT(readingStream(int)));
    connect(this,SIGNAL(startSensorStatsStream()),this->sensorPad,SLOT(sensorStatsStream()));
    connect(this,SIGNAL(startSelfDefinedAction(QString)),this->sensorPad,SLOT(doSelfDefinedAction(QString)));
    connect(this,SIGNAL(stopReadingStream()),this->sensorPad,SLOT(stopReadingStream()));
    connect(this,SIGNAL(stopSensorStatsStream()),this->sensorPad,SLOT(stopStatStream()));

    //start the station thread
    stationThread.start();

    this->isUpdated = false;

}

Station::~Station(){

    stationThread.quit();
    stationThread.wait();

    //delete corresponding coordinate system with all observations made from this station
    delete this->coordSys;

    //delete position of this station
    delete this->position;

    //TODO Sensorliste deleten
    delete this->sensorPad;

}

bool Station::getIsActiveStation(){
    return this->isActiveStation;
}

bool Station::setActiveStationState(bool isActiveStation){
    if(this->isActiveStation != isActiveStation){
        this->isActiveStation = isActiveStation;
        emit this->activeStationChanged(this->id);
        return true;
    }
    return false;
}

ReadingTypes Station::getReadingStreamType(){
    return this->sensorPad->getTypeOfReadingStream();
}

void Station::stopThread(){
    stationThread.quit();
    stationThread.wait();
}

void Station::startThread(){
    stationThread.start();
}

void Station::setInstrumentConfig(SensorConfiguration sensorConfig){

    sensorPad->InstrumentConfig = sensorConfig;
    sensorPad->instrumentListener->setInstrument(sensorConfig.mySensor);
    sensorPad->instrument->setSensorConfiguration(sensorConfig);

}

SensorConfiguration Station::getInstrumentConfig(){
    return sensorPad->InstrumentConfig;
}

void Station::recalc(){

}

QDomElement Station::toOpenIndyXML(QDomDocument &xmlDoc){

    QDomElement station = Feature::toOpenIndyXML(xmlDoc);

    if(station.isNull()){
        return station;
    }

    station.setTagName("station");

    //add used sensors
    if(this->sensorPad != NULL && this->sensorPad->usedSensors.size() > 0){
        QDomElement usedSensors = xmlDoc.createElement("usedSensors");
        foreach(Sensor *s, this->sensorPad->usedSensors){
            if(s != NULL){
                QDomElement sensor = s->toOpenIndyXML(xmlDoc);
                if(!sensor.isNull()){
                    usedSensors.appendChild(sensor);
                }
            }
        }
        station.appendChild(usedSensors);
    }

    //add active sensor
    if(this->sensorPad != NULL && this->sensorPad->instrument != NULL){
        QDomElement activeSensor = this->sensorPad->instrument->toOpenIndyXML(xmlDoc);
        if(!activeSensor.isNull()){
            activeSensor.setTagName("activeSensor");
            station.appendChild(activeSensor);
        }
    }

    //add position
    if(this->position != NULL){
        QDomElement position = xmlDoc.createElement("position");
        position.setAttribute("ref", this->position->getId());
        station.appendChild(position);
    }

    //add coordinate system
    if(this->coordSys != NULL){
        QDomElement stationSystem = xmlDoc.createElement("coordinateSystem");
        stationSystem.setAttribute("ref", this->coordSys->getId());
        station.appendChild(stationSystem);
    }

    return station;

}

bool Station::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);

    if(result){

        //set station attributes
        this->isActiveStation = false;

    }

    return result;
}

void Station::emitActionFinished(bool wasSuccesful){

    emit actionFinished(wasSuccesful);
}

void Station::emitStartReadingStream(int readingType) const{
    emit startReadingStream(readingType);
}

void Station::emitStopReadingStream() const{
    emit stopReadingStream();
}

void Station::emitStartSensorStatsStream() const
{
    emit startSensorStatsStream();
}

void Station::emitStopSensorStatsStream() const
{
    emit stopSensorStatsStream();
}

void Station::emitStartMeasure(Geometry *geom, bool isActiveCoordSys) const{
    emit startMeasure(geom,isActiveCoordSys);
}

void Station::emitStartMove(double azimuth, double zenith, double dist, bool isRelativ) const{
    emit startMove(azimuth, zenith, dist, isRelativ);
}

void Station::emitStartMove(double x, double y, double z) const{
    emit startMove(x, y, z);
}

void Station::emitStartInitialize() const{
    emit startInitialize();
}

void Station::emitStartMotorState() const{
    emit startMotorState();
}

OiSensorEmitter* Station::getActiveSensorEmitter()
{
    return &this->sensorPad->instrument->myEmitter;
}

void Station::emitStartCompensation() const{
    emit startCompensation();
}

void Station::emitSelfDefinedAction(QString s) const
{
    emit startSelfDefinedAction(s);
}

void Station::emitStartHome() const{
    emit startHome();
}

void Station::emitStartToggleSight() const{
    emit startToggleSight();
}

void Station::emitStartConnect(ConnectionConfig *cConfig) const{
    emit startConnect(cConfig);
}

void Station::emitStartDisconnect() const{
    emit startDisconnect();
}

QString Station::getDisplayX() const{
    //return QString::number(this->position->xyz.getAt(0)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    return "";
    //return QString::number(this->coordSys->origin.getAt(0)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString Station::getDisplayY() const{
    return "";
    //return QString::number(this->position->xyz.getAt(1)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    //return QString::number(this->coordSys->origin.getAt(1)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString Station::getDisplayZ() const{
    //return QString::number(this->position->xyz.getAt(2)*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
    return "";
    //return QString::number(this->coordSys->origin.getAt(2)*OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
}

QString Station::getDisplayIsCommon() const{
    return QString(position->getIsCommon()?"true":"false");
}

QString Station::getDisplayIsNominal() const{
    return QString(position->getIsNominal()?"nominal":"actual");
}

QString Station::getDisplaySolved() const{
    return QString(this->isSolved?"true":"false");
}

QString Station::getDisplayMConfig() const{
    return this->position->getMeasurementConfig().getDisplayName();
}

QString Station::getDisplayStdDev() const{

    if(this->position->getStatistic().isValid){
        //return QString::number(this->position->getStatistic().stdev * OiUnitConverter::getDistanceMultiplier(),'f',OiUnitConverter::distanceDigits);
    }
    return "-/-";
}

void Station::writeToConsole(QString msg){
    emit this->sendToConsole(msg);
}
*/

#include "featurewrapper.h"

/*!
 * \brief Station::Station
 * \param parent
 */
Station::Station(QObject *parent) : Feature(parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    //create a point object as the station's position
    this->position = new Point(false);

    //create a coordinate system object as the station's coordinate system
    this->stationSystem = new CoordinateSystem(QPointer<Station>(this));
    QObject::connect(this, SIGNAL(featureNameChanged(const int&, const QString&)), this, SLOT(stationNameChanged(const int&, const QString&)), Qt::DirectConnection);

    //create a sensor control object, connect it and move it to thread
    QPointer<Station> stationPointer(this);
    this->sensorControl = new SensorControl(stationPointer);
    this->connectSensorControl();
    this->sensorControl->moveToThread(&this->stationThread);
    this->stationThread.start();

}

/*!
 * \brief Station::Station
 * \param name
 * \param parent
 */
Station::Station(const QString &name, QObject *parent) : Feature(parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    this->name = name;

    //create a point object as the station's position
    this->position = new Point(false);
    this->position->setFeatureName(name);

    //create a coordinate system object as the station's coordinate system
    this->stationSystem = new CoordinateSystem(QPointer<Station>(this));
    this->stationSystem->setFeatureName(this->name);
    QObject::connect(this, SIGNAL(featureNameChanged(const int&, const QString&)), this, SLOT(stationNameChanged(const int&, const QString&)), Qt::DirectConnection);

    //create a sensor control object, connect it and move it to thread
    QPointer<Station> stationPointer(this);
    this->sensorControl = new SensorControl(stationPointer);
    this->connectSensorControl();
    this->sensorControl->moveToThread(&this->stationThread);
    this->stationThread.start();

}

/*!
 * \brief Station::Station
 * \param copy
 * \param parent
 */
Station::Station(const Station &copy, QObject *parent) : Feature(copy, parent){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    //copy general station attributes
    this->isActiveStation = copy.isActiveStation;
    if(!copy.position.isNull()){
        this->position = new Point(*copy.position.data());
    }

    //copy measured readings
    this->cartesianReadings = copy.cartesianReadings;
    this->directionReadings = copy.directionReadings;
    this->distanceReadings = copy.distanceReadings;
    this->polarReadings = copy.polarReadings;
    this->levelReadings = copy.levelReadings;
    this->temperatureRadings = copy.temperatureRadings;
    this->undefinedReadings = copy.undefinedReadings;

}

/*!
 * \brief Station::operator =
 * \param copy
 * \return
 */
Station &Station::operator=(const Station &copy){

    //set up feature wrapper
    if(!this->selfFeature.isNull()){
        this->selfFeature->setStation(this);
    }

    //copy general station attributes
    this->isActiveStation = copy.isActiveStation;
    if(!copy.position.isNull()){
        this->position = new Point(*copy.position.data());
    }

    //copy measured readings
    this->cartesianReadings = copy.cartesianReadings;
    this->directionReadings = copy.directionReadings;
    this->distanceReadings = copy.distanceReadings;
    this->polarReadings = copy.polarReadings;
    this->levelReadings = copy.levelReadings;
    this->temperatureRadings = copy.temperatureRadings;
    this->undefinedReadings = copy.undefinedReadings;

    return *this;

}

/*!
 * \brief Station::~Station
 */
Station::~Station(){

    this->stationThread.quit();
    this->stationThread.wait();

    //delete corresponding coordinate system with all observations made from this station
    if(!this->stationSystem.isNull()){
        delete this->stationSystem;
    }

    //delete position of this station
    if(this->position.isNull()){
        delete this->position;
    }

    //delete sensor control
    if(!this->sensorControl.isNull()){
        delete this->sensorControl;
    }

}

/*!
 * \brief Station::getIsActiveStation
 * \return
 */
const bool &Station::getIsActiveStation() const{
    return this->isActiveStation;
}

/*!
 * \brief Station::setActiveStationState
 * \param isActiveStation
 */
void Station::setActiveStationState(const bool &isActiveStation){
    if(this->isActiveStation != isActiveStation){
        this->isActiveStation = isActiveStation;
        emit this->activeStationChanged(this->id);
    }
}

/*!
 * \brief Station::getPosition
 * \return
 */
const QPointer<Point> &Station::getPosition() const{
    return this->position;
}

/*!
 * \brief Station::getCoordinateSystem
 * \return
 */
const QPointer<CoordinateSystem> &Station::getCoordinateSystem() const{
    return this->stationSystem;
}

/*!
 * \brief Station::getActiveSensorType
 * \return
 */
SensorTypes Station::getActiveSensorType() const{

    //check sensor control
    if(this->sensorControl.isNull() || this->sensorControl->getSensor().isNull()){
        return eUndefinedSensor;
    }

    SensorTypes sensorType = this->sensorControl->getSensor()->getSensorConfiguration().getTypeOfSensor();

    return sensorType;

}

/*!
 * \brief Station::setSensor
 * \param sensor
 */
void Station::setSensor(const QPointer<Sensor> &sensor){
    if(!sensor.isNull() && !this->sensorControl.isNull()){
        this->sensorControl->setSensor(sensor);
        emit this->sensorChanged(this->id);
    }
}

/*!
 * \brief Station::getSensorListener
 * \return
 */
const QPointer<const SensorListener> Station::getSensorListener() const{
    if(!this->sensorControl.isNull()){
        return this->sensorControl->getSensorListener();
    }
    return QPointer<const SensorListener>();
}

/*!
 * \brief Station::recalc
 */
void Station::recalc(){
    Feature::recalc();
}

/*!
 * \brief Station::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Station::toOpenIndyXML(QDomDocument &xmlDoc){

    QDomElement station = Feature::toOpenIndyXML(xmlDoc);

    /*if(station.isNull()){
        return station;
    }

    station.setTagName("station");

    //add used sensors
    if(this->sensorPad != NULL && this->sensorPad->usedSensors.size() > 0){
        QDomElement usedSensors = xmlDoc.createElement("usedSensors");
        foreach(Sensor *s, this->sensorPad->usedSensors){
            if(s != NULL){
                QDomElement sensor = s->toOpenIndyXML(xmlDoc);
                if(!sensor.isNull()){
                    usedSensors.appendChild(sensor);
                }
            }
        }
        station.appendChild(usedSensors);
    }

    //add active sensor
    if(this->sensorPad != NULL && this->sensorPad->instrument != NULL){
        QDomElement activeSensor = this->sensorPad->instrument->toOpenIndyXML(xmlDoc);
        if(!activeSensor.isNull()){
            activeSensor.setTagName("activeSensor");
            station.appendChild(activeSensor);
        }
    }

    //add position
    if(this->position != NULL){
        QDomElement position = xmlDoc.createElement("position");
        position.setAttribute("ref", this->position->getId());
        station.appendChild(position);
    }

    //add coordinate system
    if(this->coordSys != NULL){
        QDomElement stationSystem = xmlDoc.createElement("coordinateSystem");
        stationSystem.setAttribute("ref", this->coordSys->getId());
        station.appendChild(stationSystem);
    }*/

    return station;

}

/*!
 * \brief Station::fromOpenIndyXML
 * \param xmlElem
 * \return
 */
bool Station::fromOpenIndyXML(QDomElement &xmlElem){

    bool result = Feature::fromOpenIndyXML(xmlElem);

    if(result){

        //set station attributes
        this->isActiveStation = false;

    }

    return result;

}

/*!
 * \brief Station::getDisplayX
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayX(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->position.isNull()){
        return this->position->getDisplayX(type, digits, showDiff);
    }
    return QString("-/-");
}

/*!
 * \brief Station::getDisplayY
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayY(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->position.isNull()){
        return this->position->getDisplayY(type, digits, showDiff);
    }
    return QString("-/-");
}

/*!
 * \brief Station::getDisplayZ
 * \param type
 * \param digits
 * \param showDiff
 * \return
 */
QString Station::getDisplayZ(const UnitType &type, const int &digits, const bool &showDiff) const{
    if(this->position.isNull()){
        return this->position->getDisplayZ(type, digits, showDiff);
    }
    return QString("-/-");
}

/*!
 * \brief Station::setUpFeatureId
 * Generate a new unique id when the current job was set
 */
void Station::setUpFeatureId(){

    //check job
    if(this->job.isNull()){
        return;
    }

    //generate unique id for station's position
    if(!this->position.isNull()){
        this->position->id = this->job->generateUniqueId();
    }

    //generate unique id for station's coordinate system
    if(!this->stationSystem.isNull()){
        this->stationSystem->id = this->job->generateUniqueId();
    }

    //generate unique ids for station's readings
    foreach(const QPointer<Reading> &reading, this->cartesianReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->polarReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->directionReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->distanceReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->temperatureRadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->levelReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }
    foreach(const QPointer<Reading> &reading, this->undefinedReadings){
        if(!reading.isNull()){
            reading->id = this->job->generateUniqueId();
        }
    }

    //feature specific
    Feature::setUpFeatureId();

}

/*!
 * \brief Station::stationNameChanged
 * Update the station system name whenever the station's name has changed
 * \param featureId
 * \param oldName
 */
void Station::stationNameChanged(const int &featureId, const QString &oldName){
    if(!this->stationSystem.isNull()){
        this->stationSystem->setFeatureName(this->name);
    }
}

/*!
 * \brief Station::connectSensorControl
 */
void Station::connectSensorControl(){

    //connect sensor actions
    QObject::connect(this, &Station::connectSensor, this->sensorControl.data(), &SensorControl::connectSensor, Qt::AutoConnection);
    QObject::connect(this, &Station::disconnectSensor, this->sensorControl.data(), &SensorControl::disconnectSensor, Qt::AutoConnection);

    qRegisterMetaType<MeasurementConfig>();
    //void (Station:: *measureSignal)(const int &geomId) = &Station::measure;
    //void (SensorControl:: *measureSlot)(const int &geomId) = &SensorControl::measure;
    //QObject::connect(this, measureSignal, this->sensorControl.data(), measureSlot);
    //QObject::connect(this, SIGNAL(measure(int,MeasurementConfig)), this->sensorControl.data(), SLOT(measure(int,MeasurementConfig)), Qt::AutoConnection);
    QObject::connect(this, &Station::measure, this->sensorControl.data(), &SensorControl::measure, Qt::AutoConnection);

    void (Station:: *movePolarSignal)(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative) = &Station::move;
    void (SensorControl:: *movePolarSlot)(const double &azimuth, const double &zenith, const double &distance, const bool &isRelative) = &SensorControl::move;
    QObject::connect(this, movePolarSignal, this->sensorControl.data(), movePolarSlot);

    void (Station:: *moveCartesianSignal)(const double &x, const double &y, const double &z) = &Station::move;
    void (SensorControl:: *moveCartesianSlot)(const double &x, const double &y, const double &z) = &SensorControl::move;
    QObject::connect(this, moveCartesianSignal, this->sensorControl.data(), moveCartesianSlot);

    QObject::connect(this, &Station::initialize, this->sensorControl.data(), &SensorControl::initialize, Qt::AutoConnection);
    QObject::connect(this, &Station::motorState, this->sensorControl.data(), &SensorControl::motorState, Qt::AutoConnection);
    QObject::connect(this, &Station::home, this->sensorControl.data(), &SensorControl::home, Qt::AutoConnection);
    QObject::connect(this, &Station::toggleSight, this->sensorControl.data(), &SensorControl::toggleSight, Qt::AutoConnection);
    QObject::connect(this, &Station::compensation, this->sensorControl.data(), &SensorControl::compensation, Qt::AutoConnection);
    QObject::connect(this, &Station::selfDefinedAction, this->sensorControl.data(), &SensorControl::selfDefinedAction, Qt::AutoConnection);

    //connect sensor action results
    QObject::connect(this->sensorControl.data(), &SensorControl::commandFinished, this, &Station::commandFinished, Qt::AutoConnection);
    QObject::connect(this->sensorControl.data(), &SensorControl::measurementFinished, this, &Station::measurementFinished, Qt::AutoConnection);

}
