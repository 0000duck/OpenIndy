#include "oimodelmanager.h"

/*QStringListModel OiModelManager::pluginNamesModel;
QStringListModel OiModelManager::coordinateSystemsModel;
QStringListModel OiModelManager::nominalSystemsModel;
QStringListModel OiModelManager::geometryTypes;
QStandardItemModel OiModelManager::sensorTypes;
QStandardItemModel OiModelManager::baudRateTypes;
QStandardItemModel OiModelManager::dataBitTypes;
QStandardItemModel OiModelManager::flowControlTypes;
QStandardItemModel OiModelManager::parityTypes;
QStandardItemModel OiModelManager::stopBitTypes;
QStandardItemModel OiModelManager::availableSerialPorts;
QStandardItemModel OiModelManager::availableIpAdresses;
//QStringListModel *OiModelManager::simpleAsciiExchangePlugins;
//QStringListModel OiModelManager::definedFormatExchangePlugins;
QStringListModel OiModelManager::distanceUnitsModel;
QStringListModel OiModelManager::angleUnitsModel;
QStringListModel OiModelManager::temperatureUnitsModel;
QStringListModel OiModelManager::groupNamesModel;
FeatureTableModel OiModelManager::featureTableModel;
FeatureTableProxyModel OiModelManager::featureTableProxyModel;
//TrafoParamProxyModel OiModelManager::trafoParamProxyModel;
//FeatureTreeViewModel OiModelManager::featureTreeViewModel;
//FeatureGraphicsTreeViewProxyModel OiModelManager::featureGraphicsModel;
//PluginTreeViewModel OiModelManager::pluginTreeViewModel;
QSqlQueryModel OiModelManager::createFeatureFunctionsModel;
QSqlQueryModel OiModelManager::changeFeatureFunctionsModel;
QSqlQueryModel OiModelManager::laserTrackerModel;
QSqlQueryModel OiModelManager::totalStationModel;
QSqlQueryModel OiModelManager::undefinedSensorModel;*/




FeatureTableModel OiModelManager::featureTableModel;
FeatureTableProxyModel OiModelManager::featureTableProxyModel;

QPointer<OiJob> OiModelManager::currentJob;

OiModelManager::OiModelManager(QObject *parent) : QObject(parent){

}

/*!
 * \brief OiModelManager::getCurrentJob
 * \return
 */
const QPointer<OiJob> &OiModelManager::getCurrentJob(){
    return OiModelManager::currentJob;
}

/*!
 * \brief OiModelManager::setCurrentJob
 * \param job
 */
void OiModelManager::setCurrentJob(const QPointer<OiJob> &job){
    if(!job.isNull()){
        OiModelManager::currentJob = job;
    }
}

/*!
 * \brief OiModelManager::getPluginNamesModel
 * Get a model that contains the names of all loaded plugins
 * \return
 */
/*QStringListModel &OiModelManager::getPluginNamesModel(){
    return OiModelManager::pluginNamesModel;
}*/

/*!
 * \brief OiModelManager::getPluginTreeViewModel
 * Get a model that contains all loaded plugins with hierachical information
 * \return
 */
/*PluginTreeViewModel &OiModelManager::getPluginTreeViewModel(){
    return OiModelManager::pluginTreeViewModel;
}*/

/*!
 * \brief OiModelManager::getCreateFunctionsModel
 * Get a model that contains all create functions available in the loaded plugins
 * \return
 */
//QSqlQueryModel &OiModelManager::getCreateFunctionsModel(){
//    return OiModelManager::createFeatureFunctionsModel;
//}

/*!
 * \brief OiModelManager::getChangeFunctionsModel
 * Get a model that contains all change functions available in the loaded plugins
 * \return
 */
/*QSqlQueryModel &OiModelManager::getChangeFunctionsModel(){
    return OiModelManager::changeFeatureFunctionsModel;
}*/

/*!
 * \brief OiModelManager::getSensorsModel
 * Get a model that contains all sensors of the given type
 * \param typeOfSensor
 * \return
 */
/*QSqlQueryModel &OiModelManager::getSensorsModel(SensorTypes typeOfSensor){
    switch(typeOfSensor){
    case eLaserTracker:
        return OiModelManager::laserTrackerModel;
    case eTotalStation:
        return OiModelManager::totalStationModel;
    default:
        return OiModelManager::undefinedSensorModel;
    }
}*/

/*!
 * \brief OiModelManager::getCoordinateSystemsModel
 * Get a model that contains the names of all coordinate systems
 * \return
 */
/*QStringListModel &OiModelManager::getCoordinateSystemsModel(){
    return OiModelManager::coordinateSystemsModel;
}*/

/*!
 * \brief OiModelManager::getNominalSystemsModel
 * Get a model that contains the names of all nominal coordinate systems
 * \return
 */
/*QStringListModel &OiModelManager::getNominalSystemsModel(){
    return OiModelManager::nominalSystemsModel;
}*/

/*!
 * \brief OiModelManager::getGeometryTypesModel
 * Get a model that contains the names of all available geometry types
 * \return
 */
/*QStringListModel &OiModelManager::getGeometryTypesModel(){
    return OiModelManager::geometryTypes;
}*/

/*!
 * \brief OiModelManager::getSimpleAsciiExchangePlugins
 * Get a model that contains the names of all available simple ascii exchange plugins
 * \param plugin
 * \return
 */
/*QStringListModel *OiModelManager::getSimpleAsciiExchangePlugins(QString plugin){

    QMultiMap<QString,QString> simpleAsciiExchangePlugins;// = SystemDbManager::getAvailableSimpleAsciiExchangePlugins();

    QStringListModel *pluginsModel = new QStringListModel();
    QStringList exchangePlugins = simpleAsciiExchangePlugins.values(plugin);
    pluginsModel->setStringList(exchangePlugins);

    return pluginsModel;
}*/

/*!
 * \brief OiModelManager::getAvailableElementsModel
 * The model contains a hierarchy of all elements of type filter
 * \param filter
 * \return
 */
/*AvailableElementsTreeViewProxyModel *OiModelManager::getAvailableElementsModel(ElementTypes filter){

    AvailableElementsTreeViewProxyModel *availableElementsModel;// = new AvailableElementsTreeViewProxyModel();

    availableElementsModel->setHeader("available elements");
    availableElementsModel->setSourceModel(&OiModelManager::featureTreeViewModel);
    availableElementsModel->setFilter(filter, false);

    return availableElementsModel;

}*/

/*!
 * \brief OiModelManager::getUsedElementsModel
 * The model contains all elements used for the function at functionIndex of the active feature (at elementIndex)
 * \param functionIndex: index of the function in the list of functions of active feature
 * \param elementIndex: index of the needed elements of the function
 * \return
 */
/*UsedElementsModel *OiModelManager::getUsedElementsModel(int functionIndex, int elementIndex){

    UsedElementsModel *usedElementsModel = new UsedElementsModel();

    usedElementsModel->updateModel(functionIndex, elementIndex);

    return usedElementsModel;

}*/

/*!
 * \brief OiModelManager::getFunctionTreeViewModel
 * \return
 */
//QStandardItemModel *OiModelManager::getFunctionTreeViewModel(){
/*
    //check if active feature is valid
    if(OiJob::getActiveFeature() == NULL || OiJob::getActiveFeature()->getFeature() == NULL){
        return NULL;
    }

    QStandardItemModel *functionTreeViewModel = new QStandardItemModel();

    functionTreeViewModel->setHorizontalHeaderItem(0, new QStandardItem("functions"));

    QStandardItem *rootItem = functionTreeViewModel->invisibleRootItem();
    foreach(Function *f, OiJob::getActiveFeature()->getFeature()->getFunctions()){

        if(f == NULL){
            continue;
        }

        QStandardItem *function = new QStandardItem(f->getMetaData()->name);
        foreach(InputParams param, f->getNeededElements()){
            if(param.infinite){
                QStandardItem *element = new QStandardItem(QString("n %1s").arg(getElementTypeString(param.typeOfElement)));
                function->appendRow(element);
            }else{
                QStandardItem *element = new QStandardItem(QString("1 %1").arg(getElementTypeString(param.typeOfElement)));
                function->appendRow(element);
            }
        }

        rootItem->appendRow(function);

    }

    return functionTreeViewModel;*/

    //return new QStandardItemModel();

//}

/*!
 * \brief OiModelManager::featureSetChanged
 */
/*void OiModelManager::featureSetChanged(){
    OiModelManager::featureTableModel.updateModel();
    //OiModelManager::featureTreeViewModel.refreshModel();
}*/

/*!
 * \brief OiModelManager::activeFeatureChanged
 */
/*void OiModelManager::activeFeatureChanged(){
    OiModelManager::featureTableModel.updateModel();
}
*/
/*!
 * \brief OiModelManager::activeStationChanged
 */
/*void OiModelManager::activeStationChanged(){
    OiModelManager::featureTableModel.updateModel();
}*/

/*!
 * \brief OiModelManager::activeCoordinateSystemChanged
 */
/*void OiModelManager::activeCoordinateSystemChanged(){
    OiModelManager::featureTableModel.updateModel();
}*/

/*!
 * \brief OiModelManager::availableGroupsChanged
 */
/*void OiModelManager::availableGroupsChanged(){
    this->initGroupNameModels();
}*/

/*!
 * \brief OiModelManager::coordSystemSetChanged
 */
/*void OiModelManager::coordSystemSetChanged(){
    this->initCoordinateSystemModels();
}*/

/*!
 * \brief OiModelManager::featuresRecalculated
 */
/*void OiModelManager::featuresRecalculated(){

}*/

/*!
 * \brief OiModelManager::getSensorTypes
 * \return
 */
/*QStandardItemModel &OiModelManager::getSensorTypes(){
    return OiModelManager::sensorTypes;
}*/

/*!
 * \brief OiModelManager::getBaudRateTypes
 * \return
 */
/*QStandardItemModel &OiModelManager::getBaudRateTypes(){
    return OiModelManager::baudRateTypes;
}*/

/*!
 * \brief OiModelManager::getDataBitTypes
 * \return
 */
/*QStandardItemModel &OiModelManager::getDataBitTypes(){
    return OiModelManager::dataBitTypes;
}*/

/*!
 * \brief OiModelManager::getFlowControlTypes
 * \return
 */
/*QStandardItemModel &OiModelManager::getFlowControlTypes(){
    return OiModelManager::flowControlTypes;
}*/

/*!
 * \brief OiModelManager::getParityTypes
 * \return
 */
/*QStandardItemModel &OiModelManager::getParityTypes(){
    return OiModelManager::parityTypes;
}*/

/*!
 * \brief OiModelManager::getStopBitTypes
 * \return
 */
/*QStandardItemModel &OiModelManager::getStopBitTypes(){
    return OiModelManager::stopBitTypes;
}*/

/*!
 * \brief OiModelManager::getAvailableSerialPorts
 * \return
 */
/*QStandardItemModel &OiModelManager::getAvailableSerialPorts(){
    return OiModelManager::availableSerialPorts;
}*/

/*!
 * \brief OiModelManager::getAvailableIpAdresses
 * \return
 */
/*QStandardItemModel &OiModelManager::getAvailableIpAdresses(){
    return OiModelManager::availableIpAdresses;
}*/

/*!
 * \brief OiModelManager::getDistanceUnitsModel
 * \return
 */
/*QStringListModel &OiModelManager::getDistanceUnitsModel(){
    return OiModelManager::distanceUnitsModel;
}*/

/*!
 * \brief OiModelManager::getAngleUnitsModel
 * \return
 */
//QStringListModel &OiModelManager::getAngleUnitsModel(){
//    return OiModelManager::angleUnitsModel;
//}

///*!
// * \brief OiModelManager::getTemperatureUnitsModel
// * \return
// */
//QStringListModel &OiModelManager::getTemperatureUnitsModel(){
//    return OiModelManager::temperatureUnitsModel;
//}

///*!
// * \brief OiModelManager::getGroupNamesModel
// * \return
// */
//QStringListModel &OiModelManager::getGroupNamesModel(){
//    return OiModelManager::groupNamesModel;
//}

/*!
 * \brief OiModelManager::getFeatureTableModel
 * Returns a table model with all features of the current job
 * \return
 */
FeatureTableModel &OiModelManager::getFeatureTableModel(){
    return OiModelManager::featureTableModel;
}

/*!
 * \brief OiModelManager::getFeatureTableProxyModel
 * Returns a table model with all features of the current job except trafo params
 * \return
 */
FeatureTableProxyModel &OiModelManager::getFeatureTableProxyModel(){
    return OiModelManager::featureTableProxyModel;
}

/*!
 * \brief OiModelManager::initModels
 */
void OiModelManager::initModels(){

    OiModelManager::featureTableProxyModel.setSourceModel(&OiModelManager::featureTableModel);
    OiModelManager::featureTableProxyModel.setDynamicSortFilter(true);

}

/*!
 * \brief OiModelManager::getTrafoParamProxyModel
 * \return
 */
/*TrafoParamProxyModel &OiModelManager::getTrafoParamProxyModel(){
    return OiModelManager::trafoParamProxyModel;
}*/

/*!
 * \brief OiModelManager::getFeatureTreeViewModel
 * \return
 */
/*FeatureTreeViewModel &OiModelManager::getFeatureTreeViewModel(){
    return OiModelManager::featureTreeViewModel;
}*/

/*!
 * \brief OiModelManager::getFeatureGraphicsModel
 * \return
 */
/*FeatureGraphicsTreeViewProxyModel &OiModelManager::getFeatureGraphicsModel(){
    return OiModelManager::featureGraphicsModel;
}*/

/*!
 * \brief OiModelManager::getGeometryTypesFilterModel
 * Get a model that contains all available geometry types and has the ability to set a filter
 * \return
 */
/*GeometryTypesProxyModel *OiModelManager::getGeometryTypesFilterModel(){
    GeometryTypesProxyModel *model = new GeometryTypesProxyModel();
    model->setSourceModel(&OiModelManager::geometryTypes);
    return model;
}*/

/*!
 * \brief OiModelManager::initModels
 * Initialize all models once when OpenIndy is opened
 */
//void OiModelManager::initModels(){

//    //initialize plugin models
//    this->initPluginModels();

//    //initialize coordinate system models
//    this->initCoordinateSystemModels();

//    //initialize geometry types
//    QStringList geometryTypes;
//    /*geometryTypes.append(sPoint);
//    geometryTypes.append(sLine);
//    geometryTypes.append(sPlane);
//    geometryTypes.append(sSphere);
//    geometryTypes.append(sCylinder);
//    geometryTypes.append(sCone);
//    geometryTypes.append(sHyperboloid);
//    geometryTypes.append(sParaboloid);
//    geometryTypes.append(sEllipsoid);
//    geometryTypes.append(sPointCloud);
//    geometryTypes.append(sCircle);
//    geometryTypes.append(sNurbs);
//    geometryTypes.append(sEntitiyAngle);
//    geometryTypes.append(sEntityDistance);
//    geometryTypes.append(sEntityMeasurementSeries);
//    geometryTypes.append(sEntityTemperature);*/
//    OiModelManager::geometryTypes.setStringList(geometryTypes);

//    //initialize sensor specific models
//    this->initSensorModels();

//    //initialize unit models
//    this->initUnitModels();

//    //models for tableviews
//    OiModelManager::featureTableProxyModel.setSourceModel(&OiModelManager::featureTableModel);
//    OiModelManager::featureTableProxyModel.setDynamicSortFilter(true);
//    //OiModelManager::trafoParamProxyModel.setSourceModel(&OiModelManager::featureTableModel);

//    //treeview models
//    //OiModelManager::featureGraphicsModel.setSourceModel(&OiModelManager::featureTreeViewModel);

//    this->initGroupNameModels();















//    /*QStringList simpleAsciiExchangePlugins = SystemDbManager::getAvailableSimpleAsciiExchangePlugins();
//    QStringList definedFormatExchangePlugins = SystemDbManager::getAvailableDefinedFormatExchangePlugins();
//    OiModelManager::simpleAsciiExchangePlugins.setStringList(simpleAsciiExchangePlugins);
//    OiModelManager::definedFormatExchangePlugins.setStringList(definedFormatExchangePlugins);*/

//}

///*!
// * \brief OiModelManager::connectModels
// */
//void OiModelManager::connectModels(){

//    /*connect(OiJob::getInstance(), SIGNAL(featureSetChanged()), this, SLOT(featureSetChanged()), Qt::DirectConnection);
//    connect(OiJob::getInstance(), SIGNAL(activeFeatureChanged()), this, SLOT(activeFeatureChanged()), Qt::DirectConnection);
//    connect(OiJob::getInstance(), SIGNAL(activeCoordinateSystemChanged()), this, SLOT(activeCoordinateSystemChanged()), Qt::DirectConnection);
//    connect(OiJob::getInstance(), SIGNAL(activeStationChanged()), this, SLOT(activeStationChanged()), Qt::DirectConnection);

//    connect(FeatureUpdater::getInstance(), SIGNAL(featuresRecalculated()), this, SLOT(featuresRecalculated()), Qt::DirectConnection);

//    connect(OiJob::getInstance(), SIGNAL(availableGroupsChanged()), this, SLOT(availableGroupsChanged()), Qt::DirectConnection);

//    connect(OiJob::getInstance(), SIGNAL(coordSystemSetChanged()), this, SLOT(coordSystemSetChanged()), Qt::DirectConnection);
//*/
//}

///*!
// * \brief OiModelManager::initSensorModels
// */
//void OiModelManager::initSensorModels(){
///*
//    //initialize sensor types model
//    QStandardItem *laserTracker = new QStandardItem("lasertracker");
//    laserTracker->setData(eLaserTracker, Qt::UserRole);
//    this->sensorTypes.appendRow(laserTracker);
//    QStandardItem *totalStation = new QStandardItem("totalstation");
//    totalStation->setData(eTotalStation, Qt::UserRole);
//    this->sensorTypes.appendRow(totalStation);
//    QStandardItem *undefinedSensor = new QStandardItem("undefined sensor");
//    undefinedSensor->setData(eUndefinedSensor, Qt::UserRole);
//    this->sensorTypes.appendRow(undefinedSensor);

//    //initialize baud rate model
//    QStandardItem *baud1200 = new QStandardItem("1200");
//    baud1200->setData(QSerialPort::Baud1200, Qt::UserRole);
//    this->baudRateTypes.appendRow(baud1200);
//    QStandardItem *baud2400 = new QStandardItem("2400");
//    baud2400->setData(QSerialPort::Baud2400, Qt::UserRole);
//    this->baudRateTypes.appendRow(baud2400);
//    QStandardItem *baud4800 = new QStandardItem("4800");
//    baud4800->setData(QSerialPort::Baud4800, Qt::UserRole);
//    this->baudRateTypes.appendRow(baud4800);
//    QStandardItem *baud9600 = new QStandardItem("9600");
//    baud9600->setData(QSerialPort::Baud9600, Qt::UserRole);
//    this->baudRateTypes.appendRow(baud9600);
//    QStandardItem *baud19200 = new QStandardItem("19200");
//    baud19200->setData(QSerialPort::Baud19200, Qt::UserRole);
//    this->baudRateTypes.appendRow(baud19200);
//    QStandardItem *baud38400 = new QStandardItem("38400");
//    baud38400->setData(QSerialPort::Baud38400, Qt::UserRole);
//    this->baudRateTypes.appendRow(baud38400);
//    QStandardItem *baud57600 = new QStandardItem("57600");
//    baud57600->setData(QSerialPort::Baud57600, Qt::UserRole);
//    this->baudRateTypes.appendRow(baud57600);
//    QStandardItem *baud115200 = new QStandardItem("115200");
//    baud115200->setData(QSerialPort::Baud115200, Qt::UserRole);
//    this->baudRateTypes.appendRow(baud115200);

//    //initialize databits model
//    QStandardItem *data5 = new QStandardItem("5");
//    data5->setData(QSerialPort::Data5, Qt::UserRole);
//    this->dataBitTypes.appendRow(data5);
//    QStandardItem *data6 = new QStandardItem("6");
//    data6->setData(QSerialPort::Data6, Qt::UserRole);
//    this->dataBitTypes.appendRow(data6);
//    QStandardItem *data7 = new QStandardItem("7");
//    data7->setData(QSerialPort::Data7, Qt::UserRole);
//    this->dataBitTypes.appendRow(data7);
//    QStandardItem *data8 = new QStandardItem("8");
//    data8->setData(QSerialPort::Data8, Qt::UserRole);
//    this->dataBitTypes.appendRow(data8);

//    //initialize flow control model
//    QStandardItem *noControl = new QStandardItem("no flowcontrol");
//    noControl->setData(QSerialPort::NoFlowControl, Qt::UserRole);
//    this->flowControlTypes.appendRow(noControl);
//    QStandardItem *hardwareControl = new QStandardItem("hardware flowcontrol");
//    hardwareControl->setData(QSerialPort::HardwareControl, Qt::UserRole);
//    this->flowControlTypes.appendRow(hardwareControl);
//    QStandardItem *softwareControl = new QStandardItem("software flowcontrol");
//    softwareControl->setData(QSerialPort::SoftwareControl, Qt::UserRole);
//    this->flowControlTypes.appendRow(softwareControl);
//    QStandardItem *unknownControl = new QStandardItem("unknown flowcontrol");
//    unknownControl->setData(QSerialPort::UnknownFlowControl, Qt::UserRole);
//    this->flowControlTypes.appendRow(unknownControl);

//    //initialize parity model
//    QStandardItem *noParity = new QStandardItem("no parity");
//    noParity->setData(QSerialPort::NoParity, Qt::UserRole);
//    this->parityTypes.appendRow(noParity);
//    QStandardItem *evenParity = new QStandardItem("even parity");
//    evenParity->setData(QSerialPort::EvenParity, Qt::UserRole);
//    this->parityTypes.appendRow(evenParity);
//    QStandardItem *oddParity = new QStandardItem("odd parity");
//    oddParity->setData(QSerialPort::OddParity, Qt::UserRole);
//    this->parityTypes.appendRow(oddParity);
//    QStandardItem *spaceParity = new QStandardItem("space parity");
//    spaceParity->setData(QSerialPort::SpaceParity, Qt::UserRole);
//    this->parityTypes.appendRow(spaceParity);
//    QStandardItem *markParity = new QStandardItem("mark parity");
//    markParity->setData(QSerialPort::MarkParity, Qt::UserRole);
//    this->parityTypes.appendRow(markParity);
//    QStandardItem *unknownParity = new QStandardItem("unknown parity");
//    unknownParity->setData(QSerialPort::UnknownParity, Qt::UserRole);
//    this->parityTypes.appendRow(unknownParity);

//    //initialize stop bits model
//    QStandardItem *oneStop = new QStandardItem("one stop");
//    oneStop->setData(QSerialPort::OneStop, Qt::UserRole);
//    this->stopBitTypes.appendRow(oneStop);
//    QStandardItem *oneHalfStop = new QStandardItem("one and half");
//    oneHalfStop->setData(QSerialPort::OneAndHalfStop, Qt::UserRole);
//    this->stopBitTypes.appendRow(oneHalfStop);
//    QStandardItem *twoStop = new QStandardItem("two stop");
//    twoStop->setData(QSerialPort::TwoStop, Qt::UserRole);
//    this->stopBitTypes.appendRow(twoStop);
//    QStandardItem *unknownStop = new QStandardItem("unknown stopbits");
//    unknownStop->setData(QSerialPort::UnknownStopBits, Qt::UserRole);
//    this->stopBitTypes.appendRow(unknownStop);

//    //initialize serial ports model
//    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
//        QStandardItem *serialPort = new QStandardItem(info.portName());
//        this->availableSerialPorts.appendRow(serialPort);
//    }

//    //initialize ip4 addresses model
//    QList<QHostAddress> ipAdresses = QNetworkInterface::allAddresses();
//    foreach(const QHostAddress &adress, ipAdresses){
//        if(adress.protocol() == QAbstractSocket::IPv4Protocol){
//            QStandardItem *ip = new QStandardItem(adress.toString());
//            this->availableIpAdresses.appendRow(ip);
//        }
//    }*/

//}

///*!
// * \brief OiModelManager::initCoordinateSystemModels
// */
//void OiModelManager::initCoordinateSystemModels(){
///*
//    QStringList coordinateSystems;
//    QStringList nominalSystems;
//    QList<CoordinateSystem *> mySystems = OiJob::getCoordinateSystems();
//    foreach(CoordinateSystem *c, mySystems){
//        nominalSystems.append(c->getFeatureName());
//        coordinateSystems.append(c->getFeatureName());
//    }
//    QList<Station *> myStations = OiJob::getStations();
//    foreach(Station *s, myStations){
//        coordinateSystems.append(s->getFeatureName());
//    }
//    OiModelManager::nominalSystemsModel.setStringList(nominalSystems);
//    OiModelManager::coordinateSystemsModel.setStringList(coordinateSystems);
//*/
//}

///*!
// * \brief OiModelManager::initUnitModels
// */
//void OiModelManager::initUnitModels(){

//    //distance units
//    /*QStringList distanceUnits;
//    distanceUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eMeter));
//    distanceUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eMillimeter));
//    distanceUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eInch));
//    OiModelManager::distanceUnitsModel.setStringList(distanceUnits);

//    //angle units
//    QStringList angleUnits;
//    angleUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eDecimalDegree));
//    angleUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eGon));
//    angleUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eRadiant));
//    angleUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eMilliRadians));
//    angleUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eArcSeconds));
//    OiModelManager::angleUnitsModel.setStringList(angleUnits);

//    //temperature units
//    QStringList temperatureUnits;
//    temperatureUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eGRAD));
//    temperatureUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eKelvin));
//    temperatureUnits.append(OiUnitConverter::getUnitString(OiUnitConverter::eFahrenheit));
//    OiModelManager::temperatureUnitsModel.setStringList(temperatureUnits);*/

//}

///*!
// * \brief OiModelManager::initGroupNameModels
// */
//void OiModelManager::initGroupNameModels(){

//    /*QStringList availableGroups = OiJob::getAvailableGroups();
//    availableGroups.push_front("");
//    OiModelManager::groupNamesModel.setStringList(availableGroups);*/

//}

///*!
// * \brief OiModelManager::initPluginModels
// */
//void OiModelManager::initPluginModels(){

//    //plugin tree view
//    //OiModelManager::pluginTreeViewModel.refreshModel();

//    //plugin names
//    //QStringList pluginNames = SystemDbManager::getAvailablePluginNames();
//    //OiModelManager::pluginNamesModel.setStringList(pluginNames);



//}
