#include "createfeature.h"
#include "ui_createfeature.h"

/*!
 * \brief CreateFeature constructor. Initiates the gui.
 * \param parent
 */
CreateFeature::CreateFeature(QList<FeatureWrapper*> &features, QWidget *parent) :
    QDialog(parent),featureList(features),
    ui(new Ui::CreateFeature)
{
    ui->setupUi(this);
    initGUI();
}

/*!
 * \brief CreateFeature destructor.
 */
CreateFeature::~CreateFeature()
{
    delete ui;
}

/*!
 * \brief receiveFeatureType enables all gui elements and functions for the selected feature type.
 * Enables or disables some GUI elemts depending from the feature type. The feature type is set by the selected feature in the
 * combobox in the main application or by clicking on a create feature button (e.g. the create point button) in the toolbar of the
 * main application.
 * \param Configuration::FeatureTypes fT
 */
void CreateFeature::receiveFeatureType(Configuration::FeatureTypes fT){
    this->typeOfFeature = fT;

    initGUI();

    switch(typeOfFeature){
    case Configuration::eCoordinateSystemFeature:{
        ui->toolButton_mConfig->setVisible(false);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        ui->checkBox_Nominal->setEnabled(false);
        ui->checkBox_Actual->setChecked(true);
        ui->checkBox_Actual->setEnabled(false);
        ui->checkBox_common->setEnabled(false);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        this->setWindowTitle("create coordinatesystem");
        break;
    }
    case Configuration::eTrafoParamFeature:{
        ui->toolButton_mConfig->setVisible(false);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(true);
        ui->label_startSystem->setVisible(true);
        ui->comboBox_destinationSystem->setVisible(true);
        ui->comboBox_startSystem->setVisible(true);
        ui->checkBox_Nominal->setEnabled(false);
        ui->checkBox_Actual->setChecked(true);
        ui->checkBox_Actual->setEnabled(false);
        ui->checkBox_common->setEnabled(false);
        ui->checkBox_movement->setVisible(true);
        ui->checkBox_movement->setEnabled(true);
        ui->checkBox_movement->setChecked(false);
        this->setWindowTitle("create transformation parameter");
        break;
    }
    case Configuration::ePlaneFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create plane");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::ePointFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create point");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::eLineFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create line");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::eStationFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create station");
        ui->checkBox_Nominal->setChecked(false);
        ui->checkBox_Nominal->setEnabled(false);
        ui->checkBox_Actual->setChecked(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::eSphereFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create sphere");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::eCylinderFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create cylinder");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::eConeFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create cone");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::eEllipsoidFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create ellipsoid");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::eHyperboloidFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create hyperboloid");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::eParaboloidFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create paraboloid");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::eNurbsFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create nurbs");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::ePointCloudFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create pointcloud");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    case Configuration::eCircleFeature:
        ui->toolButton_mConfig->setVisible(true);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create circle");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    default:{
        ui->toolButton_mConfig->setVisible(false);
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
        ui->label_destinationSystem->setVisible(false);
        ui->label_startSystem->setVisible(false);
        ui->comboBox_destinationSystem->setVisible(false);
        ui->comboBox_startSystem->setVisible(false);
        this->setWindowTitle("create feature");
        ui->checkBox_Nominal->setEnabled(true);
        ui->checkBox_Actual->setEnabled(true);
        ui->checkBox_common->setEnabled(true);
        ui->checkBox_movement->setVisible(false);
        ui->checkBox_movement->setEnabled(false);
        ui->checkBox_movement->setChecked(false);
        break;
    }
    }

    initGUI();
}

/*!
 * \brief create feature.
 * Saves the values set up in the GUI and emits them to the create feature function in the controller, that crates the feature,
 * adds it to the features list and displays it in the table model.
 */
void CreateFeature::on_toolButton_create_clicked()
{
    if(ui->spinBox_count->value()>0){

        int count = ui->spinBox_count->value();
        Configuration::FeatureTypes featureType = this->typeOfFeature;
        QString name = ui->lineEdit_name->text();
        bool isActual = ui->checkBox_Actual->isChecked();
        bool isNominal = ui->checkBox_Nominal->isChecked();
        QString group = this->ui->comboBox_group->currentText();
        bool isCommon = ui->checkBox_common->isChecked();
        CoordinateSystem *nominalSystem = NULL;
        QString function = this->ui->comboBox_function->currentText();
        bool isMovement = ui->checkBox_movement->isChecked();

        if(name == ""){
            return;
        }

        if (typeOfFeature != Configuration::eTrafoParamFeature){

            if(isNominal){

                for(int k=0; k<this->featureList.size();k++){
                    if(this->featureList.at(k)->getCoordinateSystem() != NULL &&
                            ui->comboBox_nominalSystem->currentText() == this->featureList.at(k)->getCoordinateSystem()->name){
                        nominalSystem = this->featureList.at(k)->getCoordinateSystem();
                    }
                }
            }

            FeatureAttributesExchange featureAttributes;
            featureAttributes.count = count;
            featureAttributes.featureType = featureType;
            featureAttributes.name = name;
            featureAttributes.group = group;
            featureAttributes.function = function;
            featureAttributes.actual = isActual;
            featureAttributes.nominal = isNominal;
            featureAttributes.common = isCommon;
            featureAttributes.nominalSystem = nominalSystem;

            emit createFeature(featureAttributes);

        }else{
            CoordinateSystem *from;
            CoordinateSystem *to;
            //if(ui->comboBox_destinationSystem->currentText() != ui->comboBox_startSystem->currentText()){
            //allow multiple parameter for same direction
            //also parameter from system to itself (neccessary for temperature compensation) !
                for(int i=0;i<this->featureList.size();i++){
                    if(featureList.at(i)->getCoordinateSystem() != NULL &&
                            ui->comboBox_destinationSystem->currentText() == featureList.at(i)->getCoordinateSystem()->name){
                        to = featureList.at(i)->getCoordinateSystem();
                    }
                    if(featureList.at(i)->getStation() != NULL &&
                            ui->comboBox_destinationSystem->currentText() == featureList.at(i)->getStation()->name){
                        to = featureList.at(i)->getStation()->coordSys;
                    }
                    if(featureList.at(i)->getCoordinateSystem() != NULL &&
                            ui->comboBox_startSystem->currentText() == featureList.at(i)->getCoordinateSystem()->name){
                        from = featureList.at(i)->getCoordinateSystem();
                    }
                    if(featureList.at(i)->getStation() != NULL &&
                            ui->comboBox_startSystem->currentText() == featureList.at(i)->getStation()->name){
                        from = featureList.at(i)->getStation()->coordSys;
                    }
                }
                if(isMovement){
                    from = to;
                }
                FeatureAttributesExchange featureAttributes;
                featureAttributes.count = count;
                featureAttributes.featureType = featureType;
                featureAttributes.name = name;
                featureAttributes.group = group;
                featureAttributes.function = function;
                featureAttributes.actual = isActual;
                featureAttributes.nominal = isNominal;
                featureAttributes.common = isCommon;
                featureAttributes.nominalSystem = nominalSystem;
                featureAttributes.startSystem = from;
                featureAttributes.destSystem = to;
                featureAttributes.isMovement = isMovement;

                emit createFeature(featureAttributes);
            //}else{
            //    return;
            //}
        }

        this->close();
    }

}
/*!
 * \brief closes this window without saving values or creating features.
 */
void CreateFeature::on_toolButton_cancel_clicked()
{
    this->close();
}

/*!
 * \brief initiates the gui and fills the coordinate system comboboxes with availables coordinate systems.
 */
void CreateFeature::initGUI(){

    ui->comboBox_destinationSystem->clear();
    ui->comboBox_startSystem->clear();
    ui->comboBox_nominalSystem->clear();
    ui->checkBox_Nominal->setChecked(false);
    ui->checkBox_Actual->setChecked(true);
    ui->checkBox_common->setChecked(false);
    ui->lineEdit_name->setText("");
    ui->spinBox_count->setValue(1);

    if(featureList.size() !=0){
        for(int i=0; i<featureList.size();i++){
            if(featureList.at(i)->getCoordinateSystem() != NULL){
                ui->comboBox_destinationSystem->addItem(featureList.at(i)->getCoordinateSystem()->name);
                ui->comboBox_startSystem->addItem(featureList.at(i)->getCoordinateSystem()->name);
                ui->comboBox_nominalSystem->addItem(featureList.at(i)->getCoordinateSystem()->name);
            }
            if(featureList.at(i)->getStation() != NULL){
                ui->comboBox_destinationSystem->addItem(featureList.at(i)->getStation()->name);
                ui->comboBox_startSystem->addItem(featureList.at(i)->getStation()->name);

            }
        }
    }
}

/*!
 * \brief this function is called when the nominal checkboxes gets toggled and generates specific gui elements for each case.
 * \param bool checked
 */
void CreateFeature::on_checkBox_Nominal_toggled(bool checked)
{
    if(checked){
        ui->label_nominalSystem->setVisible(true);
        ui->comboBox_nominalSystem->setVisible(true);
    }else{
        ui->label_nominalSystem->setVisible(false);
        ui->comboBox_nominalSystem->setVisible(false);
    }
}

/*!
 * \brief calls the measurement configuration window for creating a config.
 * This config will be saved for this feature and all following features. It can be changed via the main application.
 */
void CreateFeature::on_toolButton_mConfig_clicked()
{
    emit createFeatureMConfig();
}

/*!
 * \brief CreateFeature::availableGroupsChanged
 * Update group combobox
 * \param myGroups
 */
void CreateFeature::availableGroupsChanged(QStringList myGroups){
    this->ui->comboBox_group->clear();
    this->ui->comboBox_group->clearEditText();
    this->ui->comboBox_group->addItems(myGroups);
}

/*!
 * \brief CreateFeature::setAvailableFunctions
 * Assign all available create functions to the function combo box and select default value
 * \param functions
 * \param defaultFunction
 */
void CreateFeature::setAvailableFunctions(QStringList functions, QString defaultFunction){
    this->ui->comboBox_function->clear();
    this->ui->comboBox_function->addItems(functions);
    this->ui->comboBox_function->setCurrentText(defaultFunction);
}

void CreateFeature::showEvent(QShowEvent *event)
{
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move( screen.center() - this->rect().center() );
    event->accept();
}

/*!
 * \brief CreateFeature::on_checkBox_Actual_toggled
 * \param checked
 */
void CreateFeature::on_checkBox_Actual_toggled(bool checked)
{
    if(checked){
        this->ui->comboBox_function->setEnabled(true);
    }else{
        this->ui->comboBox_function->setEnabled(false);
    }
}

void CreateFeature::on_checkBox_movement_toggled(bool checked)
{
    if(checked){
        ui->comboBox_startSystem->setEnabled(false);
    }else{
        ui->comboBox_startSystem->setEnabled(true);
    }
}

void CreateFeature::on_comboBox_destinationSystem_currentIndexChanged(const QString &arg1)
{
    if(ui->checkBox_movement->isChecked()){
        ui->comboBox_startSystem->setCurrentText(arg1);
    }
}
