#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlQueryModel>
#include <QString>
#include <QList>
#include <QDir>
#include "pluginmetadata.h"
#include "oimetadata.h"
#include "console.h"
#include "configuration.h"

class SystemDbManager
{
public:
    static int savePlugin(PluginMetaData *metaInfo, QList<Function*> functions, QList<Sensor*> sensors, QList<NetworkAdjustment*> networkAdjustments);
    static bool deletePlugin(int id);

    static bool getCreateFunctionModel(QSqlQueryModel *sqlModel, Configuration::FeatureTypes ft);
    static bool getChangeFunctionModel(QSqlQueryModel *sqlModel, Configuration::FeatureTypes ft);
    static bool getNeededElements(QSqlQueryModel *sqlModel, int id);

    static bool getLaserTrackerModel(QSqlQueryModel *sqlModel);
    static bool getTotalStationModel(QSqlQueryModel *sqlModel);
    static bool getUndefinedSensorModel(QSqlQueryModel *sqlModel);

    static QString getPluginFilePath(QString name, QString plugin);

private:
    static QSqlDatabase db;
    static bool isInit;

    static void init();
    static bool connect();
    static void disconnect();

    static int getElementId(QString element);
    static QList<int> getElementIds(QList<QString> elements);
    static int getLastId(QString table);
    static int savePluginHelper(PluginMetaData *metaInfo);
    static void saveFunctionPlugin(int pluginId, Function* f);
    static void saveSensorPlugin(int pluginId, Sensor* s);
    static void saveNetworkAdjustmentPlugin(int pluginId, NetworkAdjustment* n);

};

#endif // DATABASE_H
