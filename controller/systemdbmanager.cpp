#include "systemdbmanager.h"

QSqlDatabase SystemDbManager::db;
bool SystemDbManager::isInit = false;


/*!
 * \brief SystemDbManager::getCreateFunctionModel
 * Set up model with all available function plugins for the specified feature type
 * \param sqlModel
 * \param ft
 * \return
 */
bool SystemDbManager::getCreateFunctionModel(QSqlQueryModel *sqlModel, Configuration::FeatureTypes ft){
    sqlModel->clear();
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    bool check = false;
    if(SystemDbManager::connect()){

        QString feature = Configuration::getFeatureTypeString(ft);

        QString query;
        if(ft == Configuration::eTrafoParamFeature){ //trafo param is defined by system transformation
            query = QString("SELECT fp.name, fp.description, fp.iid, p.file_path FROM elementPlugin AS ep "
                                    "INNER JOIN functionPlugin AS fp ON fp.id = ep.functionPlugin_id "
                                    "INNER JOIN plugin AS p ON p.id = fp.plugin_id "
                                    "WHERE ep.element_id = %1 "
                                    "AND (fp.iid = \'" + OiMetaData::iid_SystemTransformation + "\');")
                    .arg(SystemDbManager::getElementId(feature));
        }else{ //all other features are defined by fit or construct function
            query = QString("SELECT fp.name, fp.description, fp.iid, p.file_path FROM elementPlugin AS ep "
                                    "INNER JOIN functionPlugin AS fp ON fp.id = ep.functionPlugin_id "
                                    "INNER JOIN plugin AS p ON p.id = fp.plugin_id "
                                    "WHERE ep.element_id = %1 "
                                    "AND (fp.iid = \'" + OiMetaData::iid_FitFunction + "\' OR fp.iid = \'" + OiMetaData::iid_ConstructFunction + "\');")
                    .arg(SystemDbManager::getElementId(feature));
        }


        sqlModel->setQuery(query, SystemDbManager::db);

        if(sqlModel->lastError().isValid()){
            Console::addLine( QString("Database error: %1").arg(sqlModel->lastError().text()) );
        }else{ check = true; }

        SystemDbManager::disconnect();
    }
    return check;
}

/*!
 * \brief SystemDbManager::getChangeFunctionModel
 * Set up model with all available function plugins for the specified feature type
 * \param sqlModel
 * \param ft
 * \return
 */
bool SystemDbManager::getChangeFunctionModel(QSqlQueryModel *sqlModel, Configuration::FeatureTypes ft){
    sqlModel->clear();
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    bool check = false;
    if(SystemDbManager::connect()){

        QString feature = Configuration::getFeatureTypeString(ft);

        QString query = QString("SELECT fp.name, fp.description, fp.iid, p.file_path FROM elementPlugin AS ep "
                                "INNER JOIN functionPlugin AS fp ON fp.id = ep.functionPlugin_id "
                                "INNER JOIN plugin AS p ON p.id = fp.plugin_id "
                                "WHERE ep.element_id = %1 "
                                "AND NOT (fp.iid = \'" + OiMetaData::iid_FitFunction + "\' "
                                "OR fp.iid = \'" + OiMetaData::iid_ConstructFunction + "\' "
                                "OR fp.iid = \'" + OiMetaData::iid_SystemTransformation + "\');")
                .arg(SystemDbManager::getElementId(feature));

        sqlModel->setQuery(query, SystemDbManager::db);

        if(sqlModel->lastError().isValid()){
            Console::addLine( QString("Database error: %1").arg(sqlModel->lastError().text()) );
        }else{ check = true; }

        SystemDbManager::disconnect();
    }
    return check;
}

/*!
 * \brief SystemDbManager::getNeededFeatures
 * Get all needed elements for the function plugin with the specified id
 * \param sqlModel
 * \param id
 * \return
 */
bool SystemDbManager::getNeededElements(QSqlQueryModel *sqlModel, int id){
    sqlModel->clear();
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    bool check = false;
    if(SystemDbManager::connect()){
        QString query = QString("SELECT e.id, e.element_type, pe.element_infinite FROM pluginElement AS pe "
                                "INNER JOIN element AS e ON pe.element_id = e.id "
                                "WHERE pe.functionPlugin_id = %1").arg(id);

        sqlModel->setQuery(query, SystemDbManager::db);

        if(sqlModel->lastError().isValid()){
            Console::addLine( QString("Database error: %1").arg(sqlModel->lastError().text()) );
        }else{ check = true; }

        SystemDbManager::disconnect();
    }
    return check;
}

/*!
 * \brief SystemDbManager::getLaserTrackerModel
 * Fill QSqlQueryModel with all available Lasertracker Plugins
 * \param sqlModel
 * \return
 */
bool SystemDbManager::getLaserTrackerModel(QSqlQueryModel *sqlModel){
    sqlModel->clear();
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    bool check = false;
    if(SystemDbManager::connect()){
        QString query = QString("SELECT sp.name, sp.description, p.name AS plugin, p.file_path FROM sensorPlugin AS sp INNER JOIN plugin AS p "
                                "ON sp.plugin_id = p.id WHERE sp.iid = '%1';")
            .arg(OiMetaData::iid_LaserTracker);

        sqlModel->setQuery(query, SystemDbManager::db);

        if(sqlModel->lastError().isValid()){
            Console::addLine( QString("Database error: %1").arg(sqlModel->lastError().text()) );
        }else{ check = true; }

        SystemDbManager::disconnect();
    }
    return check;
}

/*!
 * \brief SystemDbManager::getTotalStationModel
 * Fill QSqlQueryModel with all available Totalstation Plugins
 * \param sqlModel
 * \return
 */
bool SystemDbManager::getTotalStationModel(QSqlQueryModel *sqlModel){
    sqlModel->clear();
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    bool check = false;
    if(SystemDbManager::connect()){
        QString query = QString("SELECT sp.name, sp.description, p.name AS plugin, p.file_path FROM sensorPlugin AS sp INNER JOIN plugin AS p "
                                "ON sp.plugin_id = p.id WHERE sp.iid = '%1';")
            .arg(OiMetaData::iid_TotalStation);

        sqlModel->setQuery(query, SystemDbManager::db);

        if(sqlModel->lastError().isValid()){
            Console::addLine( QString("Database error: %1").arg(sqlModel->lastError().text()) );
        }else{ check = true; }

        SystemDbManager::disconnect();
    }
    return check;
}

bool SystemDbManager::getUndefinedSensorModel(QSqlQueryModel *sqlModel){
    sqlModel->clear();
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    bool check = false;
    if(SystemDbManager::connect()){
        QString query = QString("SELECT sp.name, sp.description, p.name AS plugin, p.file_path FROM sensorPlugin AS sp INNER JOIN plugin AS p "
                                "ON sp.plugin_id = p.id WHERE sp.iid = '%1';")
            .arg(OiMetaData::iid_Sensor);

        sqlModel->setQuery(query, SystemDbManager::db);

        if(sqlModel->lastError().isValid()){
            Console::addLine( QString("Database error: %1").arg(sqlModel->lastError().text()) );
        }else{ check = true; }

        SystemDbManager::disconnect();
    }
    return check;
}

/*!
 * \brief SystemDbManager::getPluginFilePath
 * Get filepath to the plugin with the name "plugin" which contains the function with the name "name"
 * \param name
 * \param plugin
 * \return
 */
QString SystemDbManager::getPluginFilePath(QString name, QString plugin){
    QString path = QString("");
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){
        QString query = QString("%1 %2 %3")
                .arg("SELECT file_path FROM plugin AS p INNER JOIN functionPlugin AS fp ON p.id = fp.plugin_id")
                .arg(QString("WHERE p.name = '%1'").arg(plugin))
                .arg(QString("AND fp.name = '%1';").arg(name));
        QSqlQuery command(SystemDbManager::db);
        command.exec(query);
        if(command.next()){ //if the desired plugin element is a function
            QVariant val = command.value(0);
            if(val.isValid()){
                path = val.toString();
            }
        }else{
            query = QString("%1 %2 %3")
                    .arg("SELECT file_path FROM plugin AS p INNER JOIN sensorPlugin AS sp ON p.id = sp.plugin_id")
                    .arg(QString("WHERE p.name = '%1'").arg(plugin))
                    .arg(QString("AND sp.name = '%1';").arg(name));
            command.finish();
            command.exec(query);
            if(command.next()){ //if the desired plugin element is a sensor
                QVariant val = command.value(0);
                if(val.isValid()){
                    path = val.toString();
                }
            }else{
                query = QString("%1 %2 %3")
                        .arg("SELECT file_path FROM plugin AS p INNER JOIN networkAdjustmentPlugin AS nap ON p.id = nap.plugin_id")
                        .arg(QString("WHERE p.name = '%1'").arg(plugin))
                        .arg(QString("AND nap.name = '%1';").arg(name));
                command.finish();
                command.exec(query);
                if(command.next()){ //if the desired plugin element is a network adjustment
                    QVariant val = command.value(0);
                    if(val.isValid()){
                        path = val.toString();
                    }
                }else{
                    query = QString("%1 %2 %3")
                            .arg("SELECT file_path FROM plugin AS p INNER JOIN simulationPlugin AS s ON p.id = s.plugin_id")
                            .arg(QString("WHERE p.name = '%1'").arg(plugin))
                            .arg(QString("AND s.name = '%1';").arg(name));
                    command.finish();
                    command.exec(query);
                    if(command.next()){
                        QVariant val = command.value(0);
                        if(val.isValid()){
                            path = val.toString();
                        }
                    }
                }
            }
        }
    }
    return path;
}

/*!
 * \brief SystemDbManager::savePlugin
 * Save the specified plugin in the system database of OpenIndy
 * \param metaInfo
 * \param f
 * \return
 */
int SystemDbManager::savePlugin(PluginMetaData *metaInfo, QList<Function*> functions, QList<Sensor*> sensors, QList<NetworkAdjustment*> networkAdjustments,QList<SimulationModel*> simulationList ){
    int pluginId = -1;
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        bool transaction = SystemDbManager::db.transaction();
        if(transaction){

            //save Plugin
            pluginId = SystemDbManager::savePluginHelper(metaInfo);

            //if the plugin was saved
            if(pluginId > -1){

                foreach(Function* f, functions){
                    SystemDbManager::saveFunctionPlugin(pluginId, f);
                }
                foreach(Sensor* s, sensors){
                    SystemDbManager::saveSensorPlugin(pluginId, s);
                }
                foreach(NetworkAdjustment* n, networkAdjustments){
                    SystemDbManager::saveNetworkAdjustmentPlugin(pluginId, n);
                }
                foreach(SimulationModel *s, simulationList){
                    SystemDbManager::saveSimulationPlugin(pluginId,s);
                }

            }

            if(!SystemDbManager::db.commit()){
                SystemDbManager::db.rollback();
            }

        }else{
            Console::addLine( QString("Database error: %1").arg(SystemDbManager::db.lastError().text()) );
        }

        SystemDbManager::disconnect();
    }
    return pluginId;
}

/*!
 * \brief SystemDbManager::deletePlugin
 * Delete the specified plugin and all dependencies
 * \param id
 * \return
 */
bool SystemDbManager::deletePlugin(int id){
    bool check = false;
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        //start transaction to delete in all tables
        bool transaction = SystemDbManager::db.transaction();
        if(transaction){

            QString query = QString("DELETE FROM elementPlugin WHERE plugin_id = %1").arg(id);
            QSqlQuery command(SystemDbManager::db);
            command.exec(query);

            query = QString("DELETE FROM pluginElement WHERE plugin_id =%1").arg(id);
            command.exec(query);

            query = QString("DELETE FROM plugin WHERE id = %1").arg(id);
            command.exec(query);

            if( !SystemDbManager::db.commit() ){
                Console::addLine( QString("Database error: %1").arg(command.lastError().text()) );
            }else{ check = true; }

        }else{
            Console::addLine( QString("Database error: %1").arg(SystemDbManager::db.lastError().text()) );
        }

        SystemDbManager::disconnect();
    }
    return check;
}

/*!
 * \brief SystemDbManager::init
 * Initialize OpenIndy system database
 */
void SystemDbManager::init(){

    SystemDbManager::db = QSqlDatabase::addDatabase("QSQLITE", "oisystemdb");

#ifdef Q_OS_MAC
    QString dbPath(qApp->applicationDirPath());
#endif

#ifdef Q_OS_WIN
    QString dbPath(qApp->applicationDirPath());
#endif

#ifdef Q_OS_LINUX
    QString dbPath(qApp->applicationDirPath());
#endif

    dbPath.append("/oisystemdb.sqlite");

    SystemDbManager::db.setDatabaseName(dbPath);
    SystemDbManager::isInit = true;
}

/*!
 * \brief SystemDbManager::connect
 * Connect to OpenIndy system database
 * \return
 */
bool SystemDbManager::connect(){
    QFileInfo checkFile(SystemDbManager::db.databaseName());

    if(checkFile.isFile()){
        SystemDbManager::db.open();
    }
    return SystemDbManager::db.isOpen();
}

/*!
 * \brief SystemDbManager::disconnect
 * Disconnect from OpenIndy system database
 */
void SystemDbManager::disconnect(){
    if(SystemDbManager::db.isOpen()){
        SystemDbManager::db.close();
    }
}

/*!
 * \brief SystemDbManager::getElementId
 * Get the id of the specified element.
 * Returns -1 if the element does not exist.
 * \param feature
 * \return
 */
int SystemDbManager::getElementId(QString element){
    QString query = QString("SELECT id FROM element WHERE element_type = '%1'").arg(element);
    QSqlQuery command(SystemDbManager::db);
    command.exec(query);
    if(command.next()){
        QVariant val = command.value(0);
        if(val.isValid()){
            return val.toInt();
        }
    }
    return -1;
}

/*!
 * \brief SystemDbManager::getElementIds
 * Get the id's of the specified elements
 * \param elements
 * \return
 */
QList<int> SystemDbManager::getElementIds(QList<QString> elements){
    QList<int> result;
    if(elements.length() > 0){
        QString query = QString("SELECT id FROM element WHERE element_type = '%1'").arg(elements.at(0));
        for(int i = 1; i < elements.length(); i++){
            query.append( QString(" OR element_type = '%1'").arg(elements.at(i)) );
        }
        QSqlQuery command(SystemDbManager::db);
        command.exec(query);
        while(command.next()){
            QVariant val = command.value(0);
            if(val.isValid()){
                result.append(val.toInt());
            }
        }
    }
    return result;
}

/*!
 * \brief SystemDbManager::getLastId
 * Get Id of the last inserted row of the specified table
 * \param table
 * \return
 */
int SystemDbManager::getLastId(QString table){
    int id = -1;
    if(table.compare("")){
        QString query = QString("SELECT max(id) FROM %1").arg(table);
        QSqlQuery command(SystemDbManager::db);
        command.exec(query);
        if(command.next()){
            QVariant val = command.value(0);
            if(val.isValid()){
                id = val.toInt();
            }
        }
    }
    return id;
}

/*!
 * \brief SystemDbManager::savePluginHelper
 * Save plugin to database
 * \return
 */
int SystemDbManager::savePluginHelper(PluginMetaData *metaInfo){
    int pluginId = -1;

    QString query = QString("INSERT INTO plugin (iid, name, description, version, author, compiler, operating_sys, "
                            "has_dependencies, file_path, is_active) VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', %8, '%9', %10);")
        .arg(metaInfo->iid).arg(metaInfo->name).arg(metaInfo->description).arg(metaInfo->pluginVersion)
        .arg(metaInfo->author).arg(metaInfo->compiler).arg(metaInfo->operatingSystem).arg(QString(metaInfo->dependencies?"1":"0"))
        .arg(metaInfo->path).arg("1");

    QSqlQuery command(SystemDbManager::db);
    bool check = command.exec(query);

    if(check){
        pluginId = SystemDbManager::getLastId("plugin");
    }

    if(command.lastError().isValid()){
        Console::addLine(query);
        Console::addLine( QString("Database error: %1").arg(command.lastError().text()) );
        pluginId = -1;
    }

    return pluginId;
}

/*!
 * \brief SystemDbManager::saveFunctionPlugin
 * \param pluginId
 * \param f
 */
void SystemDbManager::saveFunctionPlugin(int pluginId, Function* f){
    QList<QString> elements;

    //get id's of features for which this plugin is applicable
    QList<Configuration::FeatureTypes> lf = f->applicableFor();
    for(int i=0; i<lf.size(); i++){
        elements.append( Configuration::getFeatureTypeString(lf.at(i)) );
    }
    QList<int> applicableFor = SystemDbManager::getElementIds(elements);

    elements.clear();

    //get id's of elements which this plugin needs to be able to execute
    QList<InputParams> le = f->getNeededElements();
    for(int i=0; i<le.size(); i++){
        elements.append( Configuration::getElementTypeString(le.at(i).typeOfElement) );
    }
    QList<int> neededElements = SystemDbManager::getElementIds(elements);

    if(applicableFor.length() > 0){
        //insert function plugin
        QString query = QString("INSERT INTO functionPlugin (plugin_id, iid, name, description) VALUES (%1, '%2', '%3', '%4')")
                .arg(pluginId).arg(f->getMetaData()->iid).arg(f->getMetaData()->name).arg(f->getMetaData()->description);
        QSqlQuery command(SystemDbManager::db);
        command.exec(query);

        //get id of function plugin
        pluginId = SystemDbManager::getLastId("functionPlugin");

        if(pluginId > -1){
            if(neededElements.length() > 0){
                //insert needed elements
                query = QString("INSERT INTO pluginElement (functionPlugin_id, element_id, element_infinite) VALUES (%1, %2, %3)")
                        .arg(pluginId).arg(neededElements.at(0)).arg(le.at(0).infinite?"1":"0");
                for(int i = 1; i < neededElements.length(); i++){
                    query.append(QString(", (%1, %2, %3)").arg(pluginId).arg(neededElements.at(i)).arg(le.at(i).infinite?"1":"0"));
                }
                command.exec(query);
            }

            //insert applicable for
            query = QString("INSERT INTO elementPlugin (functionPlugin_id, element_id) VALUES (%1, %2)")
                    .arg(pluginId).arg(applicableFor.at(0));
            for(int i = 1; i < applicableFor.length(); i++){
                query.append( QString(", (%1, %2)").arg(pluginId).arg(applicableFor.at(i)) );
            }
            command.exec(query);
        }
    }
}

/*!
 * \brief SystemDbManager::saveSensorPlugin
 * \param pluginId
 * \param s
 */
void SystemDbManager::saveSensorPlugin(int pluginId, Sensor* s){
    //insert sensor plugin
    QString query = QString("INSERT INTO sensorPlugin (plugin_id, iid, name, description) VALUES (%1, '%2', '%3', '%4')")
            .arg(pluginId).arg(s->getMetaData()->iid).arg(s->getMetaData()->name).arg(s->getMetaData()->description);
    QSqlQuery command(SystemDbManager::db);
    command.exec(query);
}

void SystemDbManager::saveSimulationPlugin(int pluginId, SimulationModel *s)
{
    //insert sensor plugin
    QString query = QString("INSERT INTO simulationPlugin (plugin_id, iid, name, description) VALUES (%1, '%2', '%3', '%4')")
            .arg(pluginId).arg(s->getMetaData()->iid).arg(s->getMetaData()->name).arg(s->getMetaData()->description);
    QSqlQuery command(SystemDbManager::db);
    command.exec(query);
}

/*!
 * \brief SystemDbManager::saveNetworkAdjustmentPlugin
 * \param pluginId
 * \param n
 */
void SystemDbManager::saveNetworkAdjustmentPlugin(int pluginId, NetworkAdjustment* n){
    //insert network adjustment plugin
    /*QString query = QString("INSERT INTO sensorPlugin (plugin_id, iid, name, description) VALUES (%1, '%2', '%3', '%4')")
            .arg(pluginId).arg(s->getMetaData()->iid).arg(s->getMetaData()->name).arg(s->getMetaData()->description);
    QSqlQuery command(SystemDbManager::db);
    command.exec(query);*/
}

/*!
 * \brief SystemDbManager::getSupportedGeometries
 * Retrieve a list of all geometries for which a corresponding plugin exists
 * \return
 */
QStringList SystemDbManager::getSupportedGeometries(){
    QStringList result;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QString query = QString("SELECT DISTINCT e.element_type FROM elementPlugin AS ep INNER JOIN element AS e ON ep.element_id = e.id");

        QSqlQuery command(SystemDbManager::db);
        command.exec(query);
        while(command.next()){
            result.append(command.value(0).toString());
        }

        SystemDbManager::disconnect();
    }

    return result;
}

/*!
 * \brief SystemDbManager::getAvailablePlugins
 * Returns a list with all available plugins
 * \return
 */
QList<Plugin> SystemDbManager::getAvailablePlugins(){
    QList<Plugin> result;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);

        //query all available plugins

        QString query = QString("SELECT %1 FROM plugin")
                .arg("id, iid, name, description, version, author, compiler, "
                     "operating_sys, has_dependencies, file_path, is_active");

        command.exec(query);
        while(command.next()){

            Plugin myPlugin;
            myPlugin.id = command.value("id").toInt();
            myPlugin.iid = command.value("iid").toString();
            myPlugin.name = command.value("name").toString();
            myPlugin.description = command.value("description").toString();
            myPlugin.version = command.value("version").toString();
            myPlugin.author = command.value("author").toString();
            myPlugin.compiler = command.value("compiler").toString();
            myPlugin.operating_sys = command.value("operating_sys").toString();
            myPlugin.has_dependencies = command.value("has_dependencies").toBool();
            myPlugin.file_path = command.value("file_path").toString();
            myPlugin.is_active = command.value("is_active").toBool();
            result.append(myPlugin);

        }

        //for each available plugin query its functions and sensors

        for(int i = 0; i < result.size(); i++){
            Plugin &myPlugin = result[i];

            //query functions
            query = QString("SELECT %1 FROM functionPlugin WHERE plugin_id = %2")
                    .arg("id, iid, name, description")
                    .arg(myPlugin.id);

            command.exec(query);
            while(command.next()){

                FunctionPlugin myFunction;
                myFunction.id = command.value("id").toInt();
                myFunction.iid = command.value("iid").toString();
                myFunction.name = command.value("name").toString();
                myFunction.description = command.value("description").toString();
                myFunction.pluginName = myPlugin.name;

                QSqlQuery command2(SystemDbManager::db);

                //query applicableFor features
                QString queryApplicableFor = QString("SELECT e.element_type FROM elementPlugin AS ep INNER JOIN element AS e %1")
                        .arg(QString("ON ep.element_id = e.id WHERE ep.functionPlugin_id = %1")
                             .arg(myFunction.id));
                command2.exec(queryApplicableFor);
                while(command2.next()){
                    myFunction.applicableFor.append(Configuration::getFeatureTypeEnum(command2.value("element_type").toString()));
                }

                //query neededElements elements
                QString queryNeededElements = QString("SELECT e.element_type FROM pluginElement AS pe INNER JOIN element AS e %1")
                        .arg(QString("ON pe.element_id = e.id WHERE pe.functionPlugin_id = %1")
                             .arg(myFunction.id));
                command2.exec(queryNeededElements);
                while(command2.next()){
                    myFunction.neededElements.append(Configuration::getElementTypeEnum(command2.value("element_type").toString()));
                }

                myPlugin.myFunctions.append(myFunction);

            }

            //query sensors
            query = QString("SELECT %1 FROM sensorPlugin WHERE plugin_id = %2")
                    .arg("id, iid, name, description")
                    .arg(myPlugin.id);

            command.exec(query);
            while(command.next()){

                SensorPlugin mySensor;
                mySensor.id = command.value("id").toInt();
                mySensor.iid = command.value("iid").toString();
                mySensor.name = command.value("name").toString();
                mySensor.description = command.value("description").toString();
                mySensor.pluginName = myPlugin.name;
                myPlugin.mySensors.append(mySensor);

            }

            //query simulations
            query = QString("SELECT %1 FROM simulationPlugin WHERE plugin_id = %2")
                    .arg("id, iid, name, description")
                    .arg(myPlugin.id);

            command.exec(query);
            while(command.next()){

                SimulationPlugin mySimulation;
                mySimulation.id = command.value("id").toInt();
                mySimulation.iid = command.value("iid").toString();
                mySimulation.name = command.value("name").toString();
                mySimulation.description = command.value("description").toString();
                mySimulation.pluginName = myPlugin.name;
                myPlugin.mySimulations.append(mySimulation);

            }

        }

        SystemDbManager::disconnect();
    }

    return result;
}

/*!
 * \brief SystemDbManager::getAvailableFitFunctions
 * Returns all fit functions that are available for the specified feature type
 * \param featureType
 * \return
 */
QList<FunctionPlugin> SystemDbManager::getAvailableFitFunctions(Configuration::FeatureTypes featureType){
    QList<FunctionPlugin> result;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);

        QString query = QString("SELECT %1 FROM functionPlugin AS fp %2 %3 %4 WHERE %5")
                .arg("fp.id, fp.iid, fp.name, fp.description, p.name AS pluginName")
                .arg("INNER JOIN elementPlugin AS ep ON fp.id = ep.functionPlugin_id")
                .arg("INNER JOIN plugin AS p ON p.id = fp.plugin_id")
                .arg("INNER JOIN element AS e ON e.id = ep.element_id")
                .arg(QString("e.element_type = \'%1\' AND fp.iid = \'%2\'")
                     .arg(Configuration::getFeatureTypeString(featureType)).arg(FitFunction_iidd));

        command.exec(query);
        while(command.next()){

            FunctionPlugin myPlugin;
            myPlugin.id = command.value("id").toInt();
            myPlugin.iid = command.value("iid").toString();
            myPlugin.name = command.value("name").toString();
            myPlugin.description = command.value("description").toString();
            myPlugin.pluginName = command.value("pluginName").toString();

            QSqlQuery command2(SystemDbManager::db);

            //query applicableFor features
            QString queryApplicableFor = QString("SELECT e.element_type FROM elementPlugin AS ep INNER JOIN element AS e %1")
                    .arg(QString("ON ep.element_id = e.id WHERE ep.functionPlugin_id = %1")
                         .arg(myPlugin.id));
            command2.exec(queryApplicableFor);
            while(command2.next()){
                myPlugin.applicableFor.append(Configuration::getFeatureTypeEnum(command2.value("element_type").toString()));
            }

            //query neededElements elements
            QString queryNeededElements = QString("SELECT e.element_type FROM pluginElement AS pe INNER JOIN element AS e %1")
                    .arg(QString("ON pe.element_id = e.id WHERE pe.functionPlugin_id = %1")
                         .arg(myPlugin.id));
            command2.exec(queryNeededElements);
            while(command2.next()){
                myPlugin.neededElements.append(Configuration::getElementTypeEnum(command2.value("element_type").toString()));
            }

            result.append(myPlugin);

        }

        SystemDbManager::disconnect();
    }

    return result;
}

/*!
 * \brief SystemDbManager::getAvailableConstructFunctions
 * Returns all construct functions that are available for the specified feature type
 * \param featureType
 * \return
 */
QList<FunctionPlugin> SystemDbManager::getAvailableConstructFunctions(Configuration::FeatureTypes featureType){
    QList<FunctionPlugin> result;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);

        QString query = QString("SELECT %1 FROM functionPlugin AS fp %2 %3 %4 WHERE %5")
                .arg("fp.id, fp.iid, fp.name, fp.description, p.name AS pluginName")
                .arg("INNER JOIN elementPlugin AS ep ON fp.id = ep.functionPlugin_id")
                .arg("INNER JOIN plugin AS p ON p.id = fp.plugin_id")
                .arg("INNER JOIN element AS e ON e.id = ep.element_id")
                .arg(QString("e.element_type = \'%1\' AND fp.iid = \'%2\'")
                     .arg(Configuration::getFeatureTypeString(featureType)).arg(ConstructFunction_iidd));

        command.exec(query);
        while(command.next()){

            FunctionPlugin myPlugin;
            myPlugin.id = command.value("id").toInt();
            myPlugin.iid = command.value("iid").toString();
            myPlugin.name = command.value("name").toString();
            myPlugin.description = command.value("description").toString();
            myPlugin.pluginName = command.value("pluginName").toString();

            QSqlQuery command2(SystemDbManager::db);

            //query applicableFor features
            QString queryApplicableFor = QString("SELECT e.element_type FROM elementPlugin AS ep INNER JOIN element AS e %1")
                    .arg(QString("ON ep.element_id = e.id WHERE ep.functionPlugin_id = %1")
                         .arg(myPlugin.id));
            command2.exec(queryApplicableFor);
            while(command2.next()){
                myPlugin.applicableFor.append(Configuration::getFeatureTypeEnum(command2.value("element_type").toString()));
            }

            //query neededElements elements
            QString queryNeededElements = QString("SELECT e.element_type FROM pluginElement AS pe INNER JOIN element AS e %1")
                    .arg(QString("ON pe.element_id = e.id WHERE pe.functionPlugin_id = %1")
                         .arg(myPlugin.id));
            command2.exec(queryNeededElements);
            while(command2.next()){
                myPlugin.neededElements.append(Configuration::getElementTypeEnum(command2.value("element_type").toString()));
            }

            result.append(myPlugin);

        }

        SystemDbManager::disconnect();
    }

    return result;
}

/*!
 * \brief SystemDbManager::getDefaultFunction
 * Returns the default function for the specified feature type
 * \param featureType
 * \return
 */
FunctionPlugin SystemDbManager::getDefaultFunction(Configuration::FeatureTypes featureType){
    FunctionPlugin result;

    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        QSqlQuery command(SystemDbManager::db);

        QString query = QString("SELECT %1 FROM functionPlugin AS fp %2 %3 %4 WHERE %5")
                .arg("fp.id, fp.iid, fp.name, fp.description, p.name AS pluginName")
                .arg("INNER JOIN elementPlugin AS ep ON fp.id = ep.functionPlugin_id")
                .arg("INNER JOIN plugin AS p ON p.id = fp.plugin_id")
                .arg("INNER JOIN element AS e ON e.id = ep.element_id")
                .arg(QString("e.element_type = \'%1\' AND ep.use_as_default = 1").arg(Configuration::getFeatureTypeString(featureType)));

        command.exec(query);
        while(command.next()){

            FunctionPlugin myPlugin;
            myPlugin.id = command.value("id").toInt();
            myPlugin.iid = command.value("iid").toString();
            myPlugin.name = command.value("name").toString();
            myPlugin.description = command.value("description").toString();
            myPlugin.pluginName = command.value("pluginName").toString();

            QSqlQuery command2(SystemDbManager::db);

            //query applicableFor features
            QString queryApplicableFor = QString("SELECT e.element_type FROM elementPlugin AS ep INNER JOIN element AS e %1")
                    .arg(QString("ON ep.element_id = e.id WHERE ep.functionPlugin_id = %1")
                         .arg(myPlugin.id));
            command2.exec(queryApplicableFor);
            while(command2.next()){
                myPlugin.applicableFor.append(Configuration::getFeatureTypeEnum(command2.value("element_type").toString()));
            }

            //query neededElements elements
            QString queryNeededElements = QString("SELECT e.element_type FROM pluginElement AS pe INNER JOIN element AS e %1")
                    .arg(QString("ON pe.element_id = e.id WHERE pe.functionPlugin_id = %1")
                         .arg(myPlugin.id));
            command2.exec(queryNeededElements);
            while(command2.next()){
                myPlugin.neededElements.append(Configuration::getElementTypeEnum(command2.value("element_type").toString()));
            }

            result = myPlugin;
            break;

        }

        SystemDbManager::disconnect();
    }

    return result;
}

/*!
 * \brief SystemDbManager::saveDefaultFunction
 * Save function as default
 * \param featureType
 * \param function
 * \param plugin
 */
void SystemDbManager::saveDefaultFunction(Configuration::FeatureTypes featureType, QString function, QString plugin){
    if(!SystemDbManager::isInit){ SystemDbManager::init(); }
    if(SystemDbManager::connect()){

        bool transaction = SystemDbManager::db.transaction();
        if(transaction){

            QSqlQuery command(SystemDbManager::db);

            //set default state of all functions for featureType to false
            QString query = QString("UPDATE elementPlugin SET use_as_default = 0 WHERE id IN %1")
                    .arg(QString("(SELECT ep.id FROM elementPlugin AS ep %1 %2")
                         .arg("INNER JOIN element AS e ON ep.element_id = e.id")
                         .arg(QString("WHERE e.element_type = \'%1\')").arg(Configuration::getFeatureTypeString(featureType))));

            if(command.exec(query)){

                //set default state of the given function to true
                query = QString("UPDATE elementPlugin SET use_as_default = 1 WHERE id IN %1")
                        .arg(QString("(SELECT ep.id FROM elementPlugin AS ep %1 %2 %3 %4")
                             .arg("INNER JOIN element AS e ON ep.element_id = e.id")
                             .arg("INNER JOIN functionPlugin AS fp ON ep.functionPlugin_id = fp.id")
                             .arg("INNER JOIN plugin AS p ON fp.plugin_id = p.id")
                             .arg(QString("WHERE e.element_type = \'%1\' AND fp.name = \'%2\' AND p.name = \'%3\')")
                                  .arg(Configuration::getFeatureTypeString(featureType))
                                  .arg(function)
                                  .arg(plugin)));
                command.exec(query);

            }

            if(!SystemDbManager::db.commit()){
                SystemDbManager::db.rollback();
            }

        }else{
            Console::addLine( QString("Database error: %1").arg(SystemDbManager::db.lastError().text()) );
        }

        SystemDbManager::disconnect();
    }
}
