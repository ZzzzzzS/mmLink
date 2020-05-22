#ifndef MMWAVERADAR_H
#define MMWAVERADAR_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QVector>
#include <vector>
#include <opencv2/opencv.hpp>
#include <QThread>

#include "mmwaveradarfft.h"

#define MAX_TCPBuffer 65535


class mmWaveRadar : public QTcpSocket
{
    Q_OBJECT
public:

    //发送的参数
    typedef struct
    {
        int SampleRate;
        int SamplePoint;
        int ChirpNumber;
        int Slope;
    }RadarParameter_t;

    typedef union
    {
        RadarParameter_t RadarParameter;
        char TranscodeParameter[16];
    }UnionParamter_t;

    //接收的参数
    typedef struct
    {
        uint Length;
        int Slope;
        short DataID;
        short FirstFlag;
        short SampleRate;
        short SamplePoint;
        short ChirpNumber;
        short FrameNumber;
    }RadarHead_t;

    typedef struct
    {
        RadarHead_t RadarHead;
        short RadarPayload[(MAX_TCPBuffer-20)/2+1];
    }RadarData_t;

    typedef union
    {
        RadarData_t RadarData;
        char ReceiveBuffer[MAX_TCPBuffer];
    }UnionData_t;

    std::vector<double> TimeData;//时域数据

    mmWaveRadar(QObject *parent = nullptr);
    void UpdateRadarParameter();//更新参数
    bool isParameterLegal();//判断参数是否合法
    void ClearRadarCache();//清除缓存
    void SetRadarParameter(RadarParameter_t value);//设置雷达参数
    bool ReadRadarData();
    void RadarBufferCompress();//压缩合并多帧雷达数据
    RadarHead_t GetRadarHead();

    mmWaveRadarFFT *FreqDomain;

signals:
    void GetFullFrame();
private:


    UnionParamter_t Parameter; //设定的雷达参数
    UnionData_t Data;//接收的雷达数据

    std::vector<short> ReceiveBuffer;//接收缓冲区
    std::vector<std::vector<short>> AllReceivedData;//保存所有的时域数据

    QThread *FFTThread;

};

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // MMWAVERADAR_H
