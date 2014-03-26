#-------------------------------------------------
#
# Project created by QtCreator 2013-09-18T14:25:09
#
#-------------------------------------------------

QT       += core gui sql serialport network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openIndy
TEMPLATE = app

DEFINES += OI_MAIN_PROGRAM

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
} else {
    DESTDIR = $$PWD/bin/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

RESOURCES += \
    res/res.qrc

win32:RC_FILE = $$PWD/res/openIndy.rc
unix:ICON = $$PWD/res/openIndy.icns

#-----------------------------------linux dependency---------------------------------------
linux: LIBS = -lglut -lGLU
#-----------------------------------Linear Algebra---------------------------------------

INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/ui
INCLUDEPATH += $$PWD/controller
INCLUDEPATH += $$PWD/src/geometry
INCLUDEPATH += $$PWD/src/plugin
INCLUDEPATH += $$PWD/controller/oiDataExchange
INCLUDEPATH += $$PWD/lib/openIndyLib/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/release/ -lopenIndyLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/debug/ -lopenIndyLib
else:unix: CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/debug -lopenIndyLib
else:unix: CONFIG(release, debug|release): LIBS += -L$$PWD/lib/openIndyLib/bin/release -lopenIndyLib

INCLUDEPATH += $$PWD/lib/openIndyLib/bin/debug
DEPENDPATH += $$PWD/lib/openIndyLib/bin/debug

INCLUDEPATH += $$PWD/lib/openIndyLib/bin/release
DEPENDPATH += $$PWD/lib/openIndyLib/bin/release

SOURCES += \
    ui/watchwindow.cpp \
    ui/tablemodel.cpp \
    ui/sensorplugindialog.cpp \
    ui/pluginloaderdialog.cpp \
    ui/measurementconfigdialog.cpp \
    ui/mainwindow.cpp \
    ui/main.cpp \
    ui/createfeature.cpp \
    controller/systemdbmanager.cpp \
    controller/pluginloader.cpp \
    controller/controller.cpp \
    src/trafoparam.cpp \
    src/statistic.cpp \
    src/sensorcontrol.cpp \
    src/reading.cpp \
    src/pluginmetadata.cpp \
    src/oimetadata.cpp \
    src/oiemitter.cpp \
    src/observation.cpp \
    src/measurementconfig.cpp \
    src/geometry.cpp \
    src/feature.cpp \
    src/coordinatesystem.cpp \
    src/console.cpp \
    src/configuration.cpp \
    src/geometry/sphere.cpp \
    src/geometry/pointcloud.cpp \
    src/geometry/point.cpp \
    src/geometry/plane.cpp \
    src/geometry/paraboloid.cpp \
    src/geometry/nurbs.cpp \
    src/geometry/line.cpp \
    src/geometry/hyperboloid.cpp \
    src/geometry/ellipsoid.cpp \
    src/geometry/cylinder.cpp \
    src/geometry/cone.cpp \
    src/geometry/circle.cpp \
    ui/functionpluginloader.cpp \
    src/connectionconfig.cpp \
    ui/sensorinfodialog.cpp \
    src/featurewrapper.cpp \
    src/station.cpp \
    ui/glwidget.cpp \
    src/sensorconfiguration.cpp \
    ui/featuredata.cpp \
    ui/observationmodel.cpp \
    ui/readingmodel.cpp \
    ui/oiGraphixFactory/oigraphix_sphere.cpp \
    ui/oiGraphixFactory/oigraphix.cpp \
    src/geometry/scalarentityangle.cpp \
    src/geometry/scalarentitydistance.cpp \
    ui/movementdialog.cpp \
    src/unitconverter.cpp \
    ui/settingsdialog.cpp \
    ui/oiGraphixFactory/oigraphix_plane.cpp \
    ui/oiGraphixFactory/oigraphix_point.cpp \
    ui/oiGraphixFactory/oigraphix_line.cpp \
    ui/oiGraphixFactory/oigraphix_station.cpp \
    ui/scalarentitydialog.cpp \
    ui/importnominalgeometrydialog.cpp \
    ui/featuretreeviewmodel.cpp \
    ui/featuretreeitem.cpp \
    ui/featureovserviewproxymodel.cpp \
    ui/availableelementstreeviewproxymodel.cpp \
    ui/trafoparamproxymodel.cpp \
    ui/nominaldatadialog.cpp \
    ui/featuregraphicstreeviewproxymodel.cpp \
    ui/functionstatistic.cpp \
    controller/featureupdater.cpp \
    controller/sortlistbyname.cpp \
    src/residual.cpp \
    ui/usedelementsmodel.cpp \
    ui/edittrafoparamdialog.cpp \
    ui/matrixmodel.cpp \
    src/function.cpp \
    controller/oiDataExchange/oiexchangeobject.cpp \
    controller/oiDataExchange/oidataexchanger.cpp \
    controller/oiDataExchange/oiexchangeascii.cpp \
    controller/oiDataExchange/oiprojectdata.cpp \
    controller/oiDataExchange/oiprojectexchanger.cpp \
    src/functionconfiguration.cpp \
    src/geometry/scalarentitytemperature.cpp \
    src/geometry/scalarentitymeasurementseries.cpp \
    ui/customparameterwidget.cpp \
    src/elementdependencies.cpp \
    controller/oiDataExchange/projectrestorer.cpp

HEADERS  += \
    ui/watchwindow.h \
    ui/tablemodel.h \
    ui/sensorplugindialog.h \
    ui/pluginloaderdialog.h \
    ui/measurementconfigdialog.h \
    ui/mainwindow.h \
    ui/createfeature.h \
    controller/systemdbmanager.h \
    controller/pluginloader.h \
    controller/controller.h \
    src/trafoparam.h \
    src/statistic.h \
    src/sensorcontrol.h \
    src/sensor.h \
    src/reading.h \
    src/pluginmetadata.h \
    src/oimetadata.h \
    src/oiemitter.h \
    src/observation.h \
    src/measurementconfig.h \
    src/geometry.h \
    src/function.h \
    src/feature.h \
    src/element.h \
    src/coordinatesystem.h \
    src/console.h \
    src/configuration.h \
    src/plugin/pi_totalstation.h \
    src/plugin/pi_systemtransformation.h \
    src/plugin/pi_objecttransformation.h \
    src/plugin/pi_networkadjustment.h \
    src/plugin/pi_lasertracker.h \
    src/plugin/pi_geodeticfunction.h \
    src/plugin/pi_fitfunction.h \
    src/plugin/pi_constructfunction.h \
    src/geometry/sphere.h \
    src/geometry/pointcloud.h \
    src/geometry/point.h \
    src/geometry/plane.h \
    src/geometry/paraboloid.h \
    src/geometry/nurbs.h \
    src/geometry/line.h \
    src/geometry/hyperboloid.h \
    src/geometry/ellipsoid.h \
    src/geometry/cylinder.h \
    src/geometry/cone.h \
    src/geometry/circle.h \
    ui/functionpluginloader.h \
    src/connectionconfig.h \
    ui/sensorinfodialog.h \
    src/featurewrapper.h \
    src/station.h \
    ui/glwidget.h \
    src/sensorconfiguration.h \
    ui/featuredata.h \
    ui/observationmodel.h \
    ui/readingmodel.h \
    src/plugin/pi_oiplugin.h \
    lib/openIndyLib/include/oivec.h \
    lib/openIndyLib/include/oimat.h \
    lib/openIndyLib/include/linearalgebra.h \
    lib/openIndyLib/include/global.h \
    lib/openIndyLib/include/chooselalib.h \
    ui/oiGraphixFactory/oigraphix_sphere.h \
    ui/oiGraphixFactory/oigraphix_geometry.h \
    ui/oiGraphixFactory/oigraphix.h \
    src/geometry/scalarentityangle.h \
    src/geometry/scalarentitydistance.h  \
    ui/movementdialog.h \
    src/unitconverter.h \
    ui/settingsdialog.h  \
    ui/oiGraphixFactory/oigraphix_plane.h \
    ui/oiGraphixFactory/oigraphix_point.h \
    ui/oiGraphixFactory/oigraphix_line.h \
    ui/oiGraphixFactory/oigraphix_station.h \
    ui/scalarentitydialog.h \
    ui/importnominalgeometrydialog.h \
    ui/featuretreeviewmodel.h \
    ui/featuretreeitem.h \
    ui/featureovserviewproxymodel.h \
    ui/availableelementstreeviewproxymodel.h \
    ui/trafoparamproxymodel.h \
    ui/nominaldatadialog.h \
    ui/featuregraphicstreeviewproxymodel.h \
    ui/functionstatistic.h \
    controller/featureupdater.h \
    controller/sortlistbyname.h \
    src/residual.h \
    ui/usedelementsmodel.h \
    ui/edittrafoparamdialog.h \
    ui/matrixmodel.h \
    controller/oiDataExchange/oiexchangeobject.h \
    controller/oiDataExchange/oidataexchanger.h \
    controller/oiDataExchange/oiexchangeinterface.h \
    controller/oiDataExchange/oiexchangeascii.h \
    controller/oiDataExchange/oiprojectdata.h \
    controller/oiDataExchange/oiprojectexchanger.h \
    src/functionconfiguration.h \
    src/geometry/scalarentitytemperature.h \
    src/geometry/scalarentitymeasurementseries.h \
    ui/customparameterwidget.h \
    src/elementdependencies.h \
    controller/oiDataExchange/projectrestorer.h


FORMS    += ui/mainwindow.ui \
    ui/measurementconfigdialog.ui \
    ui/watchwindow.ui \
    ui/pluginloaderdialog.ui \
    ui/createfeature.ui \
    ui/sensorplugindialog.ui \
    ui/functionpluginloader.ui \
    ui/sensorinfodialog.ui \
    ui/featuredata.ui \
    ui/movementdialog.ui \
    ui/settingsdialog.ui \
    ui/scalarentitydialog.ui \
    ui/importnominalgeometrydialog.ui \
    ui/nominaldatadialog.ui \
    ui/edittrafoparamdialog.ui


