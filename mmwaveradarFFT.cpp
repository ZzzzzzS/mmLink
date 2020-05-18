#include "mmwaveradar.h"

//计算fft
void mmWaveRadar::RadarFFT(std::vector<float> &TimeData, bool angleInDegrees)
{
    cv::Mat FFTDataMat;
    cv::dft(TimeData,FFTDataMat,cv::DFT_COMPLEX_OUTPUT);//计算fft
    cv::split(FFTDataMat,this->FFTData);//分离实部虚部
    cv::magnitude(this->FFTData[0],this->FFTData[1],this->FFTMagnitude);//计算幅度
    cv::phase(this->FFTData[0],this->FFTData[1],this->FFTPhase,angleInDegrees);//计算相位
}

std::vector<float> mmWaveRadar::GetFFTRealPart()
{
    return (std::vector<float>)(this->FFTData[0].reshape(1,1));//输出实部
}

std::vector<float> mmWaveRadar::GetFFTImaginaryPart()
{
    return (std::vector<float>)(this->FFTData[1].reshape(1,1));//输出虚部
}
