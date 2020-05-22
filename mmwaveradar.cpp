#include "mmwaveradar.h"

mmWaveRadar::mmWaveRadar(QObject *parent) : QTcpSocket(parent)
{
    //将费时间的fft计算移到另一个线程里
    this->FFTThread=new QThread();
    this->FreqDomain=new mmWaveRadarFFT(this->TimeData);
    this->FreqDomain->moveToThread(this->FFTThread);
    this->FFTThread->start();
    QObject::connect(this,SIGNAL(GetFullFrame()),this->FreqDomain,SLOT(ProcessFFT()));
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

//转换大小端模式，默认接收数据是大端模式，但是在x86系统上是使用的小端模式，所以需要转换
void mmWaveRadar::ConvertEndian()
{
    this->Data.RadarData.RadarHead.Length=qFromBigEndian(this->Data.RadarData.RadarHead.Length);
    this->Data.RadarData.RadarHead.Slope=qFromBigEndian(this->Data.RadarData.RadarHead.Slope);
    this->Data.RadarData.RadarHead.DataID=qFromBigEndian(this->Data.RadarData.RadarHead.DataID);
    this->Data.RadarData.RadarHead.FirstFlag=qFromBigEndian(this->Data.RadarData.RadarHead.FirstFlag);
    this->Data.RadarData.RadarHead.SampleRate=qFromBigEndian(this->Data.RadarData.RadarHead.SampleRate);
    this->Data.RadarData.RadarHead.SamplePoint=qFromBigEndian(this->Data.RadarData.RadarHead.SamplePoint);
    this->Data.RadarData.RadarHead.ChirpNumber=qFromBigEndian(this->Data.RadarData.RadarHead.ChirpNumber);
    this->Data.RadarData.RadarHead.FrameNumber=qFromBigEndian(this->Data.RadarData.RadarHead.FrameNumber);
    for(int i=0;i<(this->Data.RadarData.RadarHead.Length-20)/2;i++)
    {
        this->Data.RadarData.RadarPayload[i]=qFromBigEndian(this->Data.RadarData.RadarPayload[i]);
        qDebug("%d",this->Data.RadarData.RadarPayload[i]);
    }
}

bool mmWaveRadar::ReadRadarData()
{
    QByteArray value=this->readAll();//读取收到的所有数据
    qDebug()<<value;
    std::string receive(value.toStdString());
    receive.copy(this->Data.ReceiveBuffer,receive.size());//把这些数据复制到接受缓冲区内
    this->ConvertEndian();
    qDebug()<<receive.length()<<this->Data.RadarData.RadarHead.Length;
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

void mmWaveRadar::RadarBufferCompress()
{
    if(this->Data.RadarData.RadarHead.FirstFlag!=0)
    {
        this->TimeData.clear();
        for(int i=0;i<ReceiveBuffer.size();i++)//将buffer中的数据取出并转换类型
        {
            this->TimeData.push_back((double)this->ReceiveBuffer[i]);
        }
        this->AllReceivedData.push_back(this->ReceiveBuffer);//将时域数据保存到存储全部数据的容器中
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
    this->AllReceivedData.clear();
    this->FreqDomain->ClearCache();
}
