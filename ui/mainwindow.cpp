#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

    //assign models of ModelManager to GUI-elements
    this->assignModels();

    //init GUI elements
    this->initFeatureTableViews();
    this->initSensorPad();
    this->initToolMenus();

    //connect controller and dialogs
    this->connectDialogs();
    this->connectController();

    //create default job and pass it to the watch window
    QPointer<OiJob> job = this->control.createDefaultJob();
    this->watchWindowDialog.setCurrentJob(job);

}

/*!
 * \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(){
    delete ui;
}

/*!
 * \brief MainWindow::importNominalsStarted
 * Triggered by Controller whenever an nominal import task has been started
 */
void MainWindow::importNominalsStarted(){
    this->loadingDialog.show();
}

/*!
 * \brief MainWindow::importNominalsFinished
 * Triggered by Controller whenever an import nominals task has been finished
 * \param success
 */
void MainWindow::importNominalsFinished(const bool &success){

    //print import success to console
    if(success){
        Console::getInstance()->addLine("Nominals successfully imported");
    }else{
        Console::getInstance()->addLine("Nominals not imported successfully");
    }

    this->loadingDialog.close();

}

/*!
 * \brief MainWindow::activeFeatureChanged
 */
void MainWindow::activeFeatureChanged(){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = sourceModel->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //update magnify window
    this->ui->label_magnifyName->setText(feature->getFeature()->getFeatureName());
    if(!feature->getGeometry().isNull()){
        this->ui->label_magnifyActualNominal->setText(feature->getGeometry()->getIsNominal()?"nominal":"actual");
    }
    this->ui->label_magnifyActualNominal->setText("actual");

}

/*!
 * \brief MainWindow::activeCoordinateSystemChanged
 */
void MainWindow::activeCoordinateSystemChanged(){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check current job
    QPointer<OiJob> job = sourceModel->getCurrentJob();
    if(job.isNull()){
        return;
    }

    //update combo box with active coordinate system
    if(!job->getActiveCoordinateSystem().isNull()){
        this->ui->comboBox_activeCoordSystem->setCurrentText(job->getActiveCoordinateSystem()->getFeatureName());
    }

}

void MainWindow::coordSystemSetChanged()
{

}

void MainWindow::stationSetChanged()
{

}

void MainWindow::availableGroupsChanged()
{

}

void MainWindow::activeGroupChanged()
{

}

/*!
 * \brief MainWindow::stationSensorChanged
 * Triggered whenever the sensor of the active station has changed
 * \param featureId
 */
void MainWindow::stationSensorChanged(const int &featureId){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get type of sensor
    QPointer<Station> activeStation = sourceModel->getActiveStation();
    if(activeStation.isNull()){
        return;
    }

    //set sensor action's visibility depending on the sensor type
    this->activeSensorTypeChanged(activeStation->getActiveSensorType(), activeStation->getSupportedSensorActions(),
                                  activeStation->getSelfDefinedActions());

}

/*!
 * \brief MainWindow::sensorActionStarted
 * \param name
 */
void MainWindow::sensorActionStarted(const QString &name){
    this->sensorTaskInfoDialog.setDisplayMessage(name);
    this->sensorTaskInfoDialog.show();
}

/*!
 * \brief MainWindow::sensorActionFinished
 * \param success
 * \param msg
 */
void MainWindow::sensorActionFinished(const bool &success, const QString &msg){
    this->sensorTaskInfoDialog.close();
    Console::getInstance()->addLine(msg);
}

/*!
 * \brief MainWindow::on_actionCreate_point_triggered
 */
void MainWindow::on_actionCreate_point_triggered(){
    this->createFeatureDialog.setFeatureType(ePointFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_line_triggered
 */
void MainWindow::on_actionCreate_line_triggered(){
    this->createFeatureDialog.setFeatureType(eLineFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_plane_triggered
 */
void MainWindow::on_actionCreate_plane_triggered(){
    this->createFeatureDialog.setFeatureType(ePlaneFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_sphere_triggered
 */
void MainWindow::on_actionCreate_sphere_triggered(){
    this->createFeatureDialog.setFeatureType(eSphereFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_station_triggered
 */
void MainWindow::on_actionCreate_station_triggered(){
    this->createFeatureDialog.setFeatureType(eStationFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_coordinatesystem_triggered
 */
void MainWindow::on_actionCreate_coordinatesystem_triggered(){
    this->createFeatureDialog.setFeatureType(eCoordinateSystemFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_scalar_entity_triggered
 */
void MainWindow::on_actionCreate_scalar_entity_triggered(){
    this->createFeatureDialog.setFeatureType(eScalarEntityAngleFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_trafoParam_triggered
 */
void MainWindow::on_actionCreate_trafoParam_triggered(){
    this->createFeatureDialog.setFeatureType(eTrafoParamFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_cone_triggered
 */
void MainWindow::on_actionCreate_cone_triggered(){
    this->createFeatureDialog.setFeatureType(eConeFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_cylinder_triggered
 */
void MainWindow::on_actionCreate_cylinder_triggered(){
    this->createFeatureDialog.setFeatureType(eCylinderFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_ellipsoid_triggered
 */
void MainWindow::on_actionCreate_ellipsoid_triggered(){
    this->createFeatureDialog.setFeatureType(eEllipsoidFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_hyperboloid_triggered
 */
void MainWindow::on_actionCreate_hyperboloid_triggered(){
    this->createFeatureDialog.setFeatureType(eHyperboloidFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_paraboloid_triggered
 */
void MainWindow::on_actionCreate_paraboloid_triggered(){
    this->createFeatureDialog.setFeatureType(eParaboloidFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_nurbs_triggered
 */
void MainWindow::on_actionCreate_nurbs_triggered(){
    this->createFeatureDialog.setFeatureType(eNurbsFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_pointcloud_triggered
 */
void MainWindow::on_actionCreate_pointcloud_triggered(){
    this->createFeatureDialog.setFeatureType(ePointCloudFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_circle_triggered
 */
void MainWindow::on_actionCreate_circle_triggered(){
    this->createFeatureDialog.setFeatureType(eCircleFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_torus_triggered
 */
void MainWindow::on_actionCreate_torus_triggered(){
    this->createFeatureDialog.setFeatureType(eTorusFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_slotted_hole_triggered
 */
void MainWindow::on_actionCreate_slotted_hole_triggered(){
    this->createFeatureDialog.setFeatureType(eSlottedHoleFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionCreate_ellipse_triggered
 */
void MainWindow::on_actionCreate_ellipse_triggered(){
    this->createFeatureDialog.setFeatureType(eEllipseFeature);
    this->createFeatureDialog.show();
}

/*!
 * \brief MainWindow::on_actionLoad_plugins_triggered
 */
void MainWindow::on_actionLoad_plugins_triggered(){
    this->pluginLoaderDialog.show();
}

/*!
 * \brief MainWindow::on_actionPlugin_manager_triggered
 */
void MainWindow::on_actionPlugin_manager_triggered(){
    this->pluginManagerDialog.show();
}

/*!
 * \brief MainWindow::on_action_importNominals_triggered
 */
void MainWindow::on_action_importNominals_triggered(){
    this->importNominalDialog.show();
}

/*!
 * \brief MainWindow::on_tableView_features_clicked
 * \param index
 */
void MainWindow::on_tableView_features_clicked(const QModelIndex &index){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //set active feature
    sourceModel->setActiveFeature(model->mapToSource(index));

}

/*!
 * \brief MainWindow::on_tableView_features_customContextMenuRequested
 * \param pos
 */
void MainWindow::on_tableView_features_customContextMenuRequested(const QPoint &pos){

    //create menu and add delete action
    QMenu *menu = new QMenu();
    menu->addAction(QIcon(":/Images/icons/edit_remove.png"), QString("delete selected feature(s)"), this, SLOT(deleteFeatures(bool)));

    //get feature table models
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel*>(this->ui->tableView_features->model());
    if(model == NULL){
        delete menu;
        return;
    }
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel*>(model->sourceModel());
    if(sourceModel == NULL){
        delete menu;
        return;
    }

    //get the selected index (where the right click was done)
    QModelIndex selectedIndex = this->ui->tableView_features->indexAt(pos);

    //get and check the feature at that index
    int id = sourceModel->getFeatureIdAtIndex(model->mapToSource(selectedIndex));
    QPointer<OiJob> job = sourceModel->getCurrentJob();
    if(job.isNull() || id < 0){
        delete menu;
        return;
    }
    QPointer<FeatureWrapper> selectedFeature = job->getFeatureById(id);
    if(selectedFeature.isNull() || selectedFeature->getFeature().isNull()){
        delete menu;
        return;
    }

    //if the selected feature is the active feature
    if(selectedFeature->getFeature()->getIsActiveFeature()){

        menu->addAction(QIcon(":/Images/icons/info.png"), QString("show properties of feature %1").arg(selectedFeature->getFeature()->getFeatureName()),
                        this, SLOT(showFeatureProperties(bool)));

    }

    menu->exec(this->ui->tableView_features->mapToGlobal(pos));

}

/*!
 * \brief MainWindow::deleteFeatures
 * \param checked
 */
void MainWindow::deleteFeatures(bool checked){



}

/*!
 * \brief MainWindow::showFeatureProperties
 * \param checked
 */
void MainWindow::showFeatureProperties(bool checked){

    //get feature table models
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel*>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel*>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check active feature
    QPointer<FeatureWrapper> feature = sourceModel->getActiveFeature();
    if(feature.isNull() || feature->getFeature().isNull()){
        return;
    }

    //depending on the type of feature display a special properties dialog
    if(!feature->getGeometry().isNull() && !feature->getGeometry()->getIsNominal()){
        this->actualPropertiesDialog.show();
    }else if(!feature->getGeometry().isNull() && feature->getGeometry()->getIsNominal()){
        this->nominalPropertiesDialog.show();
    }

}

/*!
 * \brief MainWindow::on_tableView_trafoParams_clicked
 * \param index
 */
void MainWindow::on_tableView_trafoParams_clicked(const QModelIndex &index){

    //get and check model
    TrafoParamTableProxyModel *model = static_cast<TrafoParamTableProxyModel *>(this->ui->tableView_trafoParams->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //set active feature
    sourceModel->setActiveFeature(model->mapToSource(index));

}

/*!
 * \brief MainWindow::on_actionSet_function_triggered
 */
void MainWindow::on_actionSet_function_triggered(){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    if(!sourceModel->getActiveFeature().isNull()){
        this->featureFunctionsDialog.show();
    }

}

/*!
 * \brief MainWindow::on_actionSet_instrument_triggered
 */
void MainWindow::on_actionSet_instrument_triggered(){

    this->sensorConfigurationDialog.show();

}

/*!
 * \brief MainWindow::setSensorConfiguration
 * \param name
 */
void MainWindow::setSensorConfiguration(const QString &name){

    //check name
    if(name.compare("") == 0){
        Console::getInstance()->addLine("Invalid configuration name");
        return;
    }

    //ask the user wether only to set the sensor config or to also connect to the sensor
    QMessageBox msgBox;
    msgBox.setText("Do you want to connect the sensor?");
    msgBox.setInformativeText("");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Yes:
        emit this->sensorConfigurationChanged(name, true);
        break;
    case QMessageBox::No:
        emit this->sensorConfigurationChanged(name, false);
        break;
    }

}

/*!
 * \brief MainWindow::showMoveSensorDialog
 */
void MainWindow::showMoveSensorDialog(){
    this->moveSensorDialog.show();
}

/*!
 * \brief MainWindow::on_actionControl_pad_triggered
 */
void MainWindow::on_actionControl_pad_triggered(){
    if(this->ui->toolBar_controlPad->isVisible()){
        this->ui->toolBar_controlPad->setVisible(false);
    }else{
        this->ui->toolBar_controlPad->setVisible(true);
    }
}

/*!
 * \brief MainWindow::on_comboBox_groups_currentIndexChanged
 * \param arg1
 */
void MainWindow::on_comboBox_groups_currentIndexChanged(const QString &arg1){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //check group name
    if(arg1.compare("") == 0){
        this->ui->comboBox_groups->setCurrentText(sourceModel->getActiveGroupName());
    }else{
        sourceModel->setActiveGroupName(arg1);
    }

}

/*!
 * \brief MainWindow::on_comboBox_activeCoordSystem_currentIndexChanged
 * \param arg1
 */
void MainWindow::on_comboBox_activeCoordSystem_currentIndexChanged(const QString &arg1){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //check system name
    if(arg1.compare("") == 0){
        QPointer<CoordinateSystem> activeSystem = sourceModel->getActiveCoordinateSystem();
        if(!activeSystem.isNull()){
            this->ui->comboBox_activeCoordSystem->setCurrentText(activeSystem->getFeatureName());
        }
    }else{
        sourceModel->setActiveCoordinateSystem(arg1);
    }

}

/*!
 * \brief MainWindow::on_actionWatch_window_triggered
 */
void MainWindow::on_actionWatch_window_triggered(){
    this->watchWindowDialog.show();
}

/*!
 * \brief MainWindow::on_actionOpen_triggered
 */
void MainWindow::on_actionOpen_triggered(){

    QString filename = QFileDialog::getOpenFileName(this, "Choose a filename", "oiProject", "xml (*.xml)");
    if(filename.compare("") == 0){
        return;
    }

    QPointer<QIODevice> device = new QFile(filename);
    QFileInfo info(filename);
    QString projectName = info.fileName();

    emit this->loadProject(projectName, device);

}

/*!
 * \brief MainWindow::on_actionSave_triggered
 */
void MainWindow::on_actionSave_triggered(){

}

/*!
 * \brief MainWindow::on_actionSave_as_triggered
 */
void MainWindow::on_actionSave_as_triggered(){
    QString filename = QFileDialog::getSaveFileName(this, "Choose a filename to save under", "oiProject", "xml (*.xml)");
    if(filename.compare("") != 0){
        emit this->saveProject(filename);
    }
}

/*!
 * \brief MainWindow::on_actionClose_triggered
 */
void MainWindow::on_actionClose_triggered(){
    this->close();
}

/*!
 * \brief MainWindow::on_actionMeasurement_Configuration_triggered
 */
void MainWindow::on_actionMeasurement_Configuration_triggered(){
    this->measurementConfigDialog.show();
}

/*!
 * \brief MainWindow::on_actionActivate_station_triggered
 */
void MainWindow::on_actionActivate_station_triggered(){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get and check selected index
    QModelIndexList selection = this->ui->tableView_features->selectionModel()->selectedIndexes();
    if(selection.size() != 1){
        Console::getInstance()->addLine("No station selected");
        return;
    }
    QModelIndex index = selection.at(0);

    //set selected station
    sourceModel->setActiveStation(model->mapToSource(index));

}

/*!
 * \brief MainWindow::on_actionView_settings_triggered
 */
void MainWindow::on_actionView_settings_triggered(){
    this->settingsDialog.show();
}

/*!
 * \brief MainWindow::showToolWidget
 * \param pluginName
 * \param toolName
 */
void MainWindow::showToolWidget(const QString &pluginName, const QString &toolName){

    //get and check model
    FeatureTableProxyModel *model = static_cast<FeatureTableProxyModel *>(this->ui->tableView_features->model());
    if(model == NULL){
        return;
    }

    //get and check source model
    FeatureTableModel *sourceModel = static_cast<FeatureTableModel *>(model->sourceModel());
    if(sourceModel == NULL){
        return;
    }

    //get a list of available tool plugins
    QList<QPointer<Tool> > tools = this->control.getAvailableTools();

    //search the list for the specified tool
    foreach(const QPointer<Tool> &tool, tools){

        if(tool.isNull()){
            continue;
        }

        if(tool->getMetaData().pluginName.compare(pluginName) == 0
                && tool->getMetaData().name.compare(toolName) == 0){
            tool->setJob(sourceModel->getCurrentJob());
            tool->show();
            continue;
        }else{
            delete tool;
        }

    }

}

/*!
 * \brief MainWindow::connectController
 */
void MainWindow::connectController(){

    //connect actions triggered by user to slots in controller
    QObject::connect(this, &MainWindow::addFeatures, &this->control, &Controller::addFeatures, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::importNominals, &this->control, &Controller::importNominals, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::sensorConfigurationChanged, &this->control, &Controller::sensorConfigurationChanged, Qt::AutoConnection);
    QObject::connect(this, &MainWindow::measurementConfigurationChanged, &this->control, &Controller::measurementConfigurationChanged, Qt::AutoConnection);
    QObject::connect(this, static_cast<void (MainWindow::*)()>(&MainWindow::saveProject),
                     &this->control, static_cast<void (Controller::*)()>(&Controller::saveProject), Qt::AutoConnection);
    QObject::connect(this, static_cast<void (MainWindow::*)(const QString&)>(&MainWindow::saveProject),
                     &this->control, static_cast<void (Controller::*)(const QString&)>(&Controller::saveProject), Qt::AutoConnection);
    QObject::connect(this, &MainWindow::loadProject, &this->control, &Controller::loadProject, Qt::AutoConnection);
    //QObject::connect(this, &MainWindow::setActiveFeature, &this->control, &Controller::setActiveFeature, Qt::AutoConnection);

    //connect actions triggered by controller to slots in main window
    QObject::connect(&this->control, &Controller::nominalImportStarted, this, &MainWindow::importNominalsStarted, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::nominalImportFinished, this, &MainWindow::importNominalsFinished, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::activeFeatureChanged, this, &MainWindow::activeFeatureChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::activeCoordinateSystemChanged, this, &MainWindow::activeCoordinateSystemChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::stationSensorChanged, this, &MainWindow::stationSensorChanged, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::sensorActionStarted, this, &MainWindow::sensorActionStarted, Qt::AutoConnection);
    QObject::connect(&this->control, &Controller::sensorActionFinished, this, &MainWindow::sensorActionFinished, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::connectDialogs
 */
void MainWindow::connectDialogs(){

    //connect create feature dialog
    QObject::connect(&this->createFeatureDialog, &CreateFeatureDialog::addFeatures, this, &MainWindow::addFeatures, Qt::AutoConnection);

    //connect console
    QObject::connect(Console::getInstance().data(), &Console::lineAdded, this->ui->listView_console, &QListView::scrollToBottom, Qt::AutoConnection);

    //connect import dialogs
    QObject::connect(&this->importNominalDialog, &ImportNominalDialog::startImport, this, &MainWindow::importNominals, Qt::AutoConnection);

    //connect loading dialog
    QObject::connect(&this->control, &Controller::nominalImportProgressUpdated, &this->loadingDialog, &LoadingDialog::updateProgress, Qt::AutoConnection);

    //connect sensor config dialog
    QObject::connect(&this->sensorConfigurationDialog, &SensorConfigurationDialog::setSensorConfiguration, this, &MainWindow::setSensorConfiguration, Qt::AutoConnection);

    //connect measurement config dialog
    QObject::connect(&this->measurementConfigDialog, &MeasurementConfigurationDialog::setMeasurementConfiguration, this, &MainWindow::measurementConfigurationChanged, Qt::AutoConnection);

    //connect move sensor dialog
    QObject::connect(&this->moveSensorDialog, &MoveSensorDialog::moveSensor, &this->control, &Controller::startMove, Qt::AutoConnection);

    //connect plugin manager dialog
    QObject::connect(&this->pluginManagerDialog, &PluginManagerDialog::loadPlugins, &this->pluginLoaderDialog, &PluginLoaderDialog::show, Qt::AutoConnection);

    //connect feature functions dialog
    QObject::connect(&this->featureFunctionsDialog, &FeatureFunctionsDialog::recalculateActiveFeature, &this->control, &Controller::recalcActiveFeature, Qt::AutoConnection);

    //connect settings dialog
    QObject::connect(&this->settingsDialog, &SettingsDialog::setDisplayConfig, &this->control, &Controller::setParameterDisplayConfig, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::assignModels
 * Assign the models of ModelManager to GUI-elements in MainWindow
 */
void MainWindow::assignModels(){

    //assign feature table views and delegates
    this->ui->tableView_features->setModel(&ModelManager::getFeatureTableProxyModel());
    FeatureTableDelegate *featureTableDelegate = new FeatureTableDelegate();
    this->ui->tableView_features->setItemDelegate(featureTableDelegate);
    this->ui->tableView_trafoParams->setModel(&ModelManager::getTrafoParamTableProxyModel());
    TrafoParamDelegate *trafoParamTableDelegate = new TrafoParamDelegate();
    this->ui->tableView_trafoParams->setItemDelegate(trafoParamTableDelegate);

    //assign console model
    this->ui->listView_console->setModel(&Console::getInstance()->getConsoleModel());

    //assign groups model
    this->ui->comboBox_groups->setModel(&ModelManager::getGroupNamesModel());

    //assign coordinate system model
    this->ui->comboBox_activeCoordSystem->setModel(&ModelManager::getCoordinateSystemsModel());

}

/*!
 * \brief MainWindow::initFeatureTableViews
 */
void MainWindow::initFeatureTableViews(){

    //resize rows and columns to table view contents
    this->ui->tableView_features->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //this->ui->tableView_features->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    this->ui->tableView_features->verticalHeader()->setDefaultSectionSize(22);
    this->ui->tableView_trafoParams->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->tableView_trafoParams->verticalHeader()->setDefaultSectionSize(22);

    //enable context menu
    this->ui->tableView_features->setContextMenuPolicy(Qt::CustomContextMenu);
    this->ui->tableView_trafoParams->setContextMenuPolicy(Qt::CustomContextMenu);

}

/*!
 * \brief MainWindow::initSensorPad
 */
void MainWindow::initSensorPad(){

    //clear sensor pad and hide it as default
    this->ui->toolBar_controlPad->clear();
    this->ui->toolBar_controlPad->setVisible(false);

    //initialize sensor actions
    this->actionConnect = new QAction(0);
    this->actionConnect->setText("connect");
    this->actionDisconnect = new QAction(0);
    this->actionDisconnect->setText("disconnect");
    this->actionInitialize = new QAction(0);
    this->actionInitialize->setText("initialize");
    this->actionMeasure = new QAction(0);
    this->actionMeasure->setShortcut(Qt::Key_F3);
    this->actionMeasure->setText("measure");
    this->actionAim = new QAction(0);
    this->actionAim->setShortcut(QKeySequence(Qt::ALT + Qt::Key_A));
    this->actionAim->setText("aim");
    this->actionMove = new QAction(0);
    this->actionMove->setText("move");
    this->actionHome = new QAction(0);
    this->actionHome->setShortcut(Qt::Key_F9);
    this->actionHome->setText("home");
    this->actionChangeMotorState = new QAction(0);
    this->actionChangeMotorState->setText("change motor state");
    this->actionToggleSightOrientation = new QAction(0);
    this->actionToggleSightOrientation->setText("toggle sight orientation");
    this->actionCompensation = new QAction(0);
    this->actionCompensation->setText("compensation");

    //add the actions to the sensor pad
    this->ui->toolBar_controlPad->addAction(this->actionConnect);
    this->ui->toolBar_controlPad->addAction(this->actionDisconnect);
    this->ui->toolBar_controlPad->addAction(this->actionMeasure);
    this->ui->toolBar_controlPad->addAction(this->actionAim);
    this->ui->toolBar_controlPad->addAction(this->actionMove);
    this->ui->toolBar_controlPad->addAction(this->actionHome);
    this->ui->toolBar_controlPad->addAction(this->actionChangeMotorState);
    this->ui->toolBar_controlPad->addAction(this->actionToggleSightOrientation);
    this->ui->toolBar_controlPad->addAction(this->actionCompensation);

    //disable and hide actions as default
    this->actionConnect->setVisible(false);
    this->actionConnect->setEnabled(false);
    this->actionDisconnect->setVisible(false);
    this->actionDisconnect->setEnabled(false);
    this->actionMeasure->setVisible(false);
    this->actionMeasure->setEnabled(false);
    this->actionAim->setVisible(false);
    this->actionAim->setEnabled(false);
    this->actionMove->setVisible(false);
    this->actionMove->setEnabled(false);
    this->actionHome->setVisible(false);
    this->actionHome->setEnabled(false);
    this->actionChangeMotorState->setVisible(false);
    this->actionChangeMotorState->setEnabled(false);
    this->actionToggleSightOrientation->setVisible(false);
    this->actionToggleSightOrientation->setEnabled(false);
    this->actionCompensation->setVisible(false);
    this->actionCompensation->setEnabled(false);

    //connect actions
    QObject::connect(this->actionConnect, &QAction::triggered, &this->control, &Controller::startConnect, Qt::AutoConnection);
    QObject::connect(this->actionDisconnect, &QAction::triggered, &this->control, &Controller::startDisconnect, Qt::AutoConnection);
    QObject::connect(this->actionMeasure, &QAction::triggered, &this->control, &Controller::startMeasurement, Qt::AutoConnection);
    QObject::connect(this->actionAim, &QAction::triggered, &this->control, &Controller::startAim, Qt::AutoConnection);
    QObject::connect(this->actionHome, &QAction::triggered, &this->control, &Controller::startHome, Qt::AutoConnection);
    QObject::connect(this->actionChangeMotorState, &QAction::triggered, &this->control, &Controller::startChangeMotorState, Qt::AutoConnection);
    QObject::connect(this->actionToggleSightOrientation, &QAction::triggered, &this->control, &Controller::startToggleSight, Qt::AutoConnection);
    QObject::connect(this->actionCompensation, &QAction::triggered, &this->control, &Controller::startCompensation, Qt::AutoConnection);
    QObject::connect(this->actionMove, &QAction::triggered, this, &MainWindow::showMoveSensorDialog, Qt::AutoConnection);

}

/*!
 * \brief MainWindow::initToolMenus
 */
void MainWindow::initToolMenus(){

    //get all available tool plugins
    QList<QPointer<Tool> > tools = this->control.getAvailableTools();

    //add each tool as menu entry
    foreach(const QPointer<Tool> &tool, tools){

        if(tool.isNull()){
            continue;
        }

        //create an action and add it to the corresponding menu item
        ToolAction *action;
        action = new ToolAction();
        action->setToolName(tool->getMetaData().name);
        action->setPluginName(tool->getMetaData().pluginName);
        action->setText(tool->getMetaData().name);
        switch(tool->getToolType()){
        case eReportTool:
            this->ui->menuReport->addAction(action);
            break;
        case eUnknownTool:
            this->ui->menuTools->addAction(action);
            break;
        }

        //connect the triggered event of the action
        QObject::connect(action, &ToolAction::openToolWidget, this, &MainWindow::showToolWidget);

        //delete the tool
        delete tool;

    }

}

/*!
 * \brief MainWindow::activeSensorTypeChanged
 * Depending on the active stations's sensor set visibility of sensor pad actions
 * \param type
 * \param supportedActions
 * \param selfDefinedActions
 */
void MainWindow::activeSensorTypeChanged(const SensorTypes &type, const QList<SensorFunctions> &supportedActions, const QStringList &selfDefinedActions){

    //set visibility of standard actions
    switch(type){
    case eLaserTracker:

        this->actionConnect->setVisible(true);
        this->actionConnect->setEnabled(true);
        this->actionDisconnect->setVisible(true);
        this->actionDisconnect->setEnabled(true);
        this->actionMeasure->setVisible(true);
        this->actionMeasure->setEnabled(true);
        this->actionAim->setVisible(true);
        this->actionAim->setEnabled(true);
        this->actionMove->setVisible(true);
        this->actionMove->setEnabled(true);
        this->actionHome->setVisible(true);
        this->actionHome->setEnabled(true);
        this->actionChangeMotorState->setVisible(true);
        this->actionChangeMotorState->setEnabled(true);
        this->actionToggleSightOrientation->setVisible(true);
        this->actionToggleSightOrientation->setEnabled(true);
        this->actionCompensation->setVisible(true);
        this->actionCompensation->setEnabled(true);

        break;

    case eTotalStation:

        this->actionConnect->setVisible(true);
        this->actionConnect->setEnabled(true);
        this->actionDisconnect->setVisible(true);
        this->actionDisconnect->setEnabled(true);
        this->actionMeasure->setVisible(true);
        this->actionMeasure->setEnabled(true);
        this->actionAim->setVisible(true);
        this->actionAim->setEnabled(true);
        this->actionMove->setVisible(true);
        this->actionMove->setEnabled(true);
        this->actionHome->setVisible(false);
        this->actionHome->setEnabled(false);
        this->actionChangeMotorState->setVisible(false);
        this->actionChangeMotorState->setEnabled(false);
        this->actionToggleSightOrientation->setVisible(true);
        this->actionToggleSightOrientation->setEnabled(true);
        this->actionCompensation->setVisible(false);
        this->actionCompensation->setEnabled(false);

        break;

    default:

        this->actionConnect->setVisible(false);
        this->actionConnect->setEnabled(false);
        this->actionDisconnect->setVisible(false);
        this->actionDisconnect->setEnabled(false);
        this->actionMeasure->setVisible(false);
        this->actionMeasure->setEnabled(false);
        this->actionAim->setVisible(false);
        this->actionAim->setEnabled(false);
        this->actionMove->setVisible(false);
        this->actionMove->setEnabled(false);
        this->actionHome->setVisible(false);
        this->actionHome->setEnabled(false);
        this->actionChangeMotorState->setVisible(false);
        this->actionChangeMotorState->setEnabled(false);
        this->actionToggleSightOrientation->setVisible(false);
        this->actionToggleSightOrientation->setEnabled(false);
        this->actionCompensation->setVisible(false);
        this->actionCompensation->setEnabled(false);

        break;

    }

    //delete old self defined actions
    foreach(const QPointer<QAction> &action, this->selfDefinedActions){
        if(!action.isNull()){
            this->ui->toolBar_controlPad->removeAction(action.data());
            delete action;
        }
    }
    this->selfDefinedActions.clear();

    //delete old signal mapper
    if(!this->customActionMapper.isNull()){
        delete this->customActionMapper;
    }
    this->customActionMapper = new QSignalMapper();

    //add new self defined actions
    foreach(const QString &action, selfDefinedActions){
        QPointer<QAction> customAction = new QAction(0);
        customAction->setText(action);
        this->selfDefinedActions.append(customAction);
        this->ui->toolBar_controlPad->addAction(customAction.data());
        QObject::connect(customAction.data(), SIGNAL(triggered()), this->customActionMapper.data(), SLOT(map()), Qt::AutoConnection);
        this->customActionMapper->setMapping(customAction.data(), action);
    }
    QObject::connect(this->customActionMapper.data(), SIGNAL(mapped(const QString&)), &this->control, SLOT(startCustomAction(const QString&)), Qt::AutoConnection);

}
