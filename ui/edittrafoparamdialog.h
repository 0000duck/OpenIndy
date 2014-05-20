#ifndef EDITTRAFOPARAMDIALOG_H
#define EDITTRAFOPARAMDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QShowEvent>
#include <QDesktopWidget>
#include "featurewrapper.h"
#include "unitconverter.h"
#include "functionstatistic.h"
#include "matrixmodel.h"

namespace Ui {
class EditTrafoparamDialog;
}
/*!
 * \brief The EditTrafoparamDialog class for editing transformation parameters by hand or creating own parameters by hand without running any functions.
 */
class EditTrafoparamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTrafoparamDialog(QWidget *parent = 0);
    ~EditTrafoparamDialog();

    FunctionStatistic *fModel;
    MatrixModel *mModel;

signals:

public slots:

    void getSelectedTrafoParam(FeatureWrapper *fw);

    void showEvent(QShowEvent *event);

private slots:
    void on_toolButton_cancel_clicked();

    void on_toolButton_ok_clicked();

    void initGUI();

    void getValues();

    void on_checkBox_identicalScale_toggled(bool checked);

    void on_comboBox_displayedFunction_currentIndexChanged(const QString &arg1);

private:
    Ui::EditTrafoparamDialog *ui;
    FeatureWrapper *selectedTrafoParam;
};

#endif // EDITTRAFOPARAMDIALOG_H
