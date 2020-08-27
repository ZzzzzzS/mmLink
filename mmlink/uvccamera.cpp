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

#include "uvccamera.h"
#include <QApplication>
#include <time.h>
#include <QThread>

UVCCamera::UVCCamera(QObject *parent) : QObject(parent)
{
    this->recorder=new cv::VideoWriter(); //初始化视频编码器
    this->Capture=new cv::VideoCapture(); //初始化视频采集器
}

UVCCamera::~UVCCamera()
{

}

void UVCCamera::StartCamera(QString name)
{
    bool ok;
    int number=name.toInt(&ok);
    if(ok)
    {
        ok=this->Capture->open(number);
    }
    else
    {
        ok=this->Capture->open(name.toStdString());
    }
    if(!ok)
    {
        emit CameraStartFailed();
        return;
    }
    this->FPS=this->Capture->get(cv::CAP_PROP_FPS);
    this->isRecording=false; //设置没有开始录制视频
    this->isCapturing=true; //设置开始采集的标志位
    qDebug("Camera start");
    emit CameraStarted();
    CameraLoop();
}

void UVCCamera::StopCamera()
{
    this->isCapturing=false;
}

void UVCCamera::StartRecording(QString dir)
{
    this->CaptureSize.width=this->Capture->get(cv::CAP_PROP_FRAME_WIDTH);
    this->CaptureSize.height=this->Capture->get(cv::CAP_PROP_FRAME_HEIGHT);
    this->recorder->open(dir.toStdString(),cv::VideoWriter::fourcc('D', 'I', 'V', 'X'),this->FPS,this->CaptureSize);
    this->isRecording=true;
}

void UVCCamera::StopRecording()
{
    this->isRecording=false;
    this->recorder->release();
}


//摄像头采集和视频录制就在这个死循环里进行，没出现图像撕裂的情况，就没有加锁保护
//理论上来说应该需要采集一帧后延时，延时=1/fps-(采集需要的时间)，这样才能保证采集到的视频的时间长度的正确性
//但是事实上这样效果并不好，我也不太明白为什么，反正不延时效果最好了，但是长时间录制视频还是时间会差几秒
//猜测原因是摄像头实际曝光所用的时间和fps不相同，所以导致误差，这现象在光线较暗的情况下非常明显，整个视频都比较卡
void UVCCamera::CameraLoop()
{
    clock_t start=clock();
    clock_t stop;
    QImage img;
    QPixmap pix;
    qDebug("%d",FPS);
    while(isCapturing)//一直在循环里采集图像
    {
        start=clock();//计算处理图像用时
        this->Capture->read(this->CaptureBuffer);
        img = this->cvMat2QImage(this->CaptureBuffer);
        pix = QPixmap::fromImage(img);
        if(this->isRecording)
        {
            this->recorder->write(this->CaptureBuffer);
        }
        emit this->RenewImage(pix);
        stop=clock();//计算处理图像用时
        //qDebug("%d",1000/FPS-stop+start);
        //QThread::msleep(1000/FPS-stop+start);
        //qDebug("%d",stop-start);
    }

    if(isRecording)//检查是否还在录像
    {
        StopRecording();
    }
    this->Capture->release();//释放摄像头
    emit CameraStopped(); //摄像头已停止信号
}

QImage UVCCamera::cvMat2QImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC1)					// 单通道
        {
            QImage img((const unsigned char *)(mat.data), mat.cols, mat.rows, mat.cols, QImage::Format_Grayscale8);
            return img;
        }

        else if (mat.type() == CV_8UC3)				// 3通道
        {
            const uchar *pSrc = (const uchar*)mat.data;			// 复制像素
            QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);	// R, G, B 对应 0,1,2
            return image.rgbSwapped();				// rgbSwapped是为了显示效果色彩好一些。
        }
        else if (mat.type() == CV_8UC4)
        {
            const uchar *pSrc = (const uchar*)mat.data;			// 复制像素
                                                                // Create QImage with same dimensions as input Mat
            QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);		// B,G,R,A 对应 0,1,2,3
            return image.copy();
        }
        else
        {
            return QImage();
        }

}
