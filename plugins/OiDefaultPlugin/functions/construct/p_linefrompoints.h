#ifndef P_LINEFROMPOINTS_H
#define P_LINEFROMPOINTS_H

#include <QtGlobal>
#include <QDateTime>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <qmath.h>
#include <vector>
#include "pi_constructfunction.h"
#include "line.h"
#include "configuration.h"
#include "oivec.h"
#include "oimat.h"
#include "pluginmetadata.h"

using namespace std;

class LineFromPoints : public ConstructFunction
{
public:
    PluginMetaData* getMetaData();
    bool exec(Line&);
    QList<InputParams> getNeededElements();
    QList<Configuration::FeatureTypes> applicableFor();

private:
    bool checkPointCount();
    bool setUpResult(Line&);
    OiMat preCalc(OiVec centroid);

    OiVec referenceDirection;

};

#endif // P_LINEFROMPOINTS_H
