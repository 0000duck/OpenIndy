#include "controller.h"

/*!
 * \brief Controller::Controller
 * \param parent
 */
Controller::Controller(QObject *parent) : QObject(parent){

    //register meta types
    this->registerMetaTypes();

    //initialize model manager
    ModelManager::init();

    //initialize display configs
    this->initDisplayConfigs();

    //initialize config manager
    this->initConfigManager();

    //connect helper objects
    this->connectDataExchanger();
    this->connectFeatureUpdater();

}

/*!
 * \brief Controller::getAvailableTools
 * \return
 */
QList<QPointer<Tool> > Controller::getAvailableTools() const{

    QList<QPointer<Tool> > resultSet;

    //get available tools from database
    QList<sdb::Tool> toolPlugins = SystemDbManager::getTools();

    //load all tool plugins and add them to the result list
    foreach(const sdb::Tool &tool, toolPlugins){

        QPointer<Tool> toolPlugin = PluginLoader::loadToolPlugin(tool.plugin.file_path, tool.name);
        if(!toolPlugin.isNull()){
            resultSet.append(toolPlugin);
        }

    }

    return resultSet;

}

/*!
 * \brief Controller::addFeatures
 * \param attributes
 */
void Controller::addFeatures(const FeatureAttributes &attributes){

    //check job
    if(this->job.isNull()){
        Console::getInstance()->addLine("No active job");
        return;
    }

    QList<QPointer<FeatureWrapper> > features = this->job->addFeatures(attributes);

    //create functions for the created features
    if(attributes.functionPlugin.first.compare("") == 0 || attributes.functionPlugin.second.compare("") == 0){
        return;
    }
    foreach(const QPointer<FeatureWrapper> &feature, features){

        if(feature.isNull() || feature->getFeature().isNull()){
            continue;
        }

        //load function plugin
        QPointer<Function> function = PluginLoader::loadFunctionPlugin(attributes.functionPlugin.second, attributes.functionPlugin.first);
        if(function.isNull()){
            return;
        }

        //assign function to feature
        feature->getFeature()->blockSignals(true);
        feature->getFeature()->addFunction(function);
        feature->getFeature()->blockSignals(false);

    }

}

/*!
 * \brief Controller::recalcActiveFeature
 */
void Controller::recalcActiveFeature(){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = this->job->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //recalculate the active feature
    if(!feature->getTrafoParam().isNull()){
        this->featureUpdater.recalcTrafoParam(feature->getTrafoParam());
    }else{
        this->featureUpdater.recalcFeature(feature->getFeature());
    }

}

/*!
 * \brief Controller::sensorConfigurationChanged
 * \param name
 * \param connectSensor
 */
void Controller::sensorConfigurationChanged(const QString &name, const bool &connectSensor){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //get and check the specified sensor config
    SensorConfiguration sConfig = this->sensorConfigManager->getSavedSensorConfig(name);
    if(!sConfig.getIsValid()){
        Console::getInstance()->addLine(QString("No sensor configuration available with the name %1").arg(name));
        return;
    }

    //get and check plugin information
    sdb::Plugin plugin = SystemDbManager::getPlugin(sConfig.getPluginName());
    if(plugin.id == -1){
        Console::getInstance()->addLine(QString("No plugin available with the name %1").arg(sConfig.getPluginName()));
        return;
    }

    //create sensor instance and assign it to the active station
    QPointer<Sensor> sensor = PluginLoader::loadSensorPlugin(plugin.file_path, sConfig.getSensorName());
    if(sensor.isNull()){
        Console::getInstance()->addLine(QString("No sensor available with the name %1").arg(sConfig.getSensorName()));
        return;
    }

    //set active station's sensor
    sensor->setSensorConfiguration(sConfig);
    activeStation->setSensor(sensor);

    //connect the sensor
    if(connectSensor){
        this->startConnect();
    }

}

/*!
 * \brief Controller::measurementConfigurationChanged
 * \param name
 */
void Controller::measurementConfigurationChanged(const QString &name){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        Console::getInstance()->addLine("No active geometry selected");
        return;
    }

    //get and check the specified measurement config
    MeasurementConfig mConfig = this->measurementConfigManager->getSavedMeasurementConfig(name);
    if(!mConfig.getIsValid()){
        Console::getInstance()->addLine(QString("No measurement configuration available with the name %1").arg(name));
        return;
    }

    //set measurement config for the active feature
    activeFeature->getGeometry()->setMeasurementConfig(mConfig);

}

/*!
 * \brief Controller::setActiveFeature
 * \param featureId
 */
/*void Controller::setActiveFeature(const int &featureId){

    //check job
    if(this->job.isNull()){
        Console::getInstance()->addLine("No active job");
        return;
    }

    //set active feature
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(featureId);
    if(!feature.isNull() && !feature->getFeature().isNull()){
        feature->getFeature()->setActiveFeatureState(true);
    }else{
        Console::getInstance()->addLine(QString("No is no feature with the id %1 that could be activated").arg(featureId));
    }

}*/

/*!
 * \brief Controller::setActiveStation
 * \param featureId
 */
void Controller::setActiveStation(const int &featureId){

}

/*!
 * \brief Controller::setActiveCoordinateSystem
 * \param featureId
 */
void Controller::setActiveCoordinateSystem(const int &featureId){

}

/*!
 * \brief Controller::importNominals
 * \param params
 */
void Controller::importNominals(const ExchangeParams &params){

    //try to start import nominal task
    if(!this->exchanger.importData(params)){
        return;
    }

    //show loading dialog during import
    emit this->nominalImportStarted();

}

/*!
 * \brief Controller::importObservations
 * \param filename
 */
void Controller::importObservations(const QString &filename){

    //try to start import observations task
    if(!this->exchanger.importObservations(filename)){
        return;
    }

    //show loading dialog during import
    emit this->observationImportStarted();

}

/*!
 * \brief Controller::removeObservations
 * \param featureId
 */
void Controller::removeObservations(const int &featureId){

    //check job
    if(this->job.isNull()){
        Console::getInstance()->addLine("No job available");
        return;
    }

    this->job->removeObservations(featureId);

}

/*!
 * \brief Controller::removeAllObservations
 */
void Controller::removeAllObservations(){

    //check job
    if(this->job.isNull()){
        Console::getInstance()->addLine("No job available");
        return;
    }

    this->job->removeAllObservations();

}

/*!
 * \brief Controller::setFeatureTableColumnConfig
 * \param config
 */
void Controller::setFeatureTableColumnConfig(const FeatureTableColumnConfig &config){
    ModelManager::setFeatureTableColumnConfig(config);
}

/*!
 * \brief Controller::setTrafoParamColumnConfig
 * \param config
 */
void Controller::setTrafoParamColumnConfig(const TrafoParamTableColumnConfig &config){
    ModelManager::setTrafoParamColumnConfig(config);
}

/*!
 * \brief Controller::setObservationTableColumnConfig
 * \param config
 */
void Controller::setObservationTableColumnConfig(const ObservationTableColumnConfig &config){
    ModelManager::setObservationTableColumnConfig(config);
}

/*!
 * \brief Controller::setReadingTableColumnConfig
 * \param config
 */
void Controller::setReadingTableColumnConfig(const ReadingTableColumnConfig &config){
    ModelManager::setReadingTableColumnConfig(config);
}

/*!
 * \brief Controller::setParameterDisplayConfig
 * \param config
 */
void Controller::setParameterDisplayConfig(const ParameterDisplayConfig &config){
    ModelManager::setParameterDisplayConfig(config);
}

void Controller::saveProject()
{

}

/*!
 * \brief Controller::saveProject
 * \param fileName
 */
void Controller::saveProject(const QString &fileName){

    //check job
    if(this->job.isNull()){
        Console::getInstance()->addLine("No job available");
        return;
    }

    //set name
    QFileInfo info(fileName);
    this->job->setJobName(info.fileName());

    //set device
    QPointer<QIODevice> device = new QFile(fileName);
    this->job->setJobDevice(device);

    //set config manager
    if(!this->measurementConfigManager.isNull()){
        ProjectExchanger::setMeasurementConfigManager(*this->measurementConfigManager.data());
    }

    //get project xml
    QDomDocument project = ProjectExchanger::saveProject(this->job);
    if(project.isNull()){
        Console::getInstance()->addLine("Error while create project XML");
        return;
    }

    //save project xml
    this->job->getJobDevice()->open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(this->job->getJobDevice());
    project.save(stream, 4);
    this->job->getJobDevice()->close();

    Console::getInstance()->addLine("OpenIndy project successfully stored.");

}

/*!
 * \brief Controller::loadProject
 * \param projectName
 * \param device
 */
void Controller::loadProject(const QString &projectName, const QPointer<QIODevice> &device){

    //check device
    if(device.isNull()){
        Console::getInstance()->addLine("No device");
        return;
    }

    //create empty job
    QPointer<OiJob> newJob(NULL);

    //load xml file to DOM tree
    QDomDocument project;
    try{
        device->open(QIODevice::ReadOnly);
        project.setContent(device);
        device->close();
    }catch(const exception &e){
        Console::getInstance()->addLine("Error while opening OpenIndy xml file.");
        return;
    }

    //load project from xml
    newJob = ProjectExchanger::loadProject(project);

    //check job
    if(newJob.isNull()){
        Console::getInstance()->addLine("Error while parsing OpenIndy xml file.");
        return;
    }

    //set project meta data
    newJob->setJobDevice(device);
    newJob->setJobName(projectName);

    //delete old job
    if(!this->job.isNull()){
        delete this->job;
    }

    //set new job
    this->setJob(newJob);

    //switch to active coordinate system
    this->featureUpdater.switchCoordinateSystem();

    //connect active station
    this->activeStationChangedCallback();

    Console::getInstance()->addLine("OpenIndy project successfully loaded.");

}

/*!
 * \brief Controller::createDefaultJob
 * \return
 */
const QPointer<OiJob> &Controller::createDefaultJob(){

    //create job with a station and a nominal system
    QPointer<OiJob> job = new OiJob();
    this->setJob(job);

    QPointer<FeatureWrapper> stationFeature = new FeatureWrapper();
    QPointer<Station> station = new Station();
    station->setFeatureName("STATION01");
    stationFeature->setStation(station);

    QPointer<FeatureWrapper> systemFeature = new FeatureWrapper();
    QPointer<CoordinateSystem> system = new CoordinateSystem();
    system->setFeatureName("PART");
    systemFeature->setCoordinateSystem(system);

    job->addFeature(stationFeature);
    job->addFeature(systemFeature);

    //activate features
    station->setActiveStationState(true);
    station->getCoordinateSystem()->setActiveCoordinateSystemState(true);

    return this->job;

}

/*!
 * \brief Controller::startConnect
 */
void Controller::startConnect(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("connecting sensor...");

    //connect sensor
    activeStation->connectSensor();

}

/*!
 * \brief Controller::startDisconnect
 */
void Controller::startDisconnect(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("disconnecting sensor...");

    //disconnect sensor
    activeStation->disconnectSensor();

}

/*!
 * \brief Controller::startMeasurement
 */
void Controller::startMeasurement(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        Console::getInstance()->addLine("No active feature");
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //check sensor
    if(!activeStation->getIsSensorConnected()){
        Console::getInstance()->addLine("No sensor connected to the active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("performing measurement...");

    //perform measurement
    int id = activeFeature->getGeometry()->getId();
    MeasurementConfig mConfig = activeFeature->getGeometry()->getMeasurementConfig();
    activeStation->measure(id, mConfig);

}

/*!
 * \brief Controller::startMove
 * \param reading
 */
void Controller::startMove(const Reading &reading){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        Console::getInstance()->addLine("No active feature");
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("moving sensor...");

    //move sensor
    if(reading.getTypeOfReading() == eCartesianReading){
        activeStation->move(reading.getCartesianReading().xyz.getAt(0),
                            reading.getCartesianReading().xyz.getAt(1),
                            reading.getCartesianReading().xyz.getAt(2), false);
        return;
    }else if(reading.getTypeOfReading() == ePolarReading){
        activeStation->move(reading.getPolarReading().azimuth,
                            reading.getPolarReading().zenith,
                            reading.getPolarReading().distance, false, false);
        return;
    }

    emit this->sensorActionFinished(false, "Selected reading type is not supported");

}

/*!
 * \brief Controller::startAim
 */
void Controller::startAim(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        Console::getInstance()->addLine("No active feature");
        return;
    }

    //get and check active coordinate system
    QPointer<CoordinateSystem> activeCoordinateSystem = this->job->getActiveCoordinateSystem();
    if(activeCoordinateSystem.isNull()){
        Console::getInstance()->addLine("No active coordinate system");
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull() || activeStation->getCoordinateSystem().isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //create trafo controller instance and get the transformation matrix
    TrafoController trafoController;
    OiMat t(4, 4);
    trafoController.getTransformationMatrix(t, activeCoordinateSystem, activeStation->getCoordinateSystem());

    //transform position of the active feature into the station coordinate system
    if(activeFeature->getGeometry()->hasPosition()){
        Console::getInstance()->addLine("Active feature has no position to aim");
        return;
    }
    OiVec pos = activeFeature->getGeometry()->getPosition().getVectorH();
    pos = t * pos;

    //inform about start of sensor action
    emit this->sensorActionStarted("moving sensor...");

    //aim the active feature
    activeStation->move(pos.getAt(0), pos.getAt(1), pos.getAt(2), false);

}

/*!
 * \brief Controller::startAimAndMeasure
 */
void Controller::startAimAndMeasure(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> activeFeature = this->job->getActiveFeature();
    if(activeFeature.isNull() || activeFeature->getGeometry().isNull()){
        Console::getInstance()->addLine("No active feature");
        return;
    }

    //get and check active coordinate system
    QPointer<CoordinateSystem> activeCoordinateSystem = this->job->getActiveCoordinateSystem();
    if(activeCoordinateSystem.isNull()){
        Console::getInstance()->addLine("No active coordinate system");
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull() || activeStation->getCoordinateSystem().isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //create trafo controller instance and get the transformation matrix
    TrafoController trafoController;
    OiMat t(4, 4);
    if(!trafoController.getTransformationMatrix(t, activeCoordinateSystem, activeStation->getCoordinateSystem())){
        Console::getInstance()->addLine("No transformation found to transform between active coordinate system and the active station's system");
        return;
    }

    //transform position of the active feature into the station coordinate system
    if(activeFeature->getGeometry()->hasPosition()){
        Console::getInstance()->addLine("Active feature has no position to aim");
        return;
    }
    OiVec pos = activeFeature->getGeometry()->getPosition().getVectorH();
    pos = t * pos;

    //inform about start of sensor action
    emit this->sensorActionStarted("moving sensor and performing measurement...");

    //aim the active feature and perform measurement
    activeStation->move(pos.getAt(0), pos.getAt(1), pos.getAt(2), true, activeFeature->getGeometry()->getId(),
                        activeFeature->getGeometry()->getMeasurementConfig());

}

/*!
 * \brief Controller::startToggleSight
 */
void Controller::startToggleSight(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("toggling sight orientation...");

    //toggle sight
    activeStation->toggleSight();

}

/*!
 * \brief Controller::startInitialize
 */
void Controller::startInitialize(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("initializing sensor...");

    //initialize sensor
    activeStation->initialize();

}

/*!
 * \brief Controller::startHome
 */
void Controller::startHome(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("setting sensor to home position...");

    //set sensor to home position
    activeStation->home();

}

/*!
 * \brief Controller::startCompensation
 */
void Controller::startCompensation(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("starting compensation...");

    //start compensation
    activeStation->compensation();

}

/*!
 * \brief Controller::startChangeMotorState
 */
void Controller::startChangeMotorState(){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted("changing motor state...");

    //change motor state
    activeStation->motorState();

}

/*!
 * \brief Controller::startCustomAction
 * \param task
 */
void Controller::startCustomAction(const QString &task){

    //check current job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = this->job->getActiveStation();
    if(activeStation.isNull()){
        Console::getInstance()->addLine("No active station");
        return;
    }

    //inform about start of sensor action
    emit this->sensorActionStarted(QString("custom action: %1 ...").arg(task));

    //do self defined action
    activeStation->selfDefinedAction(task);

}

/*!
 * \brief Controller::activeStationChangedCallback
 */
void Controller::activeStationChangedCallback(){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check active station
    QPointer<Station> activeStation = job->getActiveStation();
    if(activeStation.isNull()){
        return;
    }

    //connect sensor action results
    QObject::connect(activeStation.data(), &Station::commandFinished, this, &Controller::sensorActionFinished, Qt::AutoConnection);
    QObject::connect(activeStation.data(), &Station::measurementFinished, this, &Controller::measurementFinished, Qt::AutoConnection);

}

/*!
 * \brief Controller::measurementFinished
 * \param geomId
 * \param readings
 */
void Controller::measurementFinished(const int &geomId, const QList<QPointer<Reading> > &readings){

    //check job
    if(this->job.isNull()){
        return;
    }

    //get and check feature with the id geomId
    QPointer<FeatureWrapper> feature = this->job->getFeatureById(geomId);
    if(feature.isNull() || feature->getGeometry().isNull()){
        foreach(const QPointer<Reading> &reading, readings){
            if(!reading.isNull()){
                delete reading;
            }
        }
        return;
    }

    //if the feature is a nominal add the observation to its actual
    if(feature->getGeometry()->getIsNominal()){

        //check actual
        if(!feature->getGeometry()->getActual().isNull() && !feature->getGeometry()->getActual()->getFeatureWrapper().isNull()
                && !feature->getGeometry()->getActual()->getFeatureWrapper()->getGeometry().isNull()){ //has actual
            feature = feature->getGeometry()->getActual()->getFeatureWrapper();
        }else if(feature->getGeometry()->getActual().isNull()){ //does not have an actual

            //create a new feature
            FeatureAttributes attributes;
            attributes.count = 1;
            attributes.group = feature->getFeature()->getGroupName();
            attributes.name = feature->getFeature()->getFeatureName();
            attributes.typeOfFeature = feature->getFeatureTypeEnum();
            QList<QPointer<FeatureWrapper> > features = this->job->addFeatures(attributes);

            //check feature
            if(features.size() == 1 && !features.at(0).isNull() && !features.at(0)->getGeometry().isNull()){
                feature = features.at(0);
            }

        }

        //check feature
        if(feature.isNull() || feature->getGeometry().isNull() || feature->getGeometry()->getIsNominal()){
            foreach(const QPointer<Reading> &reading, readings){
                if(!reading.isNull()){
                    delete reading;
                }
            }
            return;
        }

    }

    //add observations
    this->job->addMeasurementResults(geomId, readings);

    //complete the measurement task by informing MainWindow about its success
    emit this->measurementCompleted();

}

/*!
 * \brief Controller::setJob
 * \param job
 */
void Controller::setJob(const QPointer<OiJob> &job){

    //check job
    if(job.isNull()){
        return;
    }

    //delete old job
    if(!this->job.isNull()){
        delete this->job;
    }

    //set and connect new job
    this->job = job;

    //log messages to console
    QObject::connect(this->job.data(), &OiJob::sendMessage, this, &Controller::logToConsole, Qt::AutoConnection);

    //active feature changes
    QObject::connect(this->job.data(), &OiJob::activeFeatureChanged, this, &Controller::activeFeatureChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::activeStationChanged, this, &Controller::activeStationChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::activeStationChanged, this, &Controller::activeStationChangedCallback, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::activeCoordinateSystemChanged, this, &Controller::activeCoordinateSystemChanged, Qt::AutoConnection);

    //feature(s) added or removed
    QObject::connect(this->job.data(), &OiJob::featureSetChanged, this, &Controller::featureSetChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::coordSystemSetChanged, this, &Controller::coordSystemSetChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::stationSetChanged, this, &Controller::stationSetChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::trafoParamSetChanged, this, &Controller::trafoParamSetChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometrySetChanged, this, &Controller::geometrySetChanged, Qt::AutoConnection);

    //group(s) added or removed
    QObject::connect(this->job.data(), &OiJob::availableGroupsChanged, this, &Controller::availableGroupsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::activeGroupChanged, this, &Controller::activeGroupChanged, Qt::AutoConnection);

    //general feature attributes changed
    QObject::connect(this->job.data(), &OiJob::featureAttributesChanged, this, &Controller::featureAttributesChanged, Qt::AutoConnection);

    //feature specific attributes changed
    QObject::connect(this->job.data(), &OiJob::featureNameChanged,
                     this, &Controller::featureNameChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureGroupChanged,
                     this, &Controller::featureGroupChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureCommentChanged,
                     this, &Controller::featureCommentChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureIsUpdatedChanged,
                     this, &Controller::featureIsUpdatedChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureIsSolvedChanged,
                     this, &Controller::featureIsSolvedChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureFunctionsChanged,
                     this, &Controller::featureFunctionsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featureUsedForChanged,
                     this, &Controller::featureUsedForChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::featurePreviouslyNeededChanged,
                     this, &Controller::featurePreviouslyNeededChanged, Qt::AutoConnection);

    //geometry specific attributes changed
    QObject::connect(this->job.data(), &OiJob::geometryIsCommonChanged,
                     this, &Controller::geometryIsCommonChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryNominalsChanged,
                     this, &Controller::geometryNominalsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryActualChanged,
                     this, &Controller::geometryActualChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryObservationsChanged,
                     this, &Controller::geometryObservationsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryNominalSystemChanged,
                     this, &Controller::geometryNominalSystemChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryStatisticChanged,
                     this, &Controller::geometryStatisticChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometrySimulationDataChanged,
                     this, &Controller::geometrySimulationDataChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::geometryMeasurementConfigChanged,
                     this, &Controller::geometryMeasurementConfigChanged, Qt::AutoConnection);

    //coordinate system specific attributes changed
    QObject::connect(this->job.data(), &OiJob::systemObservationsChanged,
                     this, &Controller::systemObservationsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::systemTrafoParamsChanged,
                     this, &Controller::systemTrafoParamsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::systemsNominalsChanged,
                     this, &Controller::systemsNominalsChanged, Qt::AutoConnection);

    //station specific attributes changed
    QObject::connect(this->job.data(), &OiJob::stationSensorChanged,
                     this, &Controller::stationSensorChanged, Qt::AutoConnection);

    //transformation parameter specific attributes changed
    QObject::connect(this->job.data(), &OiJob::trafoParamParametersChanged,
                     this, &Controller::trafoParamParametersChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::trafoParamSystemsChanged,
                     this, &Controller::trafoParamSystemsChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::trafoParamIsUsedChanged,
                     this, &Controller::trafoParamIsUsedChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::trafoParamValidTimeChanged,
                     this, &Controller::trafoParamValidTimeChanged, Qt::AutoConnection);
    QObject::connect(this->job.data(), &OiJob::trafoParamIsMovementChanged,
                     this, &Controller::trafoParamIsMovementChanged, Qt::AutoConnection);

    //pass the new job around
    ModelManager::setCurrentJob(this->job);
    this->exchanger.setCurrentJob(this->job);
    this->featureUpdater.setCurrentJob(this->job);

}

/*!
 * \brief Controller::initDisplayConfigs
 * Sets up initial display configs and passes them to model manager
 */
void Controller::initDisplayConfigs(){

    //create default configs
    FeatureTableColumnConfig featureTableColumnConfig;
    TrafoParamTableColumnConfig trafoParamTableColumnConfig;
    ObservationTableColumnConfig observationTableColumnConfig;
    ReadingTableColumnConfig readingTableColumnConfig;
    ParameterDisplayConfig parameterDisplayConfig;

    //pass the default configs to model manager
    ModelManager::setFeatureTableColumnConfig(featureTableColumnConfig);
    ModelManager::setTrafoParamColumnConfig(trafoParamTableColumnConfig);
    ModelManager::setObservationTableColumnConfig(observationTableColumnConfig);
    ModelManager::setReadingTableColumnConfig(readingTableColumnConfig);
    ModelManager::setParameterDisplayConfig(parameterDisplayConfig);

}

/*!
 * \brief Controller::initConfigManager
 */
void Controller::initConfigManager(){

    //load configs from config folder
    this->sensorConfigManager = new SensorConfigurationManager();
    this->sensorConfigManager->loadFromConfigFolder();
    this->measurementConfigManager = new MeasurementConfigManager();
    this->measurementConfigManager->loadFromConfigFolder();

    //pass config managers to model manager
    ModelManager::setSensorConfigManager(this->sensorConfigManager);
    ModelManager::setMeasurementConfigManager(this->measurementConfigManager);

}

/*!
 * \brief Controller::logToConsole
 * \param msg
 */
void Controller::logToConsole(const QString &msg){
    Console::getInstance()->addLine(msg);
}

/*!
 * \brief Controller::registerMetaTypes
 * Registers meta types to be able to use them in signal slot connections
 */
void Controller::registerMetaTypes(){

    qRegisterMetaType<oi::MeasurementConfig>("MeasurementConfig");
    qRegisterMetaType<oi::SensorConfiguration>("SensorConfiguration");
    qRegisterMetaType<QList<QPointer<oi::Reading> > >("QList<QPointer<Reading> >");
    qRegisterMetaType<QPointer<oi::Feature> >("QPointer<Feature>");
    qRegisterMetaType<QPointer<oi::Function> >("QPointer<Function>");
    qRegisterMetaType<QPointer<oi::FeatureWrapper> >("QPointer<FeatureWrapper>");
    qRegisterMetaType<QPointer<oi::Observation> >("QPointer<Observation>");

}

/*!
 * \brief Controller::connectDataExchanger
 */
void Controller::connectDataExchanger(){

    //nominal import
    QObject::connect(&this->exchanger, &DataExchanger::nominalImportFinished, this, &Controller::nominalImportFinished, Qt::AutoConnection);
    QObject::connect(&this->exchanger, &DataExchanger::updateNominalImportProgress, this, &Controller::nominalImportProgressUpdated, Qt::AutoConnection);

    //observation import
    QObject::connect(&this->exchanger, &DataExchanger::observationImportFinished, this, &Controller::observationImportFinished, Qt::AutoConnection);
    QObject::connect(&this->exchanger, &DataExchanger::updateObservationImportProgress, this, &Controller::observationImportProgressUpdated, Qt::AutoConnection);

}

/*!
 * \brief Controller::connectFeatureUpdater
 */
void Controller::connectFeatureUpdater(){

}
