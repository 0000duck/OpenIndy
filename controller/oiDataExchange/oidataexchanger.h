#ifndef OIDATAEXCHANGER_H
#define OIDATAEXCHANGER_H

#include <QString>
#include <QStringList>
#include <QObject>

#include "unitconverter.h"
#include "pi_oiexchangesimpleascii.h"
#include "pi_oiexchangedefinedformat.h"

class OiDataExchanger : public QObject
{
    Q_OBJECT

private:
    explicit OiDataExchanger(QObject *parent = 0);
    ~OiDataExchanger();

public:
    static bool importData(OiExchangeSimpleAscii *simpleAsciiExchange, OiExchangeObject &projectData);
    static bool importData(OiExchangeDefinedFormat *definedFormatExchange, OiExchangeObject &projectData);
    static bool exportData(OiExchangeSimpleAscii *simpleAsciiExchange, OiExchangeObject &projectData);
    static bool exportData(OiExchangeDefinedFormat *definedFormatExchange, OiExchangeObject &projectData);

private:
    QThread myExchangeThread;


    /*
public:
    static OiDataExchanger &getInstance();

    static QStringList getOutputFormats();
    static QList<Configuration::ElementTypes> getSupportedElements(QString format);
    static QStringList getElementDescription(QString format, Configuration::ElementTypes);
    static QList<UnitConverter::unitType> getDistanceUnits(QString format);
    static QList<UnitConverter::unitType> getAngleUnits(QString format);
    static QList<UnitConverter::unitType> getTemperatureUnits(QString format);

public slots:
    void importData(OiExchangeObject exchangeData, QString format);
    void exportData(OiExchangeObject exchangeData, QString format);

signals:
    void updateProgress(int progress, QString msg); //progress in percent (0-100)
    void exchangeFinished(bool success, OiExchangeObject exchangeData);

private slots:
    void exchangeData();
    void emitUpdateProgress(int progress, QString msg);

private:
    static OiDataExchanger myInstance;
    static QString format;
    static bool import;

    QThread myExchangeThread;

    static OiExchangeObject myExchangeData;
*/
};

#endif // OIDATAEXCHANGER_H
