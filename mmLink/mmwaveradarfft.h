#ifndef MMWAVERADARFFT_H
#define MMWAVERADARFFT_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QMutex>
#include <QMutexLocker>

class mmWaveRadarFFT : public QObject
{
    Q_OBJECT
public:
    explicit mmWaveRadarFFT(std::vector<double>& TimeData,QObject *parent = nullptr);
    std::vector<double> GetFFTRealPart();//获得实部数据
    std::vector<double> GetFFTImaginaryPart();//获得虚部数据
    void ClearCache();
    //计算的参数
    struct complex
    {
        double real;
        double imaginary;
    };
    std::vector<double> FFTMagnitude;//幅度数据
    std::vector<double> FFTPhase;//相位数据

signals:
    void FFTComplete();
public slots:
    void ProcessFFT();

private:
    void RadarFFT(std::vector<double> &TimeData, bool Log=true, bool angleInDegrees=false);//计算fft
    std::vector<double>& TimeData;//引用传入的时域数据
    std::vector<cv::Mat> FFTData;//原始fft数据(分离到两个Mat，实部0，虚部1)
    QMutex FFTlock;

};

#endif // MMWAVERADARFFT_H
