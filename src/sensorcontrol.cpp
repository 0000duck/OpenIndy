#include "sensorcontrol.h"
#include "function.h"
#include "sensorlistener.h"

#include "oimetadata.h"

SensorControl::SensorControl(Station *st)
{

    myStation = st;

    instrument = NULL;
    InstrumentConfig = NULL;
    instrumentListener = new SensorListener(instrument);

    connect(this,SIGNAL(activateCheckStream()),this->instrumentListener,SLOT(sensorCheckStream()));

    instrumentListener->moveToThread(&listenerThread);

    listenerThread.start();

}

SensorControl::~SensorControl()
{

}

/*!
 * \brief SensorControl::measure
 * \param mConfig
 * \param geom
 */
void SensorControl::measure(Geometry* geom,bool isActiveCoordSys){

    locker.lock();

    QList<Reading*> readings = instrument->measure(&(geom->mConfig));

    qDebug() << "count readings" << readings.size();
    if(readings.size() == 0){
        this->myEmitter.sendString("measurement not valid!");
        emit commandFinished(false);
    }else{
        this->storeReadings(readings,geom, isActiveCoordSys);
        emit commandFinished(true);
        emit this->recalcFeature(geom);
    }

    locker.unlock();

}

void SensorControl::readingStream(Configuration::ReadingTypes streamFormat){

    this->myEmitter.sendString("starting reading stream!");

    while(isReadingStreamActive){
      QVariantMap m = instrument->readingStream(streamFormat);
    }

}

void SensorControl::sensorStatsStream()
{
}


/*!
 * \brief SensorControl::move
 * \param azimuth
 * \param zenith
 * \param distance
 * \param isRelative
 */
void SensorControl::move(double azimuth, double zenith, double distance, bool isRelative){

    locker.lock();

    this->myEmitter.sendString("start move command");

    this->az =azimuth;
    this->ze= zenith;
    this->dist = distance;
    this->isMoveRelativ = isRelative;

    bool wasSuccessful = instrument->accept(this, Configuration::eMoveAngle);

    emit commandFinished(wasSuccessful);

    locker.unlock();

}

/*!
 * \brief SensorControl::move
 * \param x
 * \param y
 * \param z
 */
void SensorControl::move(double x, double y, double z){

    locker.lock();

    this->myEmitter.sendString("start move command");

    this->x_ =x;
    this->y_ =y;
    this->z_ =z;

    bool wasSuccessful = instrument->accept(this, Configuration::eMoveXYZ);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::initialize
 */
void SensorControl::initialize(){

    locker.lock();

    this->myEmitter.sendString("start initialize");

    bool wasSuccessful = instrument->accept(this, Configuration::eInitialize);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::motorState
 */
void SensorControl::motorState(){

    locker.lock();

    this->myEmitter.sendString("changing motor state");

    bool wasSuccessful = instrument->accept(this, Configuration::eMotorState);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::home
 */
void SensorControl::home(){

    locker.lock();

    this->myEmitter.sendString("set home");

    bool wasSuccessful = instrument->accept(this, Configuration::eHome);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::toggleSight
 */
void SensorControl::toggleSight(){

    locker.lock();

    this->myEmitter.sendString("toggleSight");

    bool wasSuccessful =  instrument->accept(this, Configuration::eToggleSight);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::compensation
 */
void SensorControl::compensation(){

    locker.lock();

    this->myEmitter.sendString("start compensation");

    bool wasSuccessful =  instrument->accept(this, Configuration::eCompensation);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

void SensorControl::doSelfDefinedAction(QString s)
{
    locker.lock();

    bool wasSuccessful =  instrument->doSelfDefinedAction(s);

    emit commandFinished(wasSuccessful);

    locker.unlock();

}


/*!
 * \brief SensorControl::connect
 */
void SensorControl::connectSensor(ConnectionConfig *connConfig){

    locker.lock();

    this->myEmitter.sendString("connecting sensor");

    bool wasSuccessful = instrument->connectSensor(connConfig);

    emit commandFinished(wasSuccessful);

    locker.unlock();
}

/*!
 * \brief SensorControl::disconnect
 */
void SensorControl::disconnectSensor(){

    locker.lock();

    this->myEmitter.sendString("disconnecting sensor");

    bool wasSuccessful = instrument->disconnectSensor();

    emit commandFinished(wasSuccessful);

    locker.unlock();
}


    void SensorControl::storeReadings(QList<Reading*>readings, Geometry* geom, bool isActiveCoordSys){

        for(int i = 0; i < readings.size();i++){
            this->saveReading(readings.at(i),geom,isActiveCoordSys);
        }

    }

/*!
 * \brief SensorControl::saveReading
 * \param r
 * \param mConfig
 * \param geom
 *
 * transforms the reading into an observation and saves the observation in the station and
 * given geometry
 */
void SensorControl::saveReading(Reading* r, Geometry* geom, bool isActiveCoordSys){

    r->id = Configuration::generateID();
    r->measuredAt = QDateTime::currentDateTime();

    switch(geom->mConfig.typeOfReading){
        case(Configuration::ePolar) :{
            //set type
            r->typeofReading = Configuration::ePolar;
            //set accuracy
            r->rPolar.sigmaAzimuth = myStation->getInstrumentConfig()->sigma.sigmaAzimuth;
            r->rPolar.sigmaZenith= myStation->getInstrumentConfig()->sigma.sigmaZenith;
            r->rPolar.sigmaDistance = myStation->getInstrumentConfig()->sigma.sigmaDistance;
            //store reading in station
            this->myStation->readingsPol.append(r);
            //create observation
            Observation *obs = new Observation(r,myStation);
            //check if the active coordSys is the station sys
            if(isActiveCoordSys){
                obs->isValid = true;
            }else{
                obs->isValid = false;
            }
            //calc obs Xyz
            obs->myOriginalXyz = Reading::toCartesian(r->rPolar.azimuth,r->rPolar.zenith,r->rPolar.distance);
            obs->myXyz = Reading::toCartesian(r->rPolar.azimuth,r->rPolar.zenith,r->rPolar.distance);
            //calc sigma xyz
            obs->sigmaXyz = r->errorPropagationPolarToCart();
            //save geometry in observation
            obs->myTargetGeometries.append(geom);
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }

            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::ePolar,Configuration::sPolar);
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

            break;
        }
        case(Configuration::eDistance) :{
            //set type
            r->typeofReading = Configuration::eDistance;
            //set accuracy
            r->rDistance.sigmaDistance = myStation->getInstrumentConfig()->sigma.sigmaDistance;
            //store reading in station
            this->myStation->readingsDist.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation);
            //check if the active coordSys is the station sys
            obs->isValid = false;
            //save geometry in observation
            obs->myTargetGeometries.append(geom);
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }
            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::eDistance,Configuration::sDistance);
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

            break;
        }
        case(Configuration::eDirection) :{
            //set type
            r->typeofReading =Configuration::eDirection;
            //set accuracy
            r->rDirection.sigmaAzimuth = myStation->getInstrumentConfig()->sigma.sigmaAzimuth;
            r->rDirection.sigmaZenith= myStation->getInstrumentConfig()->sigma.sigmaZenith;
            //store reading in station
            this->myStation->readingsDir.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation);
            //check if the active coordSys is the station sys
            obs->isValid = false;
            //save geometry in observation
            obs->myTargetGeometries.append(geom);
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }
            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::eDirection,Configuration::sDirection);
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

            break;
        }
        case(Configuration::eCartesian) :{
            //set type
            r->typeofReading = Configuration::eCartesian;
            //set accuracy
            r->rCartesian.sigmaXyz = myStation->getInstrumentConfig()->sigma.sigmaXyz;
            //store reading in station
            this->myStation->readingsXyz.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation);
            //check if the active coordSys is the station sys
            if(isActiveCoordSys){
                obs->isValid = true;
            }else{
                obs->isValid = false;
            }
            //save geometry in observation
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }
            obs->myTargetGeometries.append(geom);
            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::eCartesian,Configuration::sCartesian);
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

            break;
        }
        case(Configuration::eLevel) :{
            //set type
            r->typeofReading = Configuration::eLevel;
            //store reading in station
            this->myStation->readingsLevel.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation);
            //check if the active coordSys is the station sys
            obs->isValid = false;
            //save geometry in observation
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }
            obs->myTargetGeometries.append(geom);
            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::eLevel,Configuration::sLevel);
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

        break;
    }
    case(Configuration::eTemperatur) :{
        //set type
        r->typeofReading = Configuration::eTemperatur;
        //store reading in station
        this->myStation->readingsTemperatur.append(r);
        //create observation
        Observation *obs = new Observation(r, myStation);
        //check if the active coordSys is the station sys
        obs->isValid = false;
        //save geometry in observation
        //add observation to fit function of geom
        if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
            geom->functionList.at(0)->addObservation(obs, 0);
        }
        obs->myTargetGeometries.append(geom);
        //save observation in geometry
        geom->myObservations.append(obs);
        geom->insertReadingType(Configuration::eTemperatur,Configuration::sTemperatur);
        //save observation in station
        this->myStation->coordSys->observations.append(obs);

    break;
    }
    case(Configuration::eUndefined) :{
            //set type
            r->typeofReading = Configuration::eUndefined;
            //store reading in station
            this->myStation->readingsUndefined.append(r);
            //create observation
            Observation *obs = new Observation(r, myStation);
            //check if the active coordSys is the station sys
            obs->isValid = false;
            //save geometry in observation
            //add observation to fit function of geom
            if(geom->functionList.size() > 0 && geom->functionList.at(0)->getMetaData()->iid == OiMetaData::iid_FitFunction){
                geom->functionList.at(0)->addObservation(obs, 0);
            }
            obs->myTargetGeometries.append(geom);
            //save observation in geometry
            geom->myObservations.append(obs);
            geom->insertReadingType(Configuration::eUndefined, instrument->getUndefinedReadingName());
            //save observation in station
            this->myStation->coordSys->observations.append(obs);

    break;
    }
    }


}

void SensorControl::sendActivateCheckStream(streamType t)
{
    instrumentListener->isCheckStreamActive = true;

    switch (t) {
    case eCheckStream:
        emit activateCheckStream();
        break;
    case eReadingStream:
        emit activateReadingStream();
        break;
    case eSenorStats:
        emit activateStatStream();
        break;
    default:
        break;
    }


}

bool SensorControl::sendDeactivateCheckStream()
{
    instrumentListener->isCheckStreamActive = false;

    QTime timer;

    while(!instrumentListener->checkStreamFinished){

        if(timer.elapsed()>30000){
            listenerThread.quit();
            listenerThread.wait();
            listenerThread.start();
            this->myEmitter.sendString("timeout");
            return false;
        }
    }

    if(!instrument->getConnectionState()){
        this->myEmitter.sendString("sensor not connected");
        return false;
    }else{
        if(instrument->isBusy()){
          this->myEmitter.sendString("sensor is busy");
          return false;
        }
        if(!instrument->isReady()){
          this->myEmitter.sendString("sensor not ready");
          return false;
        }
    }

    return true;
}



OiEmitter& SensorControl::getOiEmitter(){
    return this->myEmitter;
}
