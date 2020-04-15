#include "mmwaveradar.h"

mmWaveRadar::mmWaveRadar(QObject *parent) : QObject(parent)
{

}

QByteArray mmWaveRadar::UpdateRadarParameter()
{
    qDebug("%s",this->Parameter.TranscodeParameter);
    QByteArray data(this->Parameter.TranscodeParameter,16);
    char head=0xBB;
    QByteArray headArray(&head,1);
    return QByteArray(headArray+data);
}

bool mmWaveRadar::isParameterLegal()
{
    if(this->Parameter.RadarParameter.SamplePoint<=0)
        return false;
    if(this->Parameter.RadarParameter.SampleRate<=0)
        return false;
    if(this->Parameter.RadarParameter.ChirpNumber<=0)
        return false;
    if(this->Parameter.RadarParameter.Slope<=0)
        return false;

    return true;

}

void mmWaveRadar::SetRadarParameter(RadarParameter_t value)
{
    this->Parameter.RadarParameter.SampleRate=value.SampleRate;
    this->Parameter.RadarParameter.SamplePoint=value.SamplePoint;
    this->Parameter.RadarParameter.ChirpNumber=value.ChirpNumber;
    this->Parameter.RadarParameter.Slope=value.Slope;
}
