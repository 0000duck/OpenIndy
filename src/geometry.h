#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QObject>
#include <QPointer>
#include <QList>
#include <QtXml>

#include "feature.h"
#include "measurementconfig.h"
#include "statistic.h"
#include "simulationdata.h"
#include "radius.h"
#include "direction.h"
#include "position.h"

class Observation;
class CoordinateSystem;

/*!
 * \brief The Geometry class
 *
 */
class Geometry : public Feature
{
    Q_OBJECT

public:
    explicit Geometry(const bool &isNominal, QObject *parent = 0);

    Geometry(const Geometry &copy, QObject *parent = 0);

    Geometry &operator=(const Geometry &copy);

    virtual ~Geometry();

    //##############################
    //get or set geometry attributes
    //##############################

    const bool &getIsCommon() const;
    void setCommonState(const bool &isCommon);

    const bool &getIsNominal() const;
    //QString getDisplayIsNominal() const;

    const QList<QPointer<Geometry> > &getNominals() const;
    bool addNominal(const QPointer<Geometry> &nominal);
    bool removeNominal(const QPointer<Geometry> &nominal);

    const QPointer<Geometry> &getActual() const;
    bool setActual(const QPointer<Geometry> &actual);

    //QString getDisplayObs() const;
    const QList<QPointer<Observation> > &getObservations() const;
    bool addObservation(const QPointer<Observation> &obs);
    bool removeObservation(const QPointer<Observation> &obs);

    const QPointer<CoordinateSystem> &getNominalSystem() const;
    bool setNominalSystem(const QPointer<CoordinateSystem> &nomSys);
	
    const MeasurementConfig &getMeasurementConfig() const;
    void setMeasurementConfig(const MeasurementConfig &myConfig);

    const Statistic &getStatistic() const;
    void setStatistic(const Statistic &myStatistic);

    const SimulationData &getSimulationData();
    void setSimulationData(const SimulationData &s);

    //###################################################################
    //check wether this geometry has a direction, a position and a radius
    //###################################################################

    virtual bool hasDirection() const;
    virtual bool hasPosition() const;
    virtual bool hasRadius() const;

    //#############################################################
    //getter to access radius, direction and position of a geometry
    //#############################################################

    virtual const Radius &getRadius() const;
    virtual const Direction &getDirection() const;
    virtual const Position &getPosition() const;

    //###########################
    //reexecute the function list
    //###########################

    virtual void recalc();

    //QMap<ReadingTypes, QString> getUsedReadingTypes() const;
    //void removeReadingType(ReadingTypes rType);

    //#################
    //save and load XML
    //#################

    virtual QDomElement toOpenIndyXML(QDomDocument &xmlDoc) const;
    virtual bool fromOpenIndyXML(QDomElement &xmlElem);

    //virtual bool saveSimulationData() = 0;
    //void resetSimulationData();

private:
    //void insertReadingType(ReadingTypes readingType, QString displayName);

signals:

    //########################################
    //signals to inform about geometry changes
    //########################################

    void geomIsCommonChanged(const int &featureId);
    void geomNominalsChanged(const int &featureId);
    void geomActualChanged(const int &featureId);
    void geomObservationsChanged(const int &featureId);
    void geomNominalSystemChanged(const int &featureId);
    void geomStatisticChanged(const int &featureId);
    void geomMeasurementConfigChanged(const int &featureId);
    //void geomUsedReadingTypesChanged(const int &featureId);

protected:

    //###################
    //geometry attributes
    //###################

    bool isCommon;
    bool isNominal;
    QList< QPointer<Geometry> > nominals;
    QPointer<Geometry> actual;
    QList<Observation*> observations;
    QPointer<CoordinateSystem> nominalSystem;

    Statistic statistic;
    SimulationData simulationData;
    //QMap<ReadingTypes, QString> usedReadingTypes;

    //current measurement config that is used to create readings until the user selects another one
    MeasurementConfig activeMeasurementConfig; //only for this geometry instance

private:

    //############################################################################
    //helper variables used for geometries without a radius, direction or position
    //############################################################################

    Position dummyPosition;
    Direction dummyDirection;
    Radius dummyRadius;

};

#endif // GEOMETRY_H
