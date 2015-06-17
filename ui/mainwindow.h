#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QString>
#include <QIODevice>
#include <QListView>
#include <QSignalMapper>

#include "controller.h"
#include "featureattributes.h"
#include "modelmanager.h"
#include "console.h"
#include "exchangeParams.h"
#include "toolaction.h"

#include "createfeaturedialog.h"
#include "pluginloaderdialog.h"
#include "importnominaldialog.h"
#include "featurefunctionsdialog.h"
#include "loadingdialog.h"
#include "sensorconfigurationdialog.h"
#include "movesensordialog.h"
#include "sensortaskinfodialog.h"
#include "pluginmanagerdialog.h"
#include "watchwindowdialog.h"
#include "measurementconfigurationdialog.h"
#include "settingsdialog.h"
#include "actualpropertiesdialog.h"
#include "nominalpropertiesdialog.h"

#include "featuretabledelegate.h"
#include "trafoparamtabledelegate.h"

using namespace oi;

namespace Ui {
class MainWindow;
}

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    //###################################
    //tasks to be performed by controller
    //###################################

    //add or remove features
    void addFeatures(const FeatureAttributes &attributes);
    void removeFeature(const int &featureId);

    //remove observations
    void removeObservations(const int &featureId);
    void removeAllObservations();

    //set sensor configuration for active sensor
    void sensorConfigurationChanged(const QString &name, const bool &connectSensor);

    //set measurement configuration for active feature
    void measurementConfigurationChanged(const QString &name);

    //recalculation of features
    void recalcAll();
    void recalcActiveFeature();
    void recalcFeature(const int &featureId);
    void recalcTrafoParam(const int &featureId);

    //import or export features
    void importNominals(const ExchangeParams &params);

    //save or load projects
    void saveProject();
    void saveProject(const QString &fileName);
    void loadProject(const QString &projectName, const QPointer<QIODevice> &device);

private slots:

    //#############################
    //tasks triggered by controller
    //#############################

    //nominal import
    void importNominalsStarted();
    void importNominalsFinished(const bool &success);

    //observation import
    void importObservationsStarted();
    void importObservationsFinished(const bool &success);

    //active feature changes
    void activeFeatureChanged();
    void activeCoordinateSystemChanged();

    //feature(s) added or removed
    void coordSystemSetChanged();
    void stationSetChanged();

    //group(s) added or removed
    void availableGroupsChanged();
    void activeGroupChanged();

    //station changes
    void stationSensorChanged(const int &featureId);

    //sensor actions
    void sensorActionStarted(const QString &name);
    void sensorActionFinished(const bool &success, const QString &msg);

    //#########################
    //actions triggered by user
    //#########################

    //key pressed
    void keyPressEvent(QKeyEvent * e);

    //create feature actions
    void on_actionCreate_point_triggered();
    void on_actionCreate_line_triggered();
    void on_actionCreate_plane_triggered();
    void on_actionCreate_sphere_triggered();
    void on_actionCreate_station_triggered();
    void on_actionCreate_coordinatesystem_triggered();
    void on_actionCreate_scalar_entity_triggered();
    void on_actionCreate_trafoParam_triggered();
    void on_actionCreate_cone_triggered();
    void on_actionCreate_cylinder_triggered();
    void on_actionCreate_ellipsoid_triggered();
    void on_actionCreate_hyperboloid_triggered();
    void on_actionCreate_paraboloid_triggered();
    void on_actionCreate_nurbs_triggered();
    void on_actionCreate_pointcloud_triggered();
    void on_actionCreate_circle_triggered();
    void on_actionCreate_torus_triggered();
    void on_actionCreate_slotted_hole_triggered();
    void on_actionCreate_ellipse_triggered();

    //plugin actions
    void on_actionLoad_plugins_triggered();
    void on_actionPlugin_manager_triggered();

    //import export actions
    void on_action_importNominals_triggered();

    //feature table view interactions
    void on_tableView_features_clicked(const QModelIndex &index);
    void tableViewFeaturesSelectionChangedByKeyboard(const QModelIndex &selected, const QModelIndex &deselected);
    void on_tableView_features_customContextMenuRequested(const QPoint &pos);
    void deleteFeatures(bool checked);
    void showFeatureProperties(bool checked);
    void on_tableView_trafoParams_clicked(const QModelIndex &index);
    void tableViewTrafoParamsSelectionChangedByKeyboard(const QModelIndex &selected, const QModelIndex &deselected);
    void on_tableView_trafoParams_customContextMenuRequested(const QPoint &pos);

    //function dialog
    void on_actionSet_function_triggered();

    //sensor dialogs
    void on_actionSet_sensor_triggered();
    void setSensorConfiguration(const QString &name);
    void showMoveSensorDialog();

    //toggle visibility of widgets
    void on_actionControl_pad_triggered();

    //active group or coordinate system changed
    void on_comboBox_groups_currentIndexChanged(const QString &arg1);
    void on_comboBox_activeCoordSystem_currentIndexChanged(const QString &arg1);

    //show watch window
    void on_actionWatch_window_triggered();

    //save or load projects
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();

    //close OpenIndy
    void on_actionClose_triggered();

    //show measurement config dialog
    void on_actionMeasurement_Configuration_triggered();

    //activate a station
    void on_actionActivate_station_triggered();

    //show settings dialog
    void on_actionView_settings_triggered();

    //show tool widget
    void showToolWidget(const QString &pluginName, const QString &toolName);

    //resize table views
    void resizeTableView();

    //remove observations
    void on_actionRemoveObservations_triggered();
    void removeObservationOfActiveFeature();

private:
    Ui::MainWindow *ui;

private:

    Controller control;

    //#################################
    //set up models and connect dialogs
    //#################################

    void connectController();
    void connectDialogs();
    void assignModels();

    //##################################
    //methods to initialize GUI elements
    //##################################

    void initFeatureTableViews();

    void initSensorPad();

    void initToolMenus();

    //##############################
    //methods to update GUI elements
    //##############################

    void activeSensorTypeChanged(const SensorTypes &type, const QList<SensorFunctions> &supportedActions, const QStringList &selfDefinedActions);

    void updateMagnifyWindow(const QPointer<FeatureWrapper> &feature);

    //############################
    //OpenIndy dialogs and widgets
    //############################

    CreateFeatureDialog createFeatureDialog;
    PluginLoaderDialog pluginLoaderDialog;
    ImportNominalDialog importNominalDialog;
    LoadingDialog loadingDialog;
    FeatureFunctionsDialog featureFunctionsDialog;
    SensorConfigurationDialog sensorConfigurationDialog;
    MoveSensorDialog moveSensorDialog;
    SensorTaskInfoDialog sensorTaskInfoDialog;
    PluginManagerDialog pluginManagerDialog;
    WatchWindowDialog watchWindowDialog;
    MeasurementConfigurationDialog measurementConfigDialog;
    SettingsDialog settingsDialog;
    ActualPropertiesDialog actualPropertiesDialog;
    NominalPropertiesDialog nominalPropertiesDialog;

    //##########
    //sensor pad
    //##########

    //sensor actions
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionInitialize;
    QAction *actionMeasure;
    QAction *actionAim;
    QAction *actionMove;
    QAction *actionHome;
    QAction *actionChangeMotorState;
    QAction *actionToggleSightOrientation;
    QAction *actionCompensation;
    QList<QPointer<QAction> > selfDefinedActions;

    QPointer<QSignalMapper> customActionMapper;

};

#endif // MAINWINDOW_H
