#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \brief MainWindow constructor that most of initializing operations.
 * most connects between gui elements and  controller class are implemented in this function.
 * Also some initializations of models, dialogs and gui elements of the main view are made here.
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initializeActions();

    this->ui->comboBox_groups->addItem("All Groups");

    ui->toolBar_ControlPad->addWidget(labelSensorControlName);
    ui->toolBar_ControlPad->addAction(cPsep9);

    ui->listView_Console->setModel(control.c->output);

    ui->widget_graphics->features = &control.features;

    FeatureOverviewDelegate *myFeatureDelegate = new FeatureOverviewDelegate();
    this->ui->tableView_data->setItemDelegate(myFeatureDelegate);
    this->ui->tableView_data->setModel(this->control.featureOverviewModel);
    TrafoParamDelegate *myTrafoParamDelegate = new TrafoParamDelegate();
    this->ui->tableView_trafoParam->setItemDelegate(myTrafoParamDelegate);
    this->ui->tableView_trafoParam->setModel(this->control.trafoParamModel);

    ui->treeView_featureOverview->setModel(this->control.featureGraphicsModel);
    fPluginDialog.receiveAvailableElementsModel(this->control.availableElementsModel);
    fPluginDialog.receiveUsedElementsModel(this->control.usedElementsModel);

    this->setUpDialog.setPluginsModel(this->control.myPluginTreeViewModel);

    cFeatureDialog = new CreateFeature(this->control.features);
    sEntityDialog = new ScalarEntityDialog(this->control.features);

    //settings for other widgets
    mConfigDialog.setModal(true);
    pLoadDialog.setModal(true);
    moveDialog.setModal(true);
    cFeatureDialog->setModal(true);
    sPluginDialog.setModal(true);
    fPluginDialog.setModal(true);
    sInfoDialog.setModal(true);
    setUpDialog.setModal(true);
    sEntityDialog->setModal(true);
    nominalDialog.setModal(true);
    trafoParamDialog.setModal(true);
    watchWindowDialog.setModal(true);

    //delete feature
    this->ui->tableView_data->setContextMenuPolicy(Qt::CustomContextMenu);
    this->ui->tableView_trafoParam->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(sendDeleteFeatures(QList<FeatureWrapper*>)), &control, SLOT(deleteFeatures(QList<FeatureWrapper*>)));
    connect(&control, SIGNAL(resetFeatureSelection()), this, SLOT(resetFeatureSelection()));

    //measurement config settings
    connect(&mConfigDialog,SIGNAL(sendConfig(FeatureWrapper*,MeasurementConfig*)),this,SLOT(receiveConfig(FeatureWrapper*,MeasurementConfig*)));
    connect(this,SIGNAL(sendConfig(MeasurementConfig*)),&mConfigDialog,SLOT(receiveConfig(MeasurementConfig*)));

    //sensor function
    connect(this->actionMeasure,SIGNAL(triggered()),&control,SLOT(startMeasurement()));
    connect(this->actionAim,SIGNAL(triggered()),&control,SLOT(startAim()));
    connect(this->actionConnect,SIGNAL(triggered()),&control,SLOT(startConnect()));
    connect(this->actionDisconnect,SIGNAL(triggered()),&control,SLOT(startDisconnect()));
    connect(this->actionToggleSightOrientation,SIGNAL(triggered()),&control,SLOT(startToggleSight()));
    connect(this,SIGNAL(sendCommandString(QString)),&control,SLOT(sendCmdString(QString)));
    connect(this->actionInitialize,SIGNAL(triggered()),&control,SLOT(startInitialize()));
    connect(this->actionHome,SIGNAL(triggered()),&control,SLOT(startHome()));
    connect(this->actionChangeMotorState,SIGNAL(triggered()),&control,SLOT(startChangeMotorState()));
    connect(this->actionCompensation,SIGNAL(triggered()),&control,SLOT(startCompensation()));
    connect(control.activeStation->sensorPad, SIGNAL(recalcFeature(Feature*)), &control, SLOT(recalcFeature(Feature*)));

    //sensor info
    connect(&control,SIGNAL(sensorWorks(QString)),&sInfoDialog,SLOT(showInfo(QString)));
    connect(control.activeStation,SIGNAL(actionFinished(bool)),&sInfoDialog,SLOT(hideInfo(bool)));
    connect(&control, SIGNAL(changedStation()),this,SLOT(changedStation()));

    //mainwindow actions
    connect(this->actionMConfig,SIGNAL(triggered()),this,SLOT(openCreateFeatureMConfig()));
    connect(ui->actionClose,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->tableView_data,SIGNAL(clicked(QModelIndex)),this,SLOT(handleTableViewClicked(QModelIndex)));
    connect(this->lineEditSendCommand,SIGNAL(returnPressed()),this, SLOT(sendCommand()));
    connect(this->actionCreate,SIGNAL(triggered()),this,SLOT(createFeature()));
    connect(this->actionMove,SIGNAL(triggered()),&moveDialog,SLOT(show()));
    connect(&moveDialog,SIGNAL(sendReading(Reading*)),&control,SLOT(startMove(Reading*)));
    //connect(ui->actionActivate_station,SIGNAL(triggered()),&control,SLOT(changeActiveStation()));
    connect(ui->tableView_trafoParam,SIGNAL(clicked(QModelIndex)),&control,SLOT(handleTrafoParamClicked(QModelIndex)));
    connect(this->comboBoxFeatureType,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeCreateFeatureToolbar(int)));
    connect(this->checkBoxNominal,SIGNAL(toggled(bool)),this,SLOT(CheckBoxNominalToggled(bool)));

    connect(this,SIGNAL(sendSelectedFeature(int)),&control,SLOT(getSelectedFeature(int)));
    connect(this,SIGNAL(sendFeatureType(Configuration::FeatureTypes)),cFeatureDialog,SLOT(receiveFeatureType(Configuration::FeatureTypes)));
    connect(control.c,SIGNAL(changedList()),this->ui->listView_Console,SLOT(scrollToBottom()));

    connect(&control,SIGNAL(CoordSystemAdded()),this,SLOT(fillCoordSysComboBox()));
    connect(ui->comboBox_activeCoordSystem,SIGNAL(currentIndexChanged(QString)),this,SLOT(getActiveCoordSystem(QString)));
    connect(this,SIGNAL(sendActiveCoordSystem(QString)),&control,SLOT(setActiveCoordSystem(QString)));

    //openGl view connects
    connect(&control,SIGNAL(sendPositionOfActiveFeature(double,double,double)),ui->widget_graphics,SLOT(focusOnFeature(double,double,double)));
    //connect(&control,SIGNAL(featureAdded()),this,SLOT(setupCreateFeature()));

    //TODO Loesung finden, da statusbar Text verschwindet
    connect(&setUpDialog,SIGNAL(accepted()),this,SLOT(setUpStatusBar()));
    connect(&setUpDialog,SIGNAL(rejected()),this,SLOT(setUpStatusBar()));

    //feature dialog
    connect(cFeatureDialog,SIGNAL(createFeature(FeatureAttributesExchange)),&control,SLOT(addFeature(FeatureAttributesExchange)));
    connect(cFeatureDialog,SIGNAL(createFeatureMConfig()),this,SLOT(openCreateFeatureMConfig()));
    //connect(cFeatureDialog,SIGNAL(createTrafoParam(int,int,QString,CoordinateSystem*,CoordinateSystem*)),&control,SLOT(addTrafoParam(int,int,QString,CoordinateSystem*,CoordinateSystem*)));
    connect(&nominalDialog, SIGNAL(sendNominalValues(NominalAttributeExchange)),&control,SLOT(getNominalValues(NominalAttributeExchange)));
    connect(this,SIGNAL(sendActiveNominalfeature(FeatureWrapper*)),&nominalDialog,SLOT(getActiveFeature(FeatureWrapper*)));

    //Scalar entity dialog
    connect(sEntityDialog,SIGNAL(createFeature(FeatureAttributesExchange)),&control,SLOT(addFeature(FeatureAttributesExchange)));
    connect(sEntityDialog,SIGNAL(createFeatureMConfig()),this,SLOT(openCreateFeatureMConfig()));

    //sensor plugin dialog
    connect(&sPluginDialog,SIGNAL(sendSensorType(Configuration::SensorTypes)),&control,SLOT(setSensorModel(Configuration::SensorTypes)));
    connect(&sPluginDialog,SIGNAL(selectedPlugin(int)),&control,SLOT(getSelectedPlugin(int)));
    connect(&sPluginDialog,SIGNAL(sendSensorConfig(SensorConfiguration*,bool)),&control,SLOT(receiveSensorConfiguration(SensorConfiguration*,bool)));
    connect(&control,SIGNAL(sendSQLModel(QSqlQueryModel*)),&sPluginDialog,SLOT(receiveModel(QSqlQueryModel*)));
    connect(&sPluginDialog,SIGNAL(selectedTempPlugin(int)),&control,SLOT(getTempSensor(int)));
    connect(&control,SIGNAL(sendTempSensor(Sensor*)),&sPluginDialog,SLOT(receiveTempSensor(Sensor*)));

    //function plugin dialog
    connect(&fPluginDialog, SIGNAL(sendPluginID(int)),&control,SLOT(receiveFunctionId(int)));
    connect(&fPluginDialog, SIGNAL(createFunction(int)), &control, SLOT(createFunction(int)));
    connect(&fPluginDialog, SIGNAL(setSelectedFunction(int,int)), &control, SLOT(setSelectedFunction(int,int)));
    connect(&control, SIGNAL(sendFunctionDescription(QString)), &fPluginDialog, SLOT(receiveFunctionDescription(QString)));
    connect(&fPluginDialog, SIGNAL(deleteFunction(int)), &control, SLOT(deleteFunctionFromFeature(int)));
    connect(&fPluginDialog, SIGNAL(addElement(FeatureTreeItem*,int,int)), &control, SLOT(addElement2Function(FeatureTreeItem*,int,int)));
    connect(&fPluginDialog, SIGNAL(removeElement(FeatureTreeItem*,int,int)), &control, SLOT(removeElementFromFunction(FeatureTreeItem*,int,int)));
    connect(&fPluginDialog, SIGNAL(recalcActiveFeature()), &control, SLOT(recalcActiveFeature()));
    connect(&control, SIGNAL(sendExtraParameterForFunction(QMap<QString,int>,QMap<QString,double>,QMap<QString,QStringList>,FunctionConfiguration)),
            &fPluginDialog, SLOT(receiveExtraParameterForFunction(QMap<QString,int>,QMap<QString,double>,QMap<QString,QStringList>,FunctionConfiguration)));
    connect(&fPluginDialog, SIGNAL(sendFunctionConfiguration(int,FunctionConfiguration)),
            &control, SLOT(setFunctionConfiguration(int,FunctionConfiguration)));

    connect(&control, SIGNAL(showMessageBox(QString,QString)), this, SLOT(showMessageBox(QString,QString)));
    connect(&control, SIGNAL(showMessageBoxForDecision(QString,QString,OiFunctor*)), this, SLOT(showMessageBoxForDecision(QString,QString,OiFunctor*)));

    //group combo boxes
    connect(&control, SIGNAL(availableGroupsChanged(QMap<QString,int>)), this, SLOT(availableGroupsChanged(QMap<QString,int>)));
    connect(control.tblModel, SIGNAL(groupNameChanged(QString,QString)), &control, SLOT(groupNameChanged(QString,QString)));

    //setup create feature toolbar
    setupCreateFeature();
    //connect(&control, SIGNAL(updateGeometryIcons(QStringList)), this, SLOT(updateGeometryIcons(QStringList)));
    //this->control.checkAvailablePlugins();

    //dataimport
    connect(&importNominalDialog,SIGNAL(sendFeature(QList<FeatureWrapper*>)),&control,SLOT(importFeatures(QList<FeatureWrapper*>)));

    ui->toolBar_ControlPad->hide();
    ui->toolbarCreateFeature->hide();

    //fill coordinatesystem comboBox
    fillCoordSysComboBox();

    setUpStatusBar();

}

/*!
 * \brief MainWindow destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief ChangeCreateFeatureToolbar function enables or disables elements of the create feature toolbar dependend on the selected feature type.
 * Only possible and relevant configurations and attributes can be set. The elements change on the fly by changing the
 * selected feature type in the combobox.
 * \param int i
 */
void MainWindow::ChangeCreateFeatureToolbar(int i)
{
    this->checkBoxNominal->setChecked(false);
    this->checkBoxActual->setChecked(true);
    this->checkBoxCommonPoint->setChecked(false);

    int featureType = static_cast<Configuration::FeatureTypes>(this->comboBoxFeatureType->itemData(this->comboBoxFeatureType->currentIndex()).toInt());

    switch (featureType) {
    case Configuration::eCoordinateSystemFeature:{
        this->actionMConfig->setEnabled(false);
        this->comboBoxNominalSystem->setEnabled(false);
        this->checkBoxNominal->setEnabled(false);
        this->checkBoxActual->setChecked(true);
        this->checkBoxActual->setEnabled(false);
        this->checkBoxCommonPoint->setEnabled(false);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    }
    case Configuration::eTrafoParamFeature:{
        this->actionMConfig->setEnabled(false);
        this->comboBoxNominalSystem->setEnabled(false);
        this->checkBoxNominal->setEnabled(false);
        this->checkBoxActual->setChecked(true);
        this->checkBoxActual->setEnabled(false);
        this->checkBoxCommonPoint->setEnabled(false);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    }
    case Configuration::ePlaneFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::ePointFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::eLineFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::eStationFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setChecked(false);
        this->checkBoxNominal->setEnabled(false);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::eSphereFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::eCylinderFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::eConeFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::eEllipsoidFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::eHyperboloidFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::eParaboloidFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::eNurbsFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::ePointCloudFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    case Configuration::eCircleFeature:
        this->actionMConfig->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(true);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(true);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    default:{
        this->actionMConfig->setEnabled(false);
        this->comboBoxNominalSystem->setEnabled(false);
        this->checkBoxNominal->setEnabled(true);
        this->checkBoxActual->setEnabled(true);
        this->checkBoxCommonPoint->setEnabled(false);
        this->comboBoxNominalSystem->setEnabled(false);
        break;
    }
    }
    this->defaultCreateFeatureSettings();
}

/*!
 * \brief CheckBoxNominalToggled function enables or disables the combobox for nominal coordinate systems dependend on the state of the checkbox.
 * \param toggled
 */
void MainWindow::CheckBoxNominalToggled(bool toggled)
{
    if(toggled){
        this->comboBoxNominalSystem->setEnabled(true);
    }else{
        this->comboBoxNominalSystem->setEnabled(false);
    }
}

/*!
 * \brief setupCreateFeature function creates and sets upp the create feature toolbar
 * With that toolbar you can create new features and set all needed
 * information for that function.
 */
void MainWindow::setupCreateFeature(){

    this->comboBoxFeatureType->clear();
    this->comboBoxNominalSystem->clear();

    ui->toolbarCreateFeature->addWidget(comboBoxFeatureType);
    ui->toolbarCreateFeature->addAction(cFsep);
    ui->toolbarCreateFeature->addWidget(labelName);
    ui->toolbarCreateFeature->addWidget(lineEditName);
    ui->toolbarCreateFeature->addAction(cFsep1);
    this->ui->toolbarCreateFeature->addWidget(this->labelGroup);
    this->ui->toolbarCreateFeature->addWidget(this->comboBoxGroup);
    this->ui->toolbarCreateFeature->addAction(this->cFsep8);
    ui->toolbarCreateFeature->addWidget(labelCount);
    ui->toolbarCreateFeature->addWidget(spinBoxNumber);
    ui->toolbarCreateFeature->addAction(cFsep2);
    ui->toolbarCreateFeature->addWidget(checkBoxActual);
    ui->toolbarCreateFeature->addAction(cFsep3);
    ui->toolbarCreateFeature->addWidget(checkBoxNominal);
    ui->toolbarCreateFeature->addAction(cFsep8);
    ui->toolbarCreateFeature->addWidget(checkBoxCommonPoint);
    ui->toolbarCreateFeature->addAction(cFsep4);
    ui->toolbarCreateFeature->addAction(actionMConfig);
    ui->toolbarCreateFeature->addAction(cFsep5);
    ui->toolbarCreateFeature->addWidget(labelNominalSystem);
    ui->toolbarCreateFeature->addAction(cFsep6);
    ui->toolbarCreateFeature->addWidget(comboBoxNominalSystem);
    ui->toolbarCreateFeature->addAction(cFsep7);
    ui->toolbarCreateFeature->addAction(actionCreate);

    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"point",Configuration::ePointFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"line",Configuration::eLineFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"plane",Configuration::ePlaneFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"sphere",Configuration::eSphereFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"station",Configuration::eStationFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"coordinatesystem",Configuration::eCoordinateSystemFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"cylinder",Configuration::eCylinderFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"cone", Configuration::eConeFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"ellipsoid",Configuration::eEllipsoidFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"hyperboloid",Configuration::eHyperboloidFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"paraboloid",Configuration::eParaboloidFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"nurbs",Configuration::eNurbsFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"pointcloud",Configuration::ePointCloudFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"circle",Configuration::eCircleFeature);

    for(int i=0; i<this->control.coordSys.size();i++){
        this->comboBoxNominalSystem->addItem(this->control.coordSys.at(i)->name);
    }

    this->defaultCreateFeatureSettings();
}

/*!
 * \brief setupLaserTrackerPad adds all needed Widgets to the control pad, if the active sensor is a lasertracker
 * With that label you can control the laser tracker and start measurements.
 */
void MainWindow::setupLaserTrackerPad(){

    ui->toolBar_ControlPad->addWidget(labelSendCommand);
    ui->toolBar_ControlPad->addWidget(lineEditSendCommand);
    ui->toolBar_ControlPad->addAction(cPsep);
    ui->toolBar_ControlPad->addAction(actionConnect);
    ui->toolBar_ControlPad->addAction(cPsep1);
    ui->toolBar_ControlPad->addAction(actionDisconnect);
    ui->toolBar_ControlPad->addAction(cPsep2);
    ui->toolBar_ControlPad->addAction(actionInitialize);
    ui->toolBar_ControlPad->addAction(cPsep3);
    ui->toolBar_ControlPad->addAction(actionHome);
    ui->toolBar_ControlPad->addAction(cPsep4);
    ui->toolBar_ControlPad->addAction(actionMeasure);
    ui->toolBar_ControlPad->addAction(cPsep5);
    ui->toolBar_ControlPad->addAction(actionToggleSightOrientation);
    ui->toolBar_ControlPad->addAction(cPsep6);
    ui->toolBar_ControlPad->addAction(actionAim);
    ui->toolBar_ControlPad->addAction(cPsep7);
    ui->toolBar_ControlPad->addAction(actionMove);
    ui->toolBar_ControlPad->addAction(cPsep8);
    ui->toolBar_ControlPad->addAction(actionChangeMotorState);
    ui->toolBar_ControlPad->addAction(cPsep10);
    ui->toolBar_ControlPad->addAction(actionCompensation);

}

/*!
 * \brief setupTotalStationPad adds all needed Widgets to the control pad, if the active sensor is a totalstation
 * With that label you can control the totalstation and start measurements.
 */
void MainWindow::setupTotalStationPad(){

    ui->toolBar_ControlPad->addWidget(labelSendCommand);
    ui->toolBar_ControlPad->addWidget(lineEditSendCommand);
    ui->toolBar_ControlPad->addAction(cPsep);
    ui->toolBar_ControlPad->addAction(actionConnect);
    ui->toolBar_ControlPad->addAction(cPsep1);
    ui->toolBar_ControlPad->addAction(actionDisconnect);
    ui->toolBar_ControlPad->addAction(cPsep2);
    ui->toolBar_ControlPad->addAction(actionMeasure);
    ui->toolBar_ControlPad->addAction(cPsep3);
    ui->toolBar_ControlPad->addAction(actionToggleSightOrientation);
    ui->toolBar_ControlPad->addAction(cPsep4);
    ui->toolBar_ControlPad->addAction(actionAim);

}

/*!
 * \brief displays the entered text in the console
 */
void MainWindow::on_lineEdit_inputConsole_returnPressed()
{
    Console::addLine(ui->lineEdit_inputConsole->text());
    ui->lineEdit_inputConsole->clear();
}

/*!
 * \brief sendCommand function sends the entered command to the controller class.
 * There the command will be send to the active sensor that analyses and handles the command.
 */
void MainWindow::sendCommand()
{
    Console::addLine("test");
    emit sendCommandString(this->lineEditSendCommand->text());
}

/*!
 * \brief Sets the control pad visible or invisible
 * Also it calls the needed set up function to add all elements for the active sensor.
 */
void MainWindow::on_actionControl_pad_triggered()
{
    if(ui->toolBar_ControlPad->isVisible()){
        ui->toolBar_ControlPad->hide();
    }else{
        ui->toolBar_ControlPad->show();

        if(control.activeStation->getInstrumentConfig() !=NULL){
            if(control.activeStation->getInstrumentConfig()->instrumentType==Configuration::eLaserTracker){
                labelSensorControlName->setText("sensor control laser tracker");
                setupLaserTrackerPad();
            }else if(control.activeStation->getInstrumentConfig()->instrumentType==Configuration::eTotalStation){
                labelSensorControlName->setText("sensor control total station");
                setupTotalStationPad();
            }
        }
    }
}

/*!
 * \brief save measurement configuration for selected feature, if the measurement configuration of an existing feature was edited
 * Else the configuration is saved in the controller for all following new features, that need a measurement configuration.
 * \param FeatureWrapper *af
 * \param MeasurementConfig *mC
 */
void MainWindow::receiveConfig(FeatureWrapper *af, MeasurementConfig *mC){

    if(af == NULL){
        this->control.lastmConfig = mC;
    }else{
        if(this->control.activeFeature->getStation() != NULL){
            this->control.activeFeature->getStation()->position->mConfig = *mC;
        }else{
            this->control.activeFeature->getGeometry()->mConfig = *mC;
        }
    }
}

/*!
 * \brief edit MeasurementConfig of selected feature.
 * This configuration only belongs to the selected feature.
 * You can set new configurations at the create feature / scalar entity menu for new features.
 */
void MainWindow::on_actionMeasurement_Configuration_triggered()
{
    if(this->control.activeFeature != NULL && (this->control.activeFeature->getGeometry() != NULL || this->control.activeFeature->getStation() != NULL)){
        mConfigDialog.activeFeature = this->control.activeFeature;
        mConfigDialog.setStation(this->control.activeStation);

        if(this->control.activeFeature->getGeometry() != NULL){
            emit sendConfig(&this->control.activeFeature->getGeometry()->mConfig);
        }
        if(this->control.activeFeature->getStation() != NULL){
            emit sendConfig(&this->control.activeFeature->getStation()->position->mConfig);
        }

        mConfigDialog.show();
    }
}

/*!
 * \brief Shows or hides the console
 */
void MainWindow::on_actionConsole_triggered()
{
    if(ui->dockWidget_Console->isVisible()){
        ui->dockWidget_Console->hide();
    }else{
        ui->dockWidget_Console->show();
    }
}

/*!
 * \brief Opens the watch window and sets the station of the watchwindow to the active station
 */
void MainWindow::on_actionWatch_window_triggered()
{
    watchWindowDialog.myStation = control.activeStation;
    watchWindowDialog.show();
}

/*!
 * \brief initializeActions
 */
void MainWindow::initializeActions(){
    //create feature actions
    labelName = new QLabel();
    labelName->setText("name:");
    labelCount = new QLabel();
    labelCount->setText("count:");
    comboBoxFeatureType = new QComboBox();
    lineEditName =  new QLineEdit();
    lineEditName->setPlaceholderText("feature name");
    this->labelGroup = new QLabel("group:");
    this->comboBoxGroup = new QComboBox();
    this->comboBoxGroup->setEditable(true);
    spinBoxNumber = new QSpinBox();
    spinBoxNumber->setMaximum(1000);
    spinBoxNumber->setMinimum(1);
    checkBoxActual = new QCheckBox();
    checkBoxActual->setText("actual");
    checkBoxNominal = new QCheckBox();
    checkBoxNominal->setText("nominal");
    checkBoxCommonPoint = new QCheckBox();
    checkBoxCommonPoint->setText("common point");
    actionCreate = new QAction(0);
    actionCreate->setText("create");
    labelNominalSystem = new QLabel();
    labelNominalSystem->setText("nominal system");
    comboBoxNominalSystem = new QComboBox();
    actionMConfig = new QAction(0);
    actionMConfig->setText("measurement config");
    actionMConfig->setToolTip("measurement configuration for new features. This will not edit existing feature configurations");

    //sensor control pad
    actionConnect = new QAction(0);
    actionConnect->setText("connect");
    actionDisconnect = new QAction(0);
    actionDisconnect->setText("disconnect");
    actionInitialize = new QAction(0);
    actionInitialize->setText("initialize");
    actionMeasure = new QAction(0);
    actionMeasure->setText("measure");
    actionAim = new QAction(0);
    actionAim->setText("aim");
    actionMove = new QAction(0);
    actionMove->setText("move");
    actionHome = new QAction(0);
    actionHome->setText("home");
    actionChangeMotorState = new QAction(0);
    actionChangeMotorState->setText("change motor state");
    actionToggleSightOrientation = new QAction(0);
    actionToggleSightOrientation->setText("toggle sight orientation");
    lineEditSendCommand = new QLineEdit();
    labelSendCommand = new QLabel();
    labelSendCommand->setText("insert command string:");
    actionCompensation = new QAction(0);
    actionCompensation->setText("compensation");
    labelSensorControlName = new QLabel();
    labelSensorControlName->setText("sensor control");


    //seperators create feature
    cFsep = new QAction(0);
    cFsep->setSeparator(true);
    cFsep1 = new QAction(0);
    cFsep1->setSeparator(true);
    cFsep2 = new QAction(0);
    cFsep2->setSeparator(true);
    cFsep3 = new QAction(0);
    cFsep3->setSeparator(true);
    cFsep4 = new QAction(0);
    cFsep4->setSeparator(true);
    cFsep5 = new QAction(0);
    cFsep5->setSeparator(true);
    cFsep6 = new QAction(0);
    cFsep6->setSeparator(true);
    cFsep7 = new QAction(0);
    cFsep7->setSeparator(true);
    cFsep8 = new QAction(0);
    cFsep8->setSeparator(true);

    //seperators control pad
    cPsep = new QAction(0);
    cPsep->setSeparator(true);
    cPsep1  = new QAction(0);
    cPsep1->setSeparator(true);
    cPsep2  = new QAction(0);
    cPsep2->setSeparator(true);
    cPsep3  = new QAction(0);
    cPsep3->setSeparator(true);
    cPsep4  = new QAction(0);
    cPsep4->setSeparator(true);
    cPsep5  = new QAction(0);
    cPsep5->setSeparator(true);
    cPsep6  = new QAction(0);
    cPsep6->setSeparator(true);
    cPsep7  = new QAction(0);
    cPsep7->setSeparator(true);
    cPsep8  = new QAction(0);
    cPsep8->setSeparator(true);
    cPsep9 = new QAction(0);
    cPsep9->setSeparator(true);
    cPsep10 = new QAction(0);
    cPsep10->setSeparator(true);
}

/*!
 * \brief createFeature aalls the add/ create feature function of the controller, and gives in the GUI set up information to that function
 */
void MainWindow::createFeature(){

    try{

        if(this->spinBoxNumber->value()!=0){

            int count = this->spinBoxNumber->value();
            QString name = this->lineEditName->text();
            QString group = this->comboBoxGroup->currentText();
            int featureType = static_cast<Configuration::FeatureTypes>(this->comboBoxFeatureType->itemData(this->comboBoxFeatureType->currentIndex()).toInt());
            bool actual = this->checkBoxActual->isChecked();
            bool nominal = this->checkBoxNominal->isChecked();
            bool comPoint = this->checkBoxCommonPoint->isChecked();

            CoordinateSystem * nominalSystem = NULL;

            //TODO nominal hinzufügen

            if(nominal){
                for(int i=0;i<this->control.coordSys.size();i++){
                    if(this->comboBoxNominalSystem->currentText() == this->control.coordSys.at(i)->name){
                        nominalSystem = this->control.coordSys.at(i);
                    }
                }
            }

            FeatureAttributesExchange featureAttributes(count,featureType,name,group,"",false,false,actual,nominal,comPoint,nominalSystem);

            control.addFeature(featureAttributes);


        }
    }catch(std::exception e){

    }
    this->defaultCreateFeatureSettings();
}

/*!
 * \brief displays the dialog to load a plugin
 */
void MainWindow::on_actionLoad_plugins_triggered()
{
    pLoadDialog.show();
}

/*!
 * \brief MainWindow::on_actionPlugin_manager_triggered
 */
void MainWindow::on_actionPlugin_manager_triggered(){

}

/*!
 * \brief shows/ hides the create feature toolbar
 */
void MainWindow::on_actionShow_hide_feature_toolbar_triggered()
{
    if(ui->toolbarCreateFeature->isVisible()){
        ui->toolbarCreateFeature->hide();
    }else{
        ui->toolbarCreateFeature->show();
    }
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (point).
 */
void MainWindow::on_actionCreate_point_triggered()
{
    emit sendFeatureType(Configuration::ePointFeature);
    //cFeatureDialog->show();
    this->showCreateFeatureDialog(Configuration::ePointFeature);
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (line).
 */
void MainWindow::on_actionCreate_line_triggered()
{
    emit sendFeatureType(Configuration::eLineFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (plane).
 */
void MainWindow::on_actionCreate_plane_triggered()
{
    emit sendFeatureType(Configuration::ePlaneFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (sphere).
 */
void MainWindow::on_actionCreate_sphere_triggered()
{
    emit sendFeatureType(Configuration::eSphereFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (cone).
 */
void MainWindow::on_actionCreate_cone_triggered()
{
    emit sendFeatureType(Configuration::eConeFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (cylinder).
 */
void MainWindow::on_actionCreate_cylinder_triggered()
{
    emit sendFeatureType(Configuration::eCylinderFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (ellipsoid).
 */
void MainWindow::on_actionCreate_ellipsoid_triggered()
{
    emit sendFeatureType(Configuration::eEllipsoidFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (hyperboloid).
 */
void MainWindow::on_actionCreate_hyperboloid_triggered()
{
    emit sendFeatureType(Configuration::eHyperboloidFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (paraboloid).
 */
void MainWindow::on_actionCreate_paraboloid_triggered()
{
    emit sendFeatureType(Configuration::eParaboloidFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (nurbs).
 */
void MainWindow::on_actionCreate_nurbs_triggered()
{
    emit sendFeatureType(Configuration::eNurbsFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (pointcloud).
 */
void MainWindow::on_actionCreate_pointcloud_triggered()
{
    emit sendFeatureType(Configuration::ePointCloudFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (station).
 */
void MainWindow::on_actionCreate_station_triggered()
{
    emit sendFeatureType(Configuration::eStationFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (coordinate system).
 */
void MainWindow::on_actionCreate_coordinatesystem_triggered()
{
    emit sendFeatureType(Configuration::eCoordinateSystemFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (transformation parameters)
 */
void MainWindow::on_actionCreate_trafoParam_triggered()
{
    emit sendFeatureType(Configuration::eTrafoParamFeature);
    cFeatureDialog->show();
}

/*!
 * \brief opens the dialog for creating a feature and emits the feature type that will be created (circle)
 */
void MainWindow::on_actionCreate_circle_triggered()
{
    emit sendFeatureType(Configuration::eCircleFeature);
    cFeatureDialog->show();
}

/*!
 * \brief set Instrument. Opens the dialog with all configurations for setting an instrument.
 */
void MainWindow::on_actionSet_instrument_triggered()
{
    sPluginDialog.show();
}

/*!
 * \brief handleTableViewClicked detects the current selected feature and sends it to the controller class.
 * Also displays the measurement config of the feature on the fly, if the measurement config dialog is open.
 * \param const QModelIndex &idx
 */
void MainWindow::handleTableViewClicked(const QModelIndex &idx){
    FeatureOvserviewProxyModel *model = static_cast<FeatureOvserviewProxyModel*>(this->ui->tableView_data->model());

    QModelIndex source_idx = model->mapToSource(idx);

    if(this->selectedFeature != source_idx.row()){
        //hide available elements treeview elements
        if(this->control.availableElementsModel != NULL){
            this->control.availableElementsModel->setFilter(Configuration::eUndefinedElement, true);
        }
        //disable used elements and available elements treeviews
        this->fPluginDialog.disableFunctionInteractions();
        //set description of function plugin loader to empty text
        this->fPluginDialog.receiveFunctionDescription("");
    }

    this->selectedFeature = source_idx.row();

    emit this->sendSelectedFeature(selectedFeature);
}

/*!
 * \brief Opens the dialog for setting functions to a feature.
 * Sets the plugins model and the function treeview model to the class.
 */
void MainWindow::on_actionSet_function_triggered(){
    if(this->control.activeFeature != NULL && this->control.activeFeature->getFeature() != NULL){
        //get models from database
        this->control.setFunction();
        //send models to function plugin loader
        fPluginDialog.receivePluginsModel(this->control.pluginsModel);
        fPluginDialog.receiveFunctionsModel(this->control.functionTreeViewModel);
        //set title and show dialog
        fPluginDialog.setActiveFeatureTitle(this->control.activeFeature->getFeature()->name);
        fPluginDialog.show();
    }
}

/*!
 * \brief opens the measurement configuration dialog.
 * Also the last configuration and the current active station is set to the class and with this the current active
 * sensor for the supported reading types.
 */
void MainWindow::openCreateFeatureMConfig(){
    emit sendConfig(this->control.lastmConfig);
    mConfigDialog.setStation(this->control.activeStation);
    mConfigDialog.show();
}

/*!
 * \brief adds all coordinate systems to the coordinate system combobox.
 */
void MainWindow::fillCoordSysComboBox(){
//TODO saubere lösung wäre vllt ein Model für die Combobox
    ui->comboBox_activeCoordSystem->clear();
    for (int i = 0; i < control.features.size();i++){
        if(control.features.at(i)->getCoordinateSystem() != NULL){
            ui->comboBox_activeCoordSystem->insertItem(i,control.features.at(i)->getCoordinateSystem()->name);

            if(control.features.at(i)->getCoordinateSystem() == control.activeCoordinateSystem){
                ui->comboBox_activeCoordSystem->setCurrentIndex(i);
            }
        }

        if(control.features.at(i)->getStation()!= NULL){
             ui->comboBox_activeCoordSystem->insertItem(i,control.features.at(i)->getStation()->name);

             if(control.features.at(i)->getStation()->coordSys == control.activeCoordinateSystem){
                 ui->comboBox_activeCoordSystem->setCurrentIndex(i);
             }
        }
    }
}

/*!
 * \brief setUpStatusBar displays the current unit settings in the mainwindow status bar.
 */
void MainWindow::setUpStatusBar(){

    QString statusText = "";

    switch (UnitConverter::distanceType) {
    case UnitConverter::eMETER:
        statusText += "metric unit: meters   ";
        break;
    case UnitConverter::eMILLIMETER:
        statusText += "metric unit: millimeters   ";
        break;
    default:
        break;
    }

    switch (UnitConverter::angleType) {
    case UnitConverter::eGON:
        statusText += "angle unit: gon   ";
        break;
    case UnitConverter::eRADIANT:
        statusText += "angle unit: radiant   ";
        break;
    case UnitConverter::eDECIMALDEGREE:
        statusText += "angle unit: decimal degree   ";
        break;
    default:
        break;
    }

    switch (UnitConverter::temperatureType) {
    case UnitConverter::eGRAD:
        statusText += "temperature unit: degree";
        break;
    default:
        break;
    }

    ui->statusBar->showMessage(statusText,0);
}

/*!
 * \brief shows the settings dialog
 */
void MainWindow::on_actionView_settings_triggered()
{
    setUpDialog.show();
}

/*!
 * \brief shows the dialog for creating scalar entities
 */
void MainWindow::on_actionCreate_scalar_entity_triggered()
{
    sEntityDialog->show();
}

/*!
 * \brief opens the import dialog for nominal features and sets all coordinate systems for the dialog.
 */
void MainWindow::on_actionNominal_geometry_triggered()
{
    importNominalDialog.setUpCoordinateSystems(control.coordSys);

    importNominalDialog.show();
}

/*!
 * \brief getActiveCoordSystem
 * \param coordSys
 */
void MainWindow::getActiveCoordSystem(QString coordSys){
    emit sendActiveCoordSystem(ui->comboBox_activeCoordSystem->currentText());
}

/*!
 * \brief saves the current job
 */
void MainWindow::on_actionSave_as_triggered()
{

    QString filename = QFileDialog::getSaveFileName(
                       this,
                       "Choose a filename to save under",
                       "oiProject",
                       "xml (*.xml)");




    oiProjectData data;
    data.device = new QFile(filename);

    QFileInfo info(filename);

    data.projectName = info.fileName();
    data.features = control.features;
    data.activeCoordSystem = control.activeCoordinateSystem;

    //bool isSuccessfull = OiDataImporter::saveToXML(control.features,file,control.activeCoordinateSystem->id);
    bool isSuccessfull = oiProjectExchanger::saveProject(data);

    if(isSuccessfull){
        QMessageBox::information(this,"save data", "Saving the data was successful.");
    }else{
        QMessageBox::information(this,"save data", "Saving the data was not successful.");
    }
}

/*!
 * \brief changedStation makes some new connects after changing a station
 */
void MainWindow::changedStation(){
    connect(control.activeStation->sensorPad, SIGNAL(recalcFeature(Feature*)), &control, SLOT(recalcFeature(Feature*)));
    connect(control.activeStation,SIGNAL(actionFinished(bool)),&sInfoDialog,SLOT(hideInfo(bool)));
}

/*!
 * \brief loads a job
 */
void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(
                       this,
                       "Choose a filename to save under",
                       "oiProject",
                       "xml (*.xml)");


    oiProjectData data;
    data.device = new QFile(filename);

    QFileInfo info(filename);

    data.projectName = info.fileName();

    //OiDataImporter::loadFromXML(control.features,file);
    bool isSuccessfull = oiProjectExchanger::loadProject(data);

    if(isSuccessfull){
        QMessageBox::information(this,"load project", "load "+info.fileName()+ " was successful.");
    }else{
        QMessageBox::information(this,"load project", "load "+info.fileName()+ "  was not successful.");
    }

    control.loadProjectData(data);

}

/*!
 * \brief showMessageBox shows a messagebox with title and message
 * used for signalizing sensor actions (e.g. measuring)
 * \param QString title
 * \param QString message
 */
void MainWindow::showMessageBox(QString title, QString message){
    QMessageBox::information(NULL, title, message);
}

/*!
 * \brief MainWindow::showMessageBoxForDecision
 * Show a message box to give the user the possibility to cancel a task
 * \param title
 * \param message
 * \return
 */
void MainWindow::showMessageBoxForDecision(QString title, QString message, OiFunctor *func){
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok){
        QVariantList result;
        result.append(true);
        (*func)(result);
    }else{
        QVariantList result;
        result.append(false);
        (*func)(result);
    }
}

/*!
 * \brief activates a station
 */
void MainWindow::on_actionActivate_station_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Selected station will be activated and current station will be deactivated!");
    msgBox.setInformativeText("Want to continue?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
        this->control.changeActiveStation();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}


void MainWindow::defaultCreateFeatureSettings()
{
    this->lineEditName->setText("");
    this->spinBoxNumber->setValue(1);
    this->checkBoxActual->setChecked(true);
    this->checkBoxCommonPoint->setChecked(false);
    this->checkBoxNominal->setChecked(false);
}

/*!
 * \brief MainWindow::deleteFeatureContextMenu
 * Is called whenever the user does a right-click on a feature in the table view and opens the context menu
 * \param point
 */
void MainWindow::featureContextMenu(const QPoint &point){
    //save model indices to delete later
    if(this->isTrafoParamSelected){
        this->featuresToDelete = this->ui->tableView_trafoParam->selectionModel()->selectedIndexes();
    }else{
        this->featuresToDelete = this->ui->tableView_data->selectionModel()->selectedIndexes();
    }

    //create menu and add delete action
    QMenu *menu = new QMenu();
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete selected feature(s)"), this, SLOT(deleteFeatures(bool)));

    //check wether the right click was done on the active feature
    QList<FeatureWrapper*> myFeatures;
    if(this->isTrafoParamSelected){
        QModelIndex selectedIndex = this->ui->tableView_trafoParam->indexAt(point);
        TrafoParamProxyModel *tableModel = static_cast<TrafoParamProxyModel*>(this->ui->tableView_trafoParam->model());
        if(tableModel != NULL){
            QModelIndexList myIndexList;
            myIndexList.append(selectedIndex);
            myFeatures = tableModel->getFeaturesAtIndices(myIndexList);
        }
    }else{
        QModelIndex selectedIndex = this->ui->tableView_data->indexAt(point);
        FeatureOvserviewProxyModel *tableModel = static_cast<FeatureOvserviewProxyModel*>(this->ui->tableView_data->model());
        if(tableModel != NULL){
            QModelIndexList myIndexList;
            myIndexList.append(selectedIndex);
            myFeatures = tableModel->getFeaturesAtIndices(myIndexList);
        }
    }
    if(this->control.activeFeature != NULL && this->control.activeFeature->getFeature() != NULL
            && this->control.activeFeature->getTypeOfFeature() != Configuration::eCoordinateSystemFeature
            && myFeatures.size() == 1 && myFeatures.at(0) != NULL && myFeatures.at(0)->getFeature() != NULL
            && myFeatures.at(0)->getFeature()->id == this->control.activeFeature->getFeature()->id){
        menu->addAction(QIcon(":/Images/icons/info.png"), QString("show properties of feature %1").arg(control.activeFeature->getFeature()->name), this, SLOT(showProperties(bool)));
    }

    menu->exec(this->ui->tableView_data->mapToGlobal(point));
}

/*!
 * \brief MainWindow::deleteFeature
 * Try to delete the features that were marked to delete
 * \param checked
 */
void MainWindow::deleteFeatures(bool checked){
    if(this->featuresToDelete.size() >= 0){

        //create new index list because of multiple indexes for one row
        QModelIndexList myIndices;
        QList<int> rows;
        for(int i = 0; i < this->featuresToDelete.size(); i++){
            if(!rows.contains(this->featuresToDelete.at(i).row())){
                myIndices.append(this->featuresToDelete.at(i));
                rows.append(this->featuresToDelete.at(i).row());
            }
        }

        //send delete features
        if(this->isTrafoParamSelected){
            TrafoParamProxyModel *tableModel = static_cast<TrafoParamProxyModel*>(this->ui->tableView_trafoParam->model());
            QList<FeatureWrapper*> myFeatures = tableModel->getFeaturesAtIndices(myIndices);
            emit this->sendDeleteFeatures(myFeatures);
        }else{
            FeatureOvserviewProxyModel *tableModel = static_cast<FeatureOvserviewProxyModel*>(this->ui->tableView_data->model());
            QList<FeatureWrapper*> myFeatures = tableModel->getFeaturesAtIndices(myIndices);
            emit this->sendDeleteFeatures(myFeatures);
        }

    }
}

/*!
 * \brief MainWindow::showProperties
 * Show properties of active feature
 * \param checked
 */
void MainWindow::showProperties(bool checked){
    //show dialog dependent on which type of feature was clicked
    if(this->control.activeFeature != NULL){
        if(this->control.activeFeature->getTypeOfFeature() == Configuration::eTrafoParamFeature){
            trafoParamDialog.getSelectedTrafoParam(this->control.activeFeature);
            trafoParamDialog.show();
        }else if(this->control.activeFeature->getGeometry() != NULL && this->control.activeFeature->getGeometry()->isNominal){
            emit sendActiveNominalfeature(this->control.activeFeature);
            nominalDialog.show();
        }else if(this->control.activeFeature->getCoordinateSystem() == NULL){
            fDataDialog.getActiveFeature(this->control.activeFeature);
            fDataDialog.show();
        }
    }
}

/*!
 * \brief MainWindow::resetFeatureSelection
 * Deselect all features in table view
 */
void MainWindow::resetFeatureSelection(){
    this->ui->tableView_data->clearSelection();
    this->ui->tableView_trafoParam->clearSelection();
}

/*!
 * \brief MainWindow::on_tableView_data_customContextMenuRequested
 * Context menu of table view with features requested
 * \param pos
 */
void MainWindow::on_tableView_data_customContextMenuRequested(const QPoint &pos)
{
    this->isTrafoParamSelected = false;
    this->featureContextMenu(pos);
}

/*!
 * \brief MainWindow::on_tableView_trafoParam_customContextMenuRequested
 * Context menu of table view with trafo params requested
 * \param pos
 */
void MainWindow::on_tableView_trafoParam_customContextMenuRequested(const QPoint &pos)
{
    this->isTrafoParamSelected = true;
    this->featureContextMenu(pos);
}

/*!
 * \brief MainWindow::availableGroupsChanged
 * Update group-comboBoxes
 * \param availableGroups
 */
void MainWindow::availableGroupsChanged(QMap<QString, int> availableGroups){
    QStringList groups = availableGroups.keys();
    this->comboBoxGroup->clear();
    this->comboBoxGroup->clearEditText();
    this->comboBoxGroup->addItems(groups);
    this->cFeatureDialog->availableGroupsChanged(groups);
    this->sEntityDialog->availableGroupsChanged(groups);

    QString activeGroup = this->ui->comboBox_groups->currentText();

    if(groups.contains(activeGroup)){
        this->ui->comboBox_groups->clear();
        this->ui->comboBox_groups->addItem("All Groups");
        this->ui->comboBox_groups->addItems(groups);
        this->ui->comboBox_groups->setCurrentText(activeGroup);
    }else{
        this->ui->comboBox_groups->clear();
        this->ui->comboBox_groups->addItem("All Groups");
        this->ui->comboBox_groups->addItems(groups);
        this->ui->comboBox_groups->setCurrentText("All Groups");
        this->control.tblModel->updateModel(this->control.activeFeature, this->control.activeStation);
    }
}

/*!
 * \brief MainWindow::on_comboBox_groups_currentIndexChanged
 * \param arg1
 */
void MainWindow::on_comboBox_groups_currentIndexChanged(const QString &arg1)
{
    FeatureOvserviewProxyModel *model = this->control.featureOverviewModel;
    if(model != NULL){
        model->activeGroupChanged(arg1);
    }
}

/*!
 * \brief MainWindow::updateGeometryIcons
 * Enable or Disable gemetry buttons dependent on the availability of corresponding plugins
 * \param availableGeometries
 */
void MainWindow::updateGeometryIcons(QStringList availableGeometries){
    this->comboBoxFeatureType->clear();

    if(availableGeometries.contains("point")){
        this->ui->actionCreate_point->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"point",Configuration::ePointFeature);
    }else{
        this->ui->actionCreate_point->setEnabled(false);
    }
    if(availableGeometries.contains("line")){
        this->ui->actionCreate_line->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"line",Configuration::eLineFeature);
    }else{
        this->ui->actionCreate_line->setEnabled(false);
    }
    if(availableGeometries.contains("plane")){
        this->ui->actionCreate_plane->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"plane",Configuration::ePlaneFeature);
    }else{
        this->ui->actionCreate_plane->setEnabled(false);
    }
    if(availableGeometries.contains("sphere")){
        this->ui->actionCreate_sphere->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"sphere",Configuration::eSphereFeature);
    }else{
        this->ui->actionCreate_sphere->setEnabled(false);
    }
    if(availableGeometries.contains("pointcloud")){
        this->ui->actionCreate_pointcloud->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"pointcloud",Configuration::ePointCloudFeature);
    }else{
        this->ui->actionCreate_pointcloud->setEnabled(false);
    }
    if(availableGeometries.contains("cone")){
        this->ui->actionCreate_cone->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"cone", Configuration::eConeFeature);
    }else{
        this->ui->actionCreate_cone->setEnabled(false);
    }
    if(availableGeometries.contains("circle")){
        this->ui->actionCreate_circle->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"circle",Configuration::eCircleFeature);
    }else{
        this->ui->actionCreate_circle->setEnabled(false);
    }
    if(availableGeometries.contains("cylinder")){
        this->ui->actionCreate_cylinder->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"cylinder",Configuration::eCylinderFeature);
    }else{
        this->ui->actionCreate_cylinder->setEnabled(false);
    }
    if(availableGeometries.contains("ellipsoid")){
        this->ui->actionCreate_ellipsoid->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"ellipsoid",Configuration::eEllipsoidFeature);
    }else{
        this->ui->actionCreate_ellipsoid->setEnabled(false);
    }
    if(availableGeometries.contains("hyperboloid")){
        this->ui->actionCreate_hyperboloid->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"hyperboloid",Configuration::eHyperboloidFeature);
    }else{
        this->ui->actionCreate_hyperboloid->setEnabled(false);
    }
    if(availableGeometries.contains("nurbs")){
        this->ui->actionCreate_nurbs->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"nurbs",Configuration::eNurbsFeature);
    }else{
        this->ui->actionCreate_nurbs->setEnabled(false);
    }
    if(availableGeometries.contains("paraboloid")){
        this->ui->actionCreate_paraboloid->setEnabled(true);
        this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"paraboloid",Configuration::eParaboloidFeature);
    }else{
        this->ui->actionCreate_paraboloid->setEnabled(false);
    }

    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"station",Configuration::eStationFeature);
    this->comboBoxFeatureType->insertItem(this->comboBoxFeatureType->count(),"coordinatesystem",Configuration::eCoordinateSystemFeature);
}

void MainWindow::on_actionShow_help_triggered()
{
#ifdef Q_OS_MAC
QDir appDir(qApp->applicationDirPath());
#endif


#ifdef Q_OS_WIN
QDir appDir(qApp->applicationDirPath());
#endif

#ifdef Q_OS_LINUX
QDir appDir(qApp->applicationDirPath());
#endif

    QString guidePath = appDir.absolutePath();
    guidePath.append("/doc/index.html");

    if(QDesktopServices::openUrl(QUrl::fromLocalFile(guidePath))){
        Console::addLine("guide opened");
    }else{
        Console::addLine("cannot open user guide");
    }

}

/*!
 * \brief MainWindow::showCreateFeatureDialog
 * After a create feature button was clicked this function is called to update and display the create feature dialog
 * \param featureType
 */
void MainWindow::showCreateFeatureDialog(Configuration::FeatureTypes featureType){
    this->cFeatureDialog->setAvailableFunctions(this->control.getAvailableCreateFunctions(featureType), this->control.getDefaultFunction(featureType));
    this->cFeatureDialog->show();
}
