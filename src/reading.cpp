#include "reading.h"

#include "sensor.h"
#include "observation.h"

Reading::Reading()
{
    this->measuredAt = QDateTime::currentDateTime();
}

Reading::~Reading(){

}

void Reading::toCartesian(){

    if(rPolar.isValid){

        this->rCartesian.xyz = OiVec(4);
        this->rCartesian.xyz.setAt( 0, this->rPolar.distance * qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth) );
        this->rCartesian.xyz.setAt( 1, this->rPolar.distance * qSin(this->rPolar.zenith) * qSin(this->rPolar.azimuth) );
        this->rCartesian.xyz.setAt( 2, this->rPolar.distance * qCos(this->rPolar.zenith) );
        this->rCartesian.xyz.setAt( 3, 1.0 );

    }

}

OiVec Reading::toCartesian(double az, double ze, double dist){
    OiVec g(4);
    g.setAt( 0, dist * qSin(ze) * qCos(az) );
    g.setAt( 1, dist * qSin(ze) * qSin(az) );
    g.setAt( 2, dist * qCos(ze) );
    g.setAt( 3, 1.0 );
    return g;
}

void Reading::toPolar(){
    return;
}

OiVec Reading::toPolar(double x, double y, double z){
    OiVec g(4);

    double azimuth = qAtan2(y,x);
    double s = qSqrt(x*x+y*y+z*z);
    double zenith = acos(z/s);

    g.setAt( 0, azimuth);
    g.setAt( 1, zenith);
    g.setAt( 2, s);
    g.setAt( 3, 1.0 );
    return g;
}

void Reading::makeBackup()
{
    this->backupPolar = this->rPolar;
    this->backupCartesian = this->rCartesian;
    this->backupDirection = this->rDirection;
    this->backupDistance = this->rDistance;
    this->backupTemperature = this->rTemperature;
    this->backupUndefined = this->rUndefined;
    this->backupLevel = this->rLevel;
}

void Reading::restoreBackup()
{
    this->rPolar = this->backupPolar;
    this->rCartesian = this->backupCartesian;
    this->rDirection = this->backupDirection;
    this->rDistance = this->backupDistance;
    this->rTemperature = this->backupTemperature;
    this->rUndefined = this->backupUndefined;
    this->rLevel = this->backupLevel;
}

/*!
 * \brief Reading::errorPropagationPolarToCart
 * Variance propagation to get sigma values for cartesian coordinates
 * \return
 */
OiVec Reading::errorPropagationPolarToCart(){
    OiVec sigmaCartXyz;

    OiMat F(3,3);
    F.setAt(0, 0, qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(0, 1, this->rPolar.distance * qSin(this->rPolar.zenith) * -qSin(this->rPolar.azimuth));
    F.setAt(0, 2, this->rPolar.distance * qCos(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(1, 0, qSin(this->rPolar.zenith) * qSin(this->rPolar.azimuth));
    F.setAt(1, 1, this->rPolar.distance * qSin(this->rPolar.zenith) * qCos(this->rPolar.azimuth));
    F.setAt(1, 2, this->rPolar.distance * qCos(this->rPolar.zenith) * qSin(this->rPolar.azimuth));
    F.setAt(2, 0, qCos(this->rPolar.zenith));
    F.setAt(2, 1, 0.0);
    F.setAt(2, 2, this->rPolar.distance * -qSin(this->rPolar.zenith));

    OiMat Sll(3,3);
    Sll.setAt(0, 0, this->rPolar.sigmaDistance * this->rPolar.sigmaDistance);
    Sll.setAt(1, 1, this->rPolar.sigmaAzimuth * this->rPolar.sigmaAzimuth);
    Sll.setAt(2, 2, this->rPolar.sigmaZenith * this->rPolar.sigmaZenith);

    OiMat Qxx = F * Sll * F.t();

    //transform Qxx into homogeneous coordinates
    OiMat Qxx_hc(4,4);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            Qxx_hc.setAt(i,j, Qxx.getAt(i,j));
        }
    }

    sigmaCartXyz.add(qSqrt(Qxx.getAt(0,0)));
    sigmaCartXyz.add(qSqrt(Qxx.getAt(1,1)));
    sigmaCartXyz.add(qSqrt(Qxx.getAt(2,2)));
    sigmaCartXyz.add(1.0);

    if(this->obs != NULL){
        this->obs->myStatistic.qxx = Qxx_hc;
        this->obs->myStatistic.s0_apriori = 1.0;
        this->obs->myOriginalStatistic = this->obs->myStatistic;
    }

    return sigmaCartXyz;
}

/*!
 * \brief Reading::toOpenIndyXML
 * \param xmlDoc
 * \return
 */
QDomElement Reading::toOpenIndyXML(QDomDocument &xmlDoc) const{

    if(xmlDoc.isNull()){
        return QDomElement();
    }

    QDomElement reading = xmlDoc.createElement("reading");

    //add reading attributes
    reading.setAttribute("id", this->id);
    reading.setAttribute("time", this->measuredAt.toString(Qt::ISODate));
    reading.setAttribute("type", Configuration::getReadingTypeString(this->typeofReading));

    //add measurements
    QDomElement measurements = xmlDoc.createElement("measurements");
    switch(this->typeofReading){
    case Configuration::eCartesian:
        if(this->rCartesian.isValid && this->rCartesian.xyz.getSize() >= 3 && this->rCartesian.sigmaXyz.getSize() >= 3){
            QDomElement x = xmlDoc.createElement("measurement");
            x.setAttribute("type", "x");
            x.setAttribute("value", this->rCartesian.xyz.getAt(0));
            x.setAttribute("sigma", this->rCartesian.sigmaXyz.getAt(0));
            measurements.appendChild(x);
            QDomElement y = xmlDoc.createElement("measurement");
            y.setAttribute("type", "y");
            y.setAttribute("value", this->rCartesian.xyz.getAt(1));
            y.setAttribute("sigma", this->rCartesian.sigmaXyz.getAt(1));
            measurements.appendChild(y);
            QDomElement z = xmlDoc.createElement("measurement");
            z.setAttribute("type", "z");
            z.setAttribute("value", this->rCartesian.xyz.getAt(2));
            z.setAttribute("sigma", this->rCartesian.sigmaXyz.getAt(2));
            measurements.appendChild(z);
        }
        break;
    case Configuration::eDirection:
        if(this->rDirection.isValid){
            QDomElement azimuth = xmlDoc.createElement("measurement");
            azimuth.setAttribute("type", "azimuth");
            azimuth.setAttribute("value", this->rDirection.azimuth);
            azimuth.setAttribute("sigma", this->rDirection.sigmaAzimuth);
            measurements.appendChild(azimuth);
            QDomElement zenith = xmlDoc.createElement("measurement");
            zenith.setAttribute("type", "zenith");
            zenith.setAttribute("value", this->rDirection.zenith);
            zenith.setAttribute("sigma", this->rDirection.sigmaZenith);
            measurements.appendChild(zenith);
        }
        break;
    case Configuration::eDistance:
        if(this->rDistance.isValid){
            QDomElement distance = xmlDoc.createElement("measurement");
            distance.setAttribute("type", "distance");
            distance.setAttribute("value", this->rDistance.distance);
            distance.setAttribute("sigma", this->rDistance.sigmaDistance);
            measurements.appendChild(distance);
        }
        break;
    case Configuration::ePolar:
        if(this->rPolar.isValid){
            QDomElement azimuth = xmlDoc.createElement("measurement");
            azimuth.setAttribute("type", "azimuth");
            azimuth.setAttribute("value", this->rPolar.azimuth);
            azimuth.setAttribute("sigma", this->rPolar.sigmaAzimuth);
            measurements.appendChild(azimuth);
            QDomElement zenith = xmlDoc.createElement("measurement");
            zenith.setAttribute("type", "zenith");
            zenith.setAttribute("value", this->rPolar.zenith);
            zenith.setAttribute("sigma", this->rPolar.sigmaZenith);
            measurements.appendChild(zenith);
            QDomElement distance = xmlDoc.createElement("measurement");
            distance.setAttribute("type", "distance");
            distance.setAttribute("value", this->rPolar.distance);
            distance.setAttribute("sigma", this->rPolar.sigmaDistance);
            measurements.appendChild(distance);
        }
        break;
    case Configuration::eTemperatur:
        if(this->rTemperature.isValid){
            QDomElement temperature = xmlDoc.createElement("measurement");
            temperature.setAttribute("type", "temperature");
            temperature.setAttribute("value", this->rTemperature.tempDeg);
            temperature.setAttribute("sigma", this->rTemperature.sigmaTempDeg);
            measurements.appendChild(temperature);
        }
        break;
    case Configuration::eLevel:
        if(this->rLevel.isValid){
            QDomElement rx = xmlDoc.createElement("measurement");
            rx.setAttribute("type", "RX");
            rx.setAttribute("value", this->rLevel.RX);
            rx.setAttribute("sigma", this->rLevel.sigmaRX);
            measurements.appendChild(rx);
            QDomElement ry = xmlDoc.createElement("measurement");
            ry.setAttribute("type", "zenith");
            ry.setAttribute("value", this->rLevel.RY);
            ry.setAttribute("sigma", this->rLevel.sigmaRY);
            measurements.appendChild(ry);
            QDomElement rz = xmlDoc.createElement("measurement");
            rz.setAttribute("type", "zenith");
            rz.setAttribute("value", this->rLevel.RZ);
            rz.setAttribute("sigma", this->rLevel.sigmaRZ);
            measurements.appendChild(rz);
        }
        break;
    case Configuration::eUndefined:
        if(this->rUndefined.isValid && this->rUndefined.values.size() == this->rUndefined.sigmaValues.size()){
            QList<QString> measurementTypes = this->rUndefined.values.keys();
            foreach(QString type, measurementTypes){
                if(this->rUndefined.sigmaValues.contains(type)){
                    QDomElement measurement = xmlDoc.createElement("measurement");
                    measurement.setAttribute("type", type);
                    measurement.setAttribute("value", this->rUndefined.values.value(type));
                    measurement.setAttribute("sigma", this->rUndefined.sigmaValues.value(type));
                    measurements.appendChild(measurement);
                }
            }
        }
        break;
    }
    reading.appendChild(measurements);

    return reading;

}
