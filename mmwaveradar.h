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

#endif // MMWAVERADAR_H
