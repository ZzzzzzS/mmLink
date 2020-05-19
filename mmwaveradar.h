#ifndef MMWAVERADAR_H
#define MMWAVERADAR_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QVector>
#include <QVector2D>
#include <vector>
#include <opencv2/opencv.hpp>

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

    //计算的参数
    struct complex
    {
        float real;
        float imaginary;
    };
    std::vector<float> FFTMagnitude;//幅度数据
    std::vector<float> FFTPhase;//相位数据
    std::vector<float> TimeData;//时域数据


    mmWaveRadar(QObject *parent = nullptr);
    void UpdateRadarParameter();//更新参数
    bool isParameterLegal();//判断参数是否合法
    void ClearRadarCache();//清除缓存
    void SetRadarParameter(RadarParameter_t value);//设置雷达参数
    bool ReadRadarData();
    RadarHead_t GetRadarHead();
    void ProcessRadarData();//处理雷达数据(合并+fft)
    std::vector<float> GetFFTRealPart();//获得实部数据
    std::vector<float> GetFFTImaginaryPart();//获得虚部数据

signals:

private:
    bool RadarBufferCompress();//压缩合并多帧雷达数据
    void RadarFFT(std::vector<float> &TimeData,bool angleInDegrees=false);//计算fft

    UnionParamter_t Parameter; //设定的雷达参数
    UnionData_t Data;//接收的雷达数据

    std::vector<short> ReceiveBuffer;//接收缓冲区
    std::vector<std::vector<short>> AllReceivedData;//保存所有的时域数据
    std::vector<cv::Mat> FFTData;//原始fft数据(分离到两个Mat，实部0，虚部1)

};

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // MMWAVERADAR_H
