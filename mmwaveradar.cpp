#include "mmwaveradar.h"

mmWaveRadar::mmWaveRadar(QObject *parent) : QTcpSocket(parent)
{

}

void mmWaveRadar::UpdateRadarParameter()
{
    qDebug("%s",this->Parameter.TranscodeParameter);
    QByteArray data(this->Parameter.TranscodeParameter,16);
    char head=0xBB;
    QByteArray headArray(&head,1);
    this->write(QByteArray(headArray+data));
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

bool mmWaveRadar::ReadRadarData()
{
    QByteArray value=this->readAll();
    qDebug()<<value;
    std::string receive=value.toStdString();
    receive.copy(this->Data.ReceiveBuffer,receive.size());
    qDebug()<<receive.length()<<this->Data.RadarData.RadarHead.Length;
    for(int i=0;i<=50;i++)
    {
        qDebug("%c",this->Data.ReceiveBuffer[i]);
    }
    if(receive.length()==this->Data.RadarData.RadarHead.Length) //验证长度正确
    {
        char ok=0xaa;
        this->write(&ok,1);
        qDebug("receive data correct");
        return true;
    }
    else
    {
        return false;
    }
}

mmWaveRadar::RadarHead_t mmWaveRadar::GetRadarHead()
{
    return this->Data.RadarData.RadarHead;
}

bool mmWaveRadar::RadarBufferProcess()
{
    if(this->Data.RadarData.RadarHead.FirstFlag==1)
    {

        return true;
    }
    else
    {
        return false;
    }
}


void mmWaveRadar::ClearRadarCache()
{
    this->X.clear();
    this->TimeData.clear();
    this->FreqData.clear();
    this->PhaseData.clear();

    this->FrameDataBuffer.clear();
}
