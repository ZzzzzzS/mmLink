#ifndef MMWAVERADAR_H
#define MMWAVERADAR_H

#include <QObject>

class mmWaveRadar : public QObject
{
    Q_OBJECT
public:
    explicit mmWaveRadar(QObject *parent = nullptr);



signals:


private:
    uint SampleRate;
    uint SamplePoint;
    uint ChirpNumber;
    uint Slope;

};

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // MMWAVERADAR_H
