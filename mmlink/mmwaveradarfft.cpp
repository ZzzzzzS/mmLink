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

#include "mmwaveradarfft.h"

mmWaveRadarFFT::mmWaveRadarFFT(std::vector<double> &TimeData, QObject *parent) : QObject(parent),TimeData(TimeData)
{

}

void mmWaveRadarFFT::ProcessFFT()
{
    QMutexLocker locker(&FFTlock);//加锁保护
    this->RadarFFT(this->TimeData,true,false);
}

void mmWaveRadarFFT::RadarFFT(std::vector<double> &TimeData,bool Log, bool angleInDegrees)
{
    cv::Mat FFTDataMat;
    cv::Mat Magnitude;
    cv::dft(TimeData,FFTDataMat,cv::DFT_COMPLEX_OUTPUT);//计算fft
    cv::split(FFTDataMat,this->FFTData);//分离实部虚部
    cv::magnitude(this->FFTData[0],this->FFTData[1],Magnitude);//计算幅度
    if(Log==true)
    {
        double* ptr=Magnitude.ptr<double>();
        for(int i=0;i<Magnitude.cols;i++)//防止计算对数时出现错误
        {
            if(*(ptr+i)==0)
            {
                *(ptr+i)=0.000001;
            }
        }
        cv::log(Magnitude,Magnitude);//转换为对数
        cv::addWeighted(Magnitude,8.685889638,Magnitude,0,0,Magnitude);
        //关于8.685889638的说明：opencv中log的底数是e，计算分贝的公式是20*log10(x)=20*((ln(x))/(ln(10)))=8.685889638*ln(x)
    }
    this->FFTMagnitude=Magnitude.reshape(1,1);//Mat转vector
    cv::phase(this->FFTData[0],this->FFTData[1],this->FFTPhase,angleInDegrees);//计算相位
    emit(this->FFTComplete());
}

std::vector<double> mmWaveRadarFFT::GetFFTRealPart()
{
    return (std::vector<double>)(this->FFTData[0].reshape(1,1));//输出实部
}

std::vector<double> mmWaveRadarFFT::GetFFTImaginaryPart()
{
    return (std::vector<double>)(this->FFTData[1].reshape(1,1));//输出虚部
}


void mmWaveRadarFFT::ClearCache()
{
    this->FFTData.clear();
    this->FFTPhase.clear();
    this->FFTMagnitude.clear();
}
