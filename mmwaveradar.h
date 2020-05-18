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
    std::vector<float> FFTMagnitude;
    std::vector<float> FFTPhase;


    mmWaveRadar(QObject *parent = nullptr);
    void UpdateRadarParameter();//更新参数
    bool isParameterLegal();//判断参数是否合法
    void ClearRadarCache();//清除缓存
    void SetRadarParameter(RadarParameter_t value);//设置雷达参数
    bool ReadRadarData();
    RadarHead_t GetRadarHead();
    bool RadarBufferProcess();
    void RadarFFT(std::vector<float> &TimeData,bool angleInDegrees=false);
    std::vector<float> GetFFTRealPart();
    std::vector<float> GetFFTImaginaryPart();

signals:

private:
    UnionParamter_t Parameter;
    UnionData_t Data;

    QVector<double> FrameDataBuffer;

    QVector<double> X;
    QVector<double> TimeData;

    QVector<double> FreqData;
    QVector<double> PhaseData;
    std::vector<cv::Mat> FFTData;

};

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // MMWAVERADAR_H
