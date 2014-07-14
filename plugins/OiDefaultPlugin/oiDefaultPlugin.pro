#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T15:43:14
#
#-------------------------------------------------

QT       += gui
QT += serialport

TEMPLATE = lib
TARGET = $$qtLibraryTarget(p_defaultPlugin)

CONFIG += plugin
CONFIG +=c++11

DEFINES += DEFAULT_LIBRARY
DEFINES += WITH_FITTING_UTILS

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/debug
} else {
    DESTDIR = $$PWD/bin/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/openIndyLib/bin/release/ -lopenIndyLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/openIndyLib/bin/debug/ -lopenIndyLib
else:unix: LIBS += -L$$PWD/../../lib/openIndyLib/bin/debug -lopenIndyLib

INCLUDEPATH += -L$$PWD/../../lib/openIndyLib/bin/debug
DEPENDPATH += -L$$PWD/../../lib/openIndyLib/bin/debug

INCLUDEPATH += -L$$PWD/../../lib/openIndyLib/bin/release
DEPENDPATH += -L$$PWD/../../lib/openIndyLib/bin/release

INCLUDEPATH += $$PWD
INCLUDEPATH += ../../src
INCLUDEPATH += ../../src/geometry
INCLUDEPATH += ../../src/plugin
INCLUDEPATH += ../../src/simulation
INCLUDEPATH += ../../lib/openIndyLib/include

INCLUDEPATH += $$PWD/sensors/laserTracker/pseudoTracker
INCLUDEPATH += $$PWD/sensors/tachymeter/LeicaGeoCom
INCLUDEPATH += $$PWD/sensors/undefinedSensor/pseudoSensor
INCLUDEPATH += $$PWD/functions/construct
INCLUDEPATH += $$PWD/functions/fit
INCLUDEPATH += $$PWD/functions/objectTransformation
INCLUDEPATH += $$PWD/functions/systemTransformation
INCLUDEPATH += $$PWD/simulations/simplePolarMeasurement

INCLUDEPATH += $$PWD/lib/fitting
INCLUDEPATH += $$PWD/lib/fitting/util

SOURCES += p_factory.cpp \
    functions/construct/p_intersectlineplane.cpp \
    functions/construct/p_intersectplaneplane.cpp \
    functions/construct/p_linefrompoints.cpp \
    functions/construct/p_planefrompoints.cpp \
    functions/construct/p_pointbetweentwopoints.cpp \
    functions/construct/p_pointfrompoints.cpp \
    functions/construct/p_spherefrompoints.cpp \
    functions/fit/p_bestfitline.cpp \
    functions/fit/p_bestfitplane.cpp \
    functions/fit/p_bestfitplane2.cpp \
    functions/fit/p_bestfitpoint.cpp \
    functions/fit/p_bestfitsphere.cpp \
    functions/objectTransformation/p_changeradius.cpp \
    functions/objectTransformation/p_projectInPlane.cpp \
    functions/objectTransformation/p_translateByLine.cpp \
    functions/objectTransformation/p_translateByPlane.cpp \
    functions/systemTransformation/p_helmert7Param.cpp \
    sensors/laserTracker/pseudoTracker/p_pseudotracker.cpp \
    sensors/tachymeter/LeicaGeoCom/p_leicatachymeter.cpp \
    sensors/undefinedSensor/pseudoSensor/p_pseudosensor.cpp \
    lib/fitting/cfitting_approxfunc.cpp \
    lib/fitting/cfitting_circle.cpp \
    lib/fitting/cfitting_ellipse.cpp \
    lib/fitting/cfitting_it.cpp \
    lib/fitting/cfitting_matrix.cpp \
    lib/fitting/cfitting_plane.cpp \
    lib/fitting/cfitting_sphere.cpp \
    lib/fitting/cfitting_util.cpp \
    lib/fitting/util/Getopt.cpp \
    lib/fitting/util/iskeyword.cpp \
    lib/fitting/util/microtim.cpp \
    lib/fitting/util/mystring.cpp \
    lib/fitting/util/rho.cpp \
    functions/objectTransformation/p_shiftplane.cpp \
    functions/fit/p_bestfitcircle.cpp \
    functions/systemTransformation/p_simpletemperaturecompensation.cpp \
    ../../src/unitconverter.cpp \
    ../../src/trafoparam.cpp \
    ../../src/statistic.cpp \
    ../../src/station.cpp \
    ../../src/sensorlistener.cpp \
    ../../src/sensorcontrol.cpp \
    ../../src/sensorconfiguration.cpp \
    ../../src/residual.cpp \
    ../../src/reading.cpp \
    ../../src/pluginmetadata.cpp \
    ../../src/oimetadata.cpp \
    ../../src/oiemitter.cpp \
    ../../src/observation.cpp \
    ../../src/measurementconfig.cpp \
    ../../src/geometry.cpp \
    ../../src/functionconfiguration.cpp \
    ../../src/function.cpp \
    ../../src/featurewrapper.cpp \
    ../../src/feature.cpp \
    ../../src/elementdependencies.cpp \
    ../../src/element.cpp \
    ../../src/coordinatesystem.cpp \
    ../../src/connectionconfig.cpp \
    ../../src/configuration.cpp \
    ../../src/geometry/sphere.cpp \
    ../../src/geometry/scalarentitytemperature.cpp \
    ../../src/geometry/scalarentitymeasurementseries.cpp \
    ../../src/geometry/scalarentitydistance.cpp \
    ../../src/geometry/scalarentityangle.cpp \
    ../../src/geometry/pointcloud.cpp \
    ../../src/geometry/point.cpp \
    ../../src/geometry/plane.cpp \
    ../../src/geometry/paraboloid.cpp \
    ../../src/geometry/nurbs.cpp \
    ../../src/geometry/line.cpp \
    ../../src/geometry/hyperboloid.cpp \
    ../../src/geometry/ellipsoid.cpp \
    ../../src/geometry/cylinder.cpp \
    ../../src/geometry/cone.cpp \
    ../../src/geometry/circle.cpp \
    functions/systemTransformation/p_extendedtemperaturecompensation.cpp \
    functions/systemTransformation/materials.cpp \
    functions/systemTransformation/p_helmert6param.cpp \
    functions/fit/p_bestfitsphererauls.cpp \
    ../../src/simulation/uncertaintycomponent.cpp \
    simulations/simplePolarMeasurement/simplepolarmeasurement.cpp \
    ../../src/simulation/simulationdata.cpp


>>>>>>> simulation
HEADERS += p_factory.h \
    ../../lib/openIndyLib/include/global.h \
    ../../lib/openIndyLib/include/linearalgebra.h \
    ../../lib/openIndyLib/include/oimat.h \
    ../../lib/openIndyLib/include/oivec.h \
    functions/construct/p_intersectlineplane.h \
    functions/construct/p_intersectplaneplane.h \
    functions/construct/p_linefrompoints.h \
    functions/construct/p_planefrompoints.h \
    functions/construct/p_pointbetweentwopoints.h \
    functions/construct/p_pointfrompoints.h \
    functions/construct/p_spherefrompoints.h \
    functions/fit/p_bestfitline.h \
    functions/fit/p_bestfitplane.h \
    functions/fit/p_bestfitplane2.h \
    functions/fit/p_bestfitpoint.h \
    functions/fit/p_bestfitsphere.h \
    functions/objectTransformation/p_changeradius.h \
    functions/objectTransformation/p_projectInPlane.h \
    functions/objectTransformation/p_translateByLine.h \
    functions/objectTransformation/p_translateByPlane.h \
    functions/systemTransformation/p_helmert7Param.h \
    sensors/laserTracker/pseudoTracker/p_pseudotracker.h \
    sensors/tachymeter/LeicaGeoCom/p_leicatachymeter.h \
    sensors/undefinedSensor/pseudoSensor/p_pseudosensor.h \
    lib/fitting/cfitting_approxfunc.h \
    lib/fitting/cfitting_circle.h \
    lib/fitting/cfitting_ellipse.h \
    lib/fitting/cfitting_it.h \
    lib/fitting/cfitting_matrix.h \
    lib/fitting/cfitting_plane.h \
    lib/fitting/cfitting_sphere.h \
    lib/fitting/cfitting_util.h \
    lib/fitting/util/getopt.h \
    lib/fitting/util/iskeyword.h \
    lib/fitting/util/microtim.h \
    lib/fitting/util/mystring.h \
    lib/fitting/util/rho.h \
    functions/objectTransformation/p_shiftplane.h \
    functions/fit/p_bestfitcircle.h \
    functions/systemTransformation/p_simpletemperaturecompensation.h \
    ../../src/unitconverter.h \
    ../../src/trafoparam.h \
    ../../src/statistic.h \
    ../../src/station.h \
    ../../src/sensorlistener.h \
    ../../src/sensorcontrol.h \
    ../../src/sensorconfiguration.h \
    ../../src/sensor.h \
    ../../src/residual.h \
    ../../src/reading.h \
    ../../src/pluginmetadata.h \
    ../../src/oimetadata.h \
    ../../src/oiemitter.h \
    ../../src/observation.h \
    ../../src/measurementconfig.h \
    ../../src/geometry.h \
    ../../src/functionconfiguration.h \
    ../../src/function.h \
    ../../src/featurewrapper.h \
    ../../src/feature.h \
    ../../src/elementdependencies.h \
    ../../src/element.h \
    ../../src/coordinatesystem.h \
    ../../src/connectionconfig.h \
    ../../src/configuration.h \
    ../../src/plugin/pi_totalstation.h \
    ../../src/plugin/pi_systemtransformation.h \
    ../../src/plugin/pi_oiplugin.h \
    ../../src/plugin/pi_objecttransformation.h \
    ../../src/plugin/pi_networkadjustment.h \
    ../../src/plugin/pi_lasertracker.h \
    ../../src/plugin/pi_geodeticfunction.h \
    ../../src/plugin/pi_fitfunction.h \
    ../../src/plugin/pi_constructfunction.h \
    ../../src/geometry/sphere.h \
    ../../src/geometry/scalarentitytemperature.h \
    ../../src/geometry/scalarentitymeasurementseries.h \
    ../../src/geometry/scalarentitydistance.h \
    ../../src/geometry/scalarentityangle.h \
    ../../src/geometry/pointcloud.h \
    ../../src/geometry/point.h \
    ../../src/geometry/plane.h \
    ../../src/geometry/paraboloid.h \
    ../../src/geometry/nurbs.h \
    ../../src/geometry/line.h \
    ../../src/geometry/hyperboloid.h \
    ../../src/geometry/ellipsoid.h \
    ../../src/geometry/cylinder.h \
    ../../src/geometry/cone.h \
    ../../src/geometry/circle.h \
    functions/systemTransformation/p_extendedtemperaturecompensation.h \
    functions/systemTransformation/materials.h \
    functions/systemTransformation/p_helmert6param.h \
    functions/fit/p_bestfitsphererauls.h \
    ../../src/simulation/simulationmodel.h \
    ../../src/simulation/uncertaintycomponent.h \
    simulations/simplePolarMeasurement/simplepolarmeasurement.h \
    ../../src/simulation/simulationdata.h

OTHER_FILES += metaInfo.json
