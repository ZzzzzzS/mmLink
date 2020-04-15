#ifndef MMWAVERADAR_H
#define MMWAVERADAR_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#define MAX_TCPBuffer 65535


class mmWaveRadar : public QTcpSocket
{
    Q_OBJECT
public:

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
        char RadarPayload[MAX_TCPBuffer-20];
    }RadarData_t;

    typedef union
    {
        RadarData_t RadarData;
        char ReceiveBuffer[MAX_TCPBuffer];
    }UnionData_t;



    mmWaveRadar(QObject *parent = nullptr);
    QByteArray UpdateRadarParameter();
    bool isParameterLegal();
    void SetRadarParameter(RadarParameter_t value);
    bool ReadRadarData();
    RadarHead_t GetRadarHead();

signals:

private:
    UnionParamter_t Parameter;
    UnionData_t Data;
};

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // MMWAVERADAR_H
