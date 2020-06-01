/****************************************************************************
MIT License

Copyright (c) 2020 ZhouZishun

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/

#ifndef MMWAVERADAR_H
#define MMWAVERADAR_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QVector>
#include <vector>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QtEndian>

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
        int Length;
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
    bool UpdateRadarParameter();//更新参数

    void ClearRadarCache();//清除缓存
    void SetRadarParameter(RadarParameter_t value);//设置雷达参数
    bool ReadRadarData();
    void RadarBufferCompress();//压缩合并多帧雷达数据
    RadarHead_t GetRadarHead();

    mmWaveRadarFFT *FreqDomain;

    enum Endian //设置大端小端模式
    {
        BigEndian=0,
        LittleEndian=1
    };

    Endian RadarEndian;

signals:
    void GetFullFrame();
    void PushBackData(QVector<short>);
private:

    UnionParamter_t Parameter; //设定的雷达参数
    UnionData_t Data;//接收的雷达数据

    QVector<short> ReceiveBuffer;//接收缓冲区

    QThread *FFTThread;

    void ConvertEndian(int Length); //处理大小端问题
    bool isParameterLegal();//判断参数是否合法
};

#endif // MMWAVERADAR_H
