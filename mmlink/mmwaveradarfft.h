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
