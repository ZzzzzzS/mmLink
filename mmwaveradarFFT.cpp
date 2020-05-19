#include "mmwaveradar.h"

//计算fft
void mmWaveRadar::RadarFFT(std::vector<float> &TimeData, bool angleInDegrees)
{
    cv::Mat FFTDataMat;
    cv::Mat Magnitude;
    cv::dft(TimeData,FFTDataMat,cv::DFT_COMPLEX_OUTPUT);//计算fft
    cv::split(FFTDataMat,this->FFTData);//分离实部虚部
    cv::magnitude(this->FFTData[0],this->FFTData[1],Magnitude);//计算幅度
    cv::log(Magnitude,Magnitude);//转换为对数
    cv::addWeighted(Magnitude,8.685889638,Magnitude,0,0,Magnitude);
    //关于8.685889638的说明：opencv中log的底数是e，计算分贝的公式是20*log10(x)=20*((ln(x))/(ln(10)))=8.685889638*ln(x)
    this->FFTMagnitude=Magnitude.reshape(1,1);//Mat转vector
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
