#include "mmwaveradar.h"

mmWaveRadar::mmWaveRadar(QObject *parent) : QTcpSocket(parent)
{
    //将费时间的fft计算移到另一个线程里
    this->FFTThread=new QThread();
    this->FreqDomain=new mmWaveRadarFFT(this->TimeData);
    this->FreqDomain->moveToThread(this->FFTThread);
    this->FFTThread->start();
    QObject::connect(this,SIGNAL(GetFullFrame()),this->FreqDomain,SLOT(ProcessFFT()));
    this->RadarEndian=mmWaveRadar::LittleEndian;//设置默认大端模式
}

bool mmWaveRadar::UpdateRadarParameter()
{
    if(this->isParameterLegal())
    {
        qDebug("%s",this->Parameter.TranscodeParameter);
        QByteArray data(this->Parameter.TranscodeParameter,16);
        char head=0xBB;
        QByteArray headArray(&head,1);
        this->write(QByteArray(headArray+data));
        return true;
    }
    else
    {
        return false;
    }

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
    if(this->RadarEndian==mmWaveRadar::BigEndian)
    {
        qToBigEndian(value.SampleRate,&this->Parameter.RadarParameter.SampleRate);
        qToBigEndian(value.SamplePoint,&this->Parameter.RadarParameter.SamplePoint);
        qToBigEndian(value.ChirpNumber,&this->Parameter.RadarParameter.ChirpNumber);
        qToBigEndian(value.Slope,&this->Parameter.RadarParameter.Slope);
    }
    else
    {
        qToLittleEndian(value.SampleRate,&this->Parameter.RadarParameter.SampleRate);
        qToLittleEndian(value.SamplePoint,&this->Parameter.RadarParameter.SamplePoint);
        qToLittleEndian(value.ChirpNumber,&this->Parameter.RadarParameter.ChirpNumber);
        qToLittleEndian(value.Slope,&this->Parameter.RadarParameter.Slope);
    }
}

//转换大小端模式，默认接收数据是大端模式，但是在x86系统上是使用的小端模式，所以需要转换
void mmWaveRadar::ConvertEndian(int Length)
{
    if(this->RadarEndian==mmWaveRadar::BigEndian)
    {
        this->Data.RadarData.RadarHead.Length=qFromBigEndian(this->Data.RadarData.RadarHead.Length);
        this->Data.RadarData.RadarHead.Slope=qFromBigEndian(this->Data.RadarData.RadarHead.Slope);
        this->Data.RadarData.RadarHead.DataID=qFromBigEndian(this->Data.RadarData.RadarHead.DataID);
        this->Data.RadarData.RadarHead.FirstFlag=qFromBigEndian(this->Data.RadarData.RadarHead.FirstFlag);
        this->Data.RadarData.RadarHead.SampleRate=qFromBigEndian(this->Data.RadarData.RadarHead.SampleRate);
        this->Data.RadarData.RadarHead.SamplePoint=qFromBigEndian(this->Data.RadarData.RadarHead.SamplePoint);
        this->Data.RadarData.RadarHead.ChirpNumber=qFromBigEndian(this->Data.RadarData.RadarHead.ChirpNumber);
        this->Data.RadarData.RadarHead.FrameNumber=qFromBigEndian(this->Data.RadarData.RadarHead.FrameNumber);
        for(int i=0;i<(Length-20)/2;i++)
        {
            this->Data.RadarData.RadarPayload[i]=qFromBigEndian(this->Data.RadarData.RadarPayload[i]);
            //qDebug("%d",this->Data.RadarData.RadarPayload[i]);
        }
    }
    else
    {
        this->Data.RadarData.RadarHead.Length=qFromLittleEndian(this->Data.RadarData.RadarHead.Length);
        this->Data.RadarData.RadarHead.Slope=qFromLittleEndian(this->Data.RadarData.RadarHead.Slope);
        this->Data.RadarData.RadarHead.DataID=qFromLittleEndian(this->Data.RadarData.RadarHead.DataID);
        this->Data.RadarData.RadarHead.FirstFlag=qFromLittleEndian(this->Data.RadarData.RadarHead.FirstFlag);
        this->Data.RadarData.RadarHead.SampleRate=qFromLittleEndian(this->Data.RadarData.RadarHead.SampleRate);
        this->Data.RadarData.RadarHead.SamplePoint=qFromLittleEndian(this->Data.RadarData.RadarHead.SamplePoint);
        this->Data.RadarData.RadarHead.ChirpNumber=qFromLittleEndian(this->Data.RadarData.RadarHead.ChirpNumber);
        this->Data.RadarData.RadarHead.FrameNumber=qFromLittleEndian(this->Data.RadarData.RadarHead.FrameNumber);
        for(int i=0;i<(Length-20)/2;i++)
        {
            this->Data.RadarData.RadarPayload[i]=qFromLittleEndian(this->Data.RadarData.RadarPayload[i]);
        }
    }

}

bool mmWaveRadar::ReadRadarData()
{
    QByteArray value=this->readAll();//读取收到的所有数据
    qDebug()<<value;

    for(int i=0;i<value.size();i++)
    {
        this->Data.ReceiveBuffer[i]=*(value.data()+i);
    }
    this->ConvertEndian(value.length());
    if(value.length()==this->Data.RadarData.RadarHead.Length) //验证长度正确
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

void mmWaveRadar::RadarBufferCompress()
{
    if(this->Data.RadarData.RadarHead.FirstFlag!=0)
    {
        this->TimeData.clear();
        for(int i=0;i<ReceiveBuffer.size();i++)//将buffer中的数据取出并转换类型
        {
            this->TimeData.push_back((double)this->ReceiveBuffer[i]);
        }
        if(!this->ReceiveBuffer.empty())
        {
            emit(this->PushBackData(this->ReceiveBuffer));//将时域数据保存到数据库中
        }
        this->ReceiveBuffer.clear();//清除缓冲区
        if(!this->TimeData.empty())//判断一下到底接收到数据没有，以便后续显示不出错
        {
            emit(this->GetFullFrame());
        }
        else
        {
            qDebug("TimeData is empty");
        }

    }
    int number=(Data.RadarData.RadarHead.Length-20)/2;
    for(int i=0;i<number;i++)//重新组合多帧数据到buffer
    {
        this->ReceiveBuffer.push_back(*(this->Data.RadarData.RadarPayload+i));
        qDebug("%d",this->ReceiveBuffer.back());
    }
    qDebug("%d",this->ReceiveBuffer.size());

}


void mmWaveRadar::ClearRadarCache()
{
    this->TimeData.clear();
    this->ReceiveBuffer.clear();
    this->FreqDomain->ClearCache();
}
