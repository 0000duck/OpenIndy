#include "oisimulationwidget.h"
#include "ui_oisimulationwidget.h"

OiSimulationWidget::OiSimulationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OiSimulationWidget)
{
    ui->setupUi(this);

    masterLayout = NULL;
    resultModel = new QStringListModel();


    this->ui->listView_simulations->setModel(this->control.availableSimulations);

    this->ui->comboBox_typeOfUnit->addItem("position");
    this->ui->comboBox_typeOfUnit->addItem("direction");

    this->ui->tableView_sensor->setModel(control.sensorErrorModel);
    this->ui->tableView_object->setModel(control.objectErrorModel);
    this->ui->tableView_humanInfluence->setModel(control.humanErrorModel);
    this->ui->tableView_environmet->setModel(control.environmentErrorModel);
    this->ui->treeView_feature->setModel(control.resultModel);

    SimulationDelegate *errorDelegate = new SimulationDelegate();
    this->ui->tableView_sensor->setItemDelegate(errorDelegate);

    SimulationDelegate *objectErrorDelegate = new SimulationDelegate();
    this->ui->tableView_object->setItemDelegate(objectErrorDelegate);

    SimulationDelegate *environmetErrorDelegate = new SimulationDelegate();
    this->ui->tableView_environmet->setItemDelegate(environmetErrorDelegate);

    SimulationDelegate *humanErrorDelegate = new SimulationDelegate();
    this->ui->tableView_humanInfluence->setItemDelegate(humanErrorDelegate);

    connect(this,SIGNAL(startSimulation()),&this->control,SLOT(recalcAll()));
    connect(&this->control,SIGNAL(counter(int)),this->ui->progressBar,SLOT(setValue(int)));

    control.moveToThread(&workerThread);

    workerThread.start();

}

OiSimulationWidget::~OiSimulationWidget()
{
    delete ui;
}

void OiSimulationWidget::setFeatureUpdater(FeatureUpdater *f)
{
    control.setFeatureUpdater(f);
}


void OiSimulationWidget::on_pushButton_startSimulation_clicked()
{

    this->ui->progressBar->setMaximum(ui->spinBox_iteration->value());
    this->ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);

    control.setIterations(ui->spinBox_iteration->value());

    this->setSettings();

    if(control.actualSimulation == NULL){

        QMessageBox::information(NULL, "no simulation set", "please choose your simulation first");

    }else{

    emit startSimulation();

    }

}

void OiSimulationWidget::showEvent(QShowEvent *event)
{
    this->ui->comboBox_typeOfUnit->setVisible(false);
    control.setUpSimulations();
    control.resultModel->refreshModel();

    event->accept();
}



void OiSimulationWidget::on_listView_simulations_clicked(const QModelIndex &index)
{
    if(index.isValid()){
        control.setSimulationAt(index.row());

        control.sensorErrorModel->setErrors(control.actualSimulation->getSensorUncertainties());
        control.objectErrorModel->setErrors(control.actualSimulation->getObjectUncertainties());
        control.humanErrorModel->setErrors(control.actualSimulation->getHumanInfluence());
        control.environmentErrorModel->setErrors(control.actualSimulation->getEnviromentUncertainties());


        this->makeSettingsGui();

        ui->tableView_sensor->resizeColumnsToContents();
        ui->tableView_sensor->resizeRowsToContents();

        ui->tableView_environmet->resizeColumnsToContents();
        ui->tableView_environmet->resizeRowsToContents();

        ui->tableView_object->resizeColumnsToContents();
        ui->tableView_object->resizeRowsToContents();

        ui->tableView_humanInfluence->resizeColumnsToContents();
        ui->tableView_humanInfluence->resizeRowsToContents();

        Console::addLine(control.actualSimulation->getMetaData()->name);
    }
}

void OiSimulationWidget::on_treeView_feature_clicked(const QModelIndex &index)
{

    SimulationTreeViewModel *model = dynamic_cast<SimulationTreeViewModel*>(this->ui->treeView_feature->model());
    FeatureTreeItem *item = model->getSelectedItem(index);
    if(item != NULL && index.isValid()){
        if(model != NULL){

            if(item->getParent() != NULL && item->getParent()->getIsFeature()){ //if an attribute of a feature was clicked
                this->ui->comboBox_typeOfUnit->setVisible(false);
                QString attributeToDraw = item->getDisplayValue().toString();
                ui->widgetHistogram->paintData(item->getParent()->getFeature(),attributeToDraw);
                this->setResultList(item->getParent()->getFeature(),"all");

            }else if(item->getIsFeature()){
                this->ui->comboBox_typeOfUnit->setVisible(true);

                if(item->getFeature()->getPoint() != NULL || item->getFeature()->getSphere() != NULL){
                    this->ui->comboBox_typeOfUnit->setEnabled(false);
                }else{
                    this->ui->comboBox_typeOfUnit->setEnabled(true);
                }

                ui->widgetHistogram->paintData(item->getFeature(),"all");
                this->setResultList(item->getFeature(),"all");
            }

        }
    }

}

void OiSimulationWidget::makeSettingsGui()
{

    foreach(QComboBox *c, comboBoxes.values()){
        delete c;
    }
    comboBoxes.clear();

    foreach(QLineEdit *e, doubleLineEdits.values()){
        delete e;
    }
    doubleLineEdits.clear();

    foreach(QLineEdit *e, intLineEdits.values()){
        delete e;
    }
    intLineEdits.clear();

    foreach(QLabel *la, labels){
        delete la;
    }
    labels.clear();

    foreach(QLayout *ly, layouts){
        delete ly;
    }
    layouts.clear();

    if(masterLayout != NULL){
      delete masterLayout;
    }
    masterLayout = new QVBoxLayout();


    control.getDefaultSettings();

    if(control.getIntegerParamter() != NULL){
       QMapIterator<QString,int> j(*control.getIntegerParamter());
       while (j.hasNext()) {
           j.next();

           QLabel* l = new QLabel();
           l->setText(j.key());

           QLineEdit *le = new QLineEdit();
           le->setText(QString::number(j.value()));

           QHBoxLayout *hLayout = new QHBoxLayout();
           hLayout->addWidget(l);
           hLayout->addWidget(le);

           hLayout->setStretch(0,1);
           hLayout->setStretch(1,1);

           labels.append(l);
           intLineEdits.insert(j.key(),le);

           layouts.append(hLayout);

           masterLayout->addLayout(hLayout);
       }
    }

    if(control.getDoubleParamter() != NULL){
       QMapIterator<QString,double> j(*control.getDoubleParamter());
       while (j.hasNext()) {
           j.next();

           QLabel* l = new QLabel();
           l->setText(j.key());

           QLineEdit *le = new QLineEdit();
           le->setText(QString::number(j.value()));

           QHBoxLayout *hLayout = new QHBoxLayout();
           hLayout->addWidget(l);
           hLayout->addWidget(le);

           hLayout->setStretch(0,1);
           hLayout->setStretch(1,1);

           labels.append(l);
           doubleLineEdits.insert(j.key(),le);

           layouts.append(hLayout);

           masterLayout->addLayout(hLayout);
       }
    }

    if(control.getStringParamter() != NULL){
       QMapIterator<QString,QStringList> j(*control.getStringParamter());
       while (j.hasNext()) {
           j.next();

           QLabel* l = new QLabel();
           l->setText(j.key());

           QComboBox *cb = new QComboBox();
           cb->addItems(j.value());

           QHBoxLayout *hLayout = new QHBoxLayout();
           hLayout->addWidget(l);
           hLayout->addWidget(cb);

           hLayout->setStretch(0,1);
           hLayout->setStretch(1,1);

           labels.append(l);
           comboBoxes.insert(j.key(),cb);

           layouts.append(hLayout);

           masterLayout->addLayout(hLayout);
       }
    }


    ui->groupBox_customSettings->setLayout(masterLayout);
}

void OiSimulationWidget::setSettings()
{

    if(intLineEdits.size() > 0){
       QMapIterator<QString,QLineEdit*> j(intLineEdits);
       while (j.hasNext()) {
           j.next();

           control.setSettingItem(j.key(),j.value()->text().toInt());

        }
    }

    if(doubleLineEdits.size() > 0){
       QMapIterator<QString,QLineEdit*> j(doubleLineEdits);
       while (j.hasNext()) {
           j.next();

           control.setSettingItem(j.key(),j.value()->text().toDouble());

        }
    }

    if(comboBoxes.size() > 0){
       QMapIterator<QString,QComboBox*> j(comboBoxes);
       while (j.hasNext()) {
           j.next();

           control.setSettingItem(j.key(),j.value()->currentText());

        }
    }

    control.setSettings();

}

void OiSimulationWidget::setResultList(FeatureWrapper *f,QString attributeToDraw)
{

    QStringList result;

    /*foreach(double d, f->getGeometry()->getSimulationData().uncertaintyX.values){
        result.append(QString::number(d));
    }*/

    QList<double> tmpXList = f->getGeometry()->getSimulationData().uncertaintyX.values;
    QList<double> tmpYList = f->getGeometry()->getSimulationData().uncertaintyY.values;
    QList<double> tmpZList = f->getGeometry()->getSimulationData().uncertaintyZ.values;

    qSort(tmpXList);
    qSort(tmpYList);
    qSort(tmpZList);

    for(int i = 0; i<tmpXList.size();i++){

        double x = tmpXList.at(i);
        double y = tmpYList.at(i);
        double z = tmpZList.at(i);

        QString X = QString::number(x*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
        QString Y = QString::number(y*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);
        QString Z = QString::number(z*UnitConverter::getDistanceMultiplier(),'f',UnitConverter::distanceDigits);

        result.append("xyz "+X+";"+Y+";"+Z);
    }

    resultModel->setStringList(result);

    ui->listView_result->setModel(resultModel);
}

void OiSimulationWidget::on_comboBox_typeOfUnit_currentTextChanged(const QString &arg1)
{
    ui->widgetHistogram->setTypeOfUnit(arg1);
}
