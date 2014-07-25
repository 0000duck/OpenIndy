#include "simplepolarmeasurement.h"

SimplePolarMeasurement::SimplePolarMeasurement()
{
    this->distributions.append("normal");
    this->distributions.append("uniform");
    this->distributions.append("triangular");
}

PluginMetaData *SimplePolarMeasurement::getMetaData()
{
    PluginMetaData* metaData = new PluginMetaData();

    metaData->name = "SimplePolarMeasurement";
    metaData->pluginName = "OpenIndy Default Plugin";
    metaData->author = "mlux";
    metaData->description = "simulation of a simple polar measurement system";
    metaData->iid = "de.openIndy.Plugin.Simulation.v001";
    //...

    return metaData;
}

double densityNormal(double x, double expectation, double uncertainty)
{
    double e = -0.5*(((x-expectation)/(uncertainty))*((x-expectation)/(uncertainty)));

    double result = (1.0/(uncertainty*sqrt(2.0*M_PI)))*qExp(e);

    return result;
}

double distributionNormal(double x, double expectation, double uncertainty)
{
    //return 0.5*(1.0+erf((x-expectation)/(sqrt(2.0*uncertainty*uncertainty))));
    return 0.0;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getSensorUncertainties()
{
    QMap<QString, UncertaintyComponent> sensorUncertainties;

    //######################### lambda #########################
    UncertaintyComponent lambda;

    lambda.name = "lambda";
    lambda.value = 0.0;
    lambda.uncertainty = 0.000403;
    lambda.distribution = this->distributions.at(0);
    lambda.distributions = this->distributions;
    lambda.errorUnit = "[mm]";

    lambda.description="lambda - Range offset in millimeter";

    sensorUncertainties.insert("lambda",lambda);

    //######################### mu #########################
    UncertaintyComponent mu;

    mu.name = "mu";
    mu.value = 0.0;
    mu.uncertainty = 0.000005;
    mu.distribution = this->distributions.at(0);
    mu.distributions = this->distributions;
    mu.errorUnit = "[-]";

    mu.description="mu - Scale factor for range";

    sensorUncertainties.insert("mu",mu);

    //######################### ex #########################
    UncertaintyComponent ex;

    ex.name = "ex";
    ex.value = 0.0;
    ex.uncertainty = 0.0000122;
    ex.distribution = this->distributions.at(0);
    ex.distributions = this->distributions;
    ex.errorUnit = "[mm]";

    ex.description="ex - Transit axis offset from the standing axis";

    sensorUncertainties.insert("ex",ex);

    //######################### by #########################
    UncertaintyComponent by;

    by.name = "by";
    by.value = 0.0;
    by.uncertainty = 0.0000654;
    by.distribution = this->distributions.at(0);
    by.distributions = this->distributions;
    by.errorUnit =  "[mm]";

    by.description="by - Beam offset (y-direction) from the origin";

    sensorUncertainties.insert("by",by);

    //######################### bz #########################
    UncertaintyComponent bz;

    bz.name = "bz";
    bz.value = 0.0;
    bz.uncertainty = 0.0000974;
    bz.distribution = this->distributions.at(0);
    bz.distributions = this->distributions;
    bz.errorUnit =  "[mm]";

    bz.description="bz - Beam offset (z-direction) from the origin";

    sensorUncertainties.insert("bz",bz);

    //######################### alpha #########################
    UncertaintyComponent alpha;

    alpha.name = "alpha";
    alpha.value = 0.0;
    alpha.uncertainty = 0.128;
    alpha.distribution = this->distributions.at(0);
    alpha.distributions = this->distributions;
    alpha.errorUnit =  "[arcsec]";

    alpha.description="alpha - Transit axis angle in the yz-plane";

    sensorUncertainties.insert("alpha",alpha);

    //######################### gamma #########################
    UncertaintyComponent gamma;

    gamma.name = "gamma";
    gamma.value = 0.0;
    gamma.uncertainty = 0.079;
    gamma.distribution = this->distributions.at(0);
    gamma.distributions = this->distributions;
    gamma.errorUnit = "[arcsec]";

    gamma.description="gamma - Beam axis angle in the xy-plane";

    sensorUncertainties.insert("gamma",gamma);

    //######################### Aa1 #########################
    UncertaintyComponent Aa1;

    Aa1.name = "Aa1";
    Aa1.value = 0.0;
    Aa1.uncertainty = 0.064;
    Aa1.distribution = this->distributions.at(0);
    Aa1.distributions = this->distributions;
    Aa1.errorUnit = "[arcsec]";

    Aa1.description="Aa1 - Azimuth scale error, first order";

    sensorUncertainties.insert("Aa1",Aa1);

    //######################### Ba1 #########################
    UncertaintyComponent Ba1;

    Ba1.name = "Aa1";
    Ba1.value = 0.0;
    Ba1.uncertainty = 0.080;
    Ba1.distribution = this->distributions.at(0);
    Ba1.distributions = this->distributions;
    Ba1.errorUnit = "[arcsec]";

    Ba1.description="Ba1 - Azimuth scale error, first order";

    sensorUncertainties.insert("Ba1",Ba1);

    //######################### Aa2 #########################
    UncertaintyComponent Aa2;

    Aa2.name = "Aa2";
    Aa2.value = 0.0;
    Aa2.uncertainty = 0.073;
    Aa2.distribution = this->distributions.at(0);
    Aa2.distributions = this->distributions;
    Aa2.errorUnit = "[arcsec]";

    Aa2.description="Aa2 - Azimuth scale error, second order";

    sensorUncertainties.insert("Aa2",Aa2);

    //######################### Ba2 #########################
    UncertaintyComponent Ba2;

    Ba2.name = "Ba2";
    Ba2.value = 0.0;
    Ba2.uncertainty = 0.090;
    Ba2.distribution = this->distributions.at(0);
    Ba2.distributions = this->distributions;
    Ba2.errorUnit = "[arcsec]";

    Ba2.description="Ba2 - Azimuth scale error, second order";

    sensorUncertainties.insert("Ba2",Ba2);

    //######################### Ba2 #########################
    UncertaintyComponent Ae0;

    Ae0.name = "Ae0";
    Ae0.value = 0.0;
    Ae0.uncertainty = 0.223;
    Ae0.distribution = this->distributions.at(0);
    Ae0.distributions = this->distributions;
    Ae0.errorUnit = "[arcsec]";

    Ae0.description="Ae0 - Elevation angle offset";

    sensorUncertainties.insert("Ae0",Ae0);

    //######################### Ae1 #########################
    UncertaintyComponent Ae1;

    Ae1.name = "Ae1";
    Ae1.value = 0.0;
    Ae1.uncertainty = 0.152;
    Ae1.distribution = this->distributions.at(0);
    Ae1.distributions = this->distributions;
    Ae1.errorUnit = "[arcsec]";

    Ae1.description="Ae1 - Elevation scale error, first order";

    sensorUncertainties.insert("Ae1",Ae1);

    //######################### Be1 #########################
    UncertaintyComponent Be1;

    Be1.name = "Be1";
    Be1.value = 0.0;
    Be1.uncertainty = 0.183;
    Be1.distribution = this->distributions.at(0);
    Be1.distributions = this->distributions;
    Be1.errorUnit = "[arcsec]";

    Be1.description="Be1 - Elevation scale error, first order";

    sensorUncertainties.insert("Be1",Be1);

    //######################### Ae2 #########################
    UncertaintyComponent Ae2;

    Ae2.name = "Ae2";
    Ae2.value = 0.0;
    Ae2.uncertainty = 0.214;
    Ae2.distribution = this->distributions.at(0);
    Ae2.distributions = this->distributions;
    Ae2.errorUnit = "[arcsec]";

    Ae2.description="Ae2 - Elevation scale error, second order";

    sensorUncertainties.insert("Ae2",Ae2);

    //######################### Be2 #########################
    UncertaintyComponent Be2;

    Be2.name = "Be2";
    Be2.value = 0.0;
    Be2.uncertainty = 0.214;
    Be2.distribution = this->distributions.at(0);
    Be2.distributions = this->distributions;
    Be2.errorUnit = "[arcsec]";

    Be2.description="Be2 - Elevation scale error, second order";

    sensorUncertainties.insert("Be2",Be2);


    return sensorUncertainties;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getObjectUncertainties()
{
    QMap<QString, UncertaintyComponent> objectUncertainties;

    //######################### coefficient of thermal expansion #########################
    UncertaintyComponent cOfE;

    cOfE.name = "coefficient of thermal expansion";
    cOfE.value = 11,8;
    cOfE.uncertainty = 1;
    cOfE.distribution = this->distributions.at(0);
    cOfE.distributions = this->distributions;
    cOfE.errorUnit = "1/K";

    cOfE.description="The degree of expansion divided by the change in temperaturer";

    objectUncertainties.insert("coefficientOfExpansion",cOfE);


    return objectUncertainties;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getEnviromentUncertainties()
{
    QMap<QString, UncertaintyComponent> enviromentUncertainties;

    //######################### temperature #########################
    UncertaintyComponent t;

    t.name = "temperature";
    t.value = 20;
    t.uncertainty = 0.5;
    t.distribution = this->distributions.at(0);
    t.distributions = this->distributions;
    t.errorUnit = "[celsius]";

    t.description="influence of temperaturer on the object and sensor";

    enviromentUncertainties.insert("temperature",t);

    //######################### pressure #########################
    UncertaintyComponent p;

    p.name = "pressure";
    p.value = 101325 ;
    p.uncertainty = 10;
    p.distribution = this->distributions.at(0);
    p.distributions = this->distributions;
    p.errorUnit = "[pascal]";

    p.description="influence of pressure(air)  on the sensor";

    enviromentUncertainties.insert("pressures ",p);

    //######################### humidity #########################
    UncertaintyComponent h;

    h.name = "humidity";
    h.value = 50 ;
    h.uncertainty = 5;
    h.distribution = this->distributions.at(0);
    h.distributions = this->distributions;
    h.errorUnit = "[percent %]";

    h.description="influence of humidity(air)  on the sensor";

    enviromentUncertainties.insert("humidity ",h);

    return enviromentUncertainties;
}

QMap<QString, UncertaintyComponent> SimplePolarMeasurement::getHumanInfluence()
{
    QMap<QString, UncertaintyComponent> humanInfluence;


    return humanInfluence;
}

QMap<QString, int> *SimplePolarMeasurement::getIntegerParameter()
{
    QMap<QString,int>* intParam = new QMap<QString,int>;

    intParam->insert("wavelength [nm]",633);

    return intParam;
}

QMap<QString, double> *SimplePolarMeasurement::getDoubleParameter()
{
    return NULL;
}

QMap<QString, QStringList> *SimplePolarMeasurement::getStringParameter()
{
    QMap <QString, QStringList>* stringParameter = new QMap<QString, QStringList>;

    QStringList dice;
    QStringList diceDefaultNo;

    dice.append("yes");
    dice.append("no");

    diceDefaultNo.append("no");
    diceDefaultNo.append("yes");

    stringParameter->insert("use sensor errors",dice);
    stringParameter->insert("use environment errors", dice);
    stringParameter->insert("use object errors", diceDefaultNo);
    stringParameter->insert("use human errors", diceDefaultNo);


    return stringParameter;
}

bool SimplePolarMeasurement::analyseSimulationData(UncertaintyData &d)
{


   if(d.values.size() != 0){

       double sumDD = 0.0;

       d.maxValue = d.values.at(0);
       d.minValue = d.values.at(0);

       for(int i = 0; i<d.values.size();i++){
           if(d.values.at(i) > d.maxValue){
               d.maxValue = d.values.at(i) ;
           }
           if(d.values.at(i)  < d.minValue){
               d.minValue = d.values.at(i) ;
           }
           sumDD += d.values.at(i);
       }

       d.expectation = sumDD/d.values.size();

      this->checkDistribution(d);
      this->calcUncertainty(d);

       d.densityFunction = densityNormal;
       d.distributionFunction = distributionNormal;

   }else{
    return false;
   }



    return true;
}

bool SimplePolarMeasurement::distort(Reading *r,OiMat objectRelation,bool newIterationStart)
{
    distortionBySensor(r);

    return true;
}

double SimplePolarMeasurement::distortComponent(UncertaintyComponent u)
{


    if(u.distribution.compare("normal")==0){
        std::normal_distribution<double> dist_Normal(u.value,u.uncertainty);
        std::random_device rd;
        std::default_random_engine generator(rd());

        return dist_Normal(generator);

    }else if(u.distribution.compare("uniform")==0){

        double a = u.value-u.uncertainty;
        double b = u.value+u.uncertainty;

        std::uniform_real_distribution<double> dist_Uniform(a,b);
        std::random_device rd;
        std::default_random_engine generator(rd());

        return dist_Uniform(generator);

    }else if (u.distribution.compare("triangular")==0){

        double a = u.value-u.uncertainty;
        double b = u.value+u.uncertainty;
        double c = u.value;

        std::uniform_real_distribution<double> dist_Uniform(0,1);
        std::random_device rd;
        std::default_random_engine generator(rd());

        double U = dist_Uniform(generator);
        double F = (c - a) / (b - a);

           if (U <= F)
              return a + sqrt(U * (b - a) * (c - a));
           else
               return b - sqrt((1 - U) * (b - a) * (b - c));
    }


    return u.value;
}

bool SimplePolarMeasurement::distortionBySensor(Reading *r)
{
    if(r->typeofReading != Configuration::ePolar){
        return false;
    }


    double lambda = distortComponent(givenUncertainties.sensorUncertainties.value("lambda"))/1000.0;
    double mu = distortComponent(givenUncertainties.sensorUncertainties.value("mu"));
    double ex = distortComponent(givenUncertainties.sensorUncertainties.value("ex"))/1000.0;
    double by = distortComponent(givenUncertainties.sensorUncertainties.value("by"))/1000.0;
    double bz = distortComponent(givenUncertainties.sensorUncertainties.value("bz"))/1000.0;
    double alpha = distortComponent(givenUncertainties.sensorUncertainties.value("alpha"))*(M_PI/648000.0);
    double gamma = distortComponent(givenUncertainties.sensorUncertainties.value("gamma"))*(M_PI/648000.0);
    double Aa1 = distortComponent(givenUncertainties.sensorUncertainties.value("Aa1"))*(M_PI/648000.0);
    double Ba1 = distortComponent(givenUncertainties.sensorUncertainties.value("Ba1"))*(M_PI/648000.0);
    double Aa2 = distortComponent(givenUncertainties.sensorUncertainties.value("Aa2"))*(M_PI/648000.0);
    double Ba2 = distortComponent(givenUncertainties.sensorUncertainties.value("Ba2"))*(M_PI/648000.0);
    double Ae0 = distortComponent(givenUncertainties.sensorUncertainties.value("Ae0"))*(M_PI/648000.0);
    double Ae1 = distortComponent(givenUncertainties.sensorUncertainties.value("Ae1"))*(M_PI/648000.0);
    double Be1 = distortComponent(givenUncertainties.sensorUncertainties.value("Be1"))*(M_PI/648000.0);
    double Ae2 = distortComponent(givenUncertainties.sensorUncertainties.value("Ae2"))*(M_PI/648000.0);
    double Be2 = distortComponent(givenUncertainties.sensorUncertainties.value("Be2"))*(M_PI/648000.0);

    double az = r->rPolar.azimuth;
    double ze = r->rPolar.zenith;
    double d = r->rPolar.distance;

    d = (1+mu)*d+lambda;

    double azF1 = Aa1*cos(az) + Ba1*sin(az);
    double azF2 = Aa2*cos(2*az) + Ba2*sin(2*az);

    az = az+azF1+azF2;

    double zeF1 = Ae1*cos(ze) + Be1*sin(ze);
    double zeF2 = Ae2*cos(2*ze) + Be2*sin(2*ze);

    ze = ze+Ae0+zeF1+zeF2;

    OiVec ebb;
    ebb.add(-ex);
    ebb.add(by);
    ebb.add(bz);

    OiVec e00;
    e00.add(ex);
    e00.add(0.0);
    e00.add(0.0);

    OiVec xAxis;
    xAxis.add(1);
    xAxis.add(0);
    xAxis.add(0);


    OiMat Rz_Azimuth = OiMat::getRotationMatrix(az,Rotation::Z_AXIS);
    OiMat Rx_alpha = OiMat::getRotationMatrix(alpha,Rotation::X_AXIS);
    OiMat Ry_zenith = OiMat::getRotationMatrix(ze-(M_PI/2.0),Rotation::Y_AXIS);
    OiMat Rx_minusAlpha = OiMat::getRotationMatrix(-1.0*alpha,Rotation::X_AXIS);
    OiMat Rz_gamma = OiMat::getRotationMatrix(gamma,Rotation::Z_AXIS);


    OiVec b(3);
    b = Rz_Azimuth*e00 + Rz_Azimuth*Rx_alpha*Ry_zenith*Rx_minusAlpha*ebb;;

    OiVec n(3);
    n = Rz_Azimuth*Rx_alpha*Ry_zenith*Rx_minusAlpha*Rz_gamma*xAxis;


    OiVec p(3);
    p = b+d*n;

    r->rPolar.azimuth = qAtan2(p.getAt(1),p.getAt(0));
    r->rPolar.distance = qSqrt(p.getAt(0)*p.getAt(0)+p.getAt(1)*p.getAt(1)+p.getAt(2)*p.getAt(2));
    r->rPolar.zenith = acos(p.getAt(2)/r->rPolar.distance);

    return true;
}

bool SimplePolarMeasurement::distortionByEnviroment(Reading *r)
{
    return false;
}

bool SimplePolarMeasurement::distortionByHuman(Reading *r)
{
    return false;
}

bool SimplePolarMeasurement::distortionByObject(Reading *r, OiMat objectRelation)
{
    return false;
}

void SimplePolarMeasurement::checkDistribution(UncertaintyData &d)
{
    d.distribution = "normal";
}

void SimplePolarMeasurement::calcUncertainty(UncertaintyData &d)
{

    if(d.distribution == "normal"){

        double sumVV = 0.0;


        foreach(double v, d.values){

            sumVV += (d.expectation-v)*(d.expectation-v);

        }

        d.uncertainty = sqrt(sumVV/(d.values.size()-1.0));
    }

}




