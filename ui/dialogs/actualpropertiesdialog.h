#ifndef ACTUALPROPERTIESDIALOG_H
#define ACTUALPROPERTIESDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QMap>
#include <QLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QNetworkInterface>
#include <QShowEvent>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QClipboard>
#include <QPointer>

#include "modelmanager.h"

namespace Ui {
class ActualPropertiesDialog;
}

/*!
 * \brief The ActualPropertiesDialog class
 */
class ActualPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ActualPropertiesDialog(QWidget *parent = 0);
    ~ActualPropertiesDialog();

private:

    //##################################
    //methods to initialize GUI elements
    //##################################

    void showEvent(QShowEvent *event);

    void initGUI();
    void initModels();

    /*

    ReadingModel *rModel;
    ObservationModel *oModel;
    FunctionStatistic *fModel;
    MatrixModel *qxxModel;
    MatrixModel *sxxModel;

    ObservationProxyModel *observationOverviewModel;
    ReadingProxyModel *readingOverviewModel;

    //void getActiveFeature(FeatureWrapper *activeFeature);

    void showEvent(QShowEvent *event);

public slots:

    void resizeView();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void initGUI();

    void on_comboBox_displayedFunction_currentIndexChanged(const QString &arg1);

    void closeEvent(QCloseEvent *event);

    void displayUsedReadings();

    void on_comboBox_readings_currentTextChanged(const QString &arg1);

    void updateModels();

    void keyPressEvent(QKeyEvent *event);

    void copyValuesFromView();*/

private:
    Ui::ActualPropertiesDialog *ui;
};

#endif // ACTUALPROPERTIESDIALOG_H
