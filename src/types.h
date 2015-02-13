#ifndef TYPES_H
#define TYPES_H

//#######################
//available element types
//#######################

//! all OpenIndy element types
enum ElementTypes{
    eUndefinedElement,
    ePointElement,
    eLineElement,
    ePlaneElement,
    eSphereElement,
    eScalarEntityAngleElement,
    eScalarEntityDistanceElement,
    eScalarEntityTemperatureElement,
    eStationElement,
    eCoordinateSystemElement,
    eTrafoParamElement,
    ePointCloudElement,
    eCircleElement,
    eConeElement,
    eCylinderElement,
    eEllipsoidElement,
    eHyperboloidElement,
    eParaboloidElement,
    eNurbsElement,
    eObservationElement,
    eReadingCartesianElement,
    eReadingPolarElement,
    eReadingDistanceElement,
    eReadingDirectionElement,
    eReadingTemperatureElement,
    eReadingLevelElement,
    eScalarEntityMeasurementSeriesElement
};

//! all OpenIndy feature types
enum FeatureTypes{
    ePointFeature,
    eLineFeature,
    ePlaneFeature,
    eSphereFeature,
    eStationFeature,
    eCoordinateSystemFeature,
    eTrafoParamFeature,
    ePointCloudFeature,
    eCircleFeature,
    eConeFeature,
    eCylinderFeature,
    eEllipsoidFeature,
    eHyperboloidFeature,
    eNurbsFeature,
    eParaboloidFeature,
    eScalarEntityAngleFeature,
    eScalarEntityDistanceFeature,
    eScalarEntityTemperatureFeature,
    eScalarEntityMeasurementSeriesFeature
};

//! all OpenIndy geometry types
enum GeometryTypes{
    ePointGeometry,
    eLineGeometry,
    ePlaneGeometry,
    eSphereGeometry,
    ePointCloudGeometry,
    eCircleGeometry,
    eConeGeometry,
    eCylinderGeometry,
    eEllipsoidGeometry,
    eHyperboloidGeometry,
    eNurbsGeometry,
    eParaboloidGeometry,
    eScalarEntityAngleGeometry,
    eScalarEntityDistanceGeometry,
    eScalarEntityTemperatureGeometry,
    eScalarEntityMeasurementSeriesGeometry
};


#endif // TYPES_H
