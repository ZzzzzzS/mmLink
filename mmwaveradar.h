#ifndef MMWAVERADAR_H
#define MMWAVERADAR_H

#include <QObject>
#include <QByteArray>



class mmWaveRadar : public QObject
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

    explicit mmWaveRadar(QObject *parent = nullptr);
    QByteArray UpdateRadarParameter();
    bool isParameterLegal();
    void SetRadarParameter(RadarParameter_t value);

signals:

private:
    UnionParamter_t Parameter;
};

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // MMWAVERADAR_H
