#include "uvccamera.h"
#include <QApplication>
#include <time.h>
#include <opencv2/core/cuda.hpp>

UVCCamera::UVCCamera(QObject *parent) : QObject(parent)
{
    this->recorder=new VideoWriter(); //初始化视频编码器
    this->Capture=new VideoCapture(); //初始化视频采集器
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
    this->FPS=this->Capture->get(CAP_PROP_FPS);
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

void UVCCamera::StartRecording()
{
    this->CaptureSize.width=this->Capture->get(cv::CAP_PROP_FRAME_WIDTH);
    this->CaptureSize.height=this->Capture->get(cv::CAP_PROP_FRAME_HEIGHT);
    this->recorder->open("CameraCapture.avi",VideoWriter::fourcc('D', 'I', 'V', 'X'),this->FPS,this->CaptureSize);
    this->isRecording=true;
}

void UVCCamera::StopRecording()
{
    this->isRecording=false;
    this->recorder->release();
}

void UVCCamera::CameraLoop()
{
    //time_t start,end;
    //int t=0;
    QImage img;
    QPixmap pix;
    while(isCapturing)//一直在循环里采集图像
    {
        //start=clock();
        this->Capture->read(this->CaptureBuffer);
        img = this->cvMat2QImage(this->CaptureBuffer);
        pix = QPixmap::fromImage(img);
        if(this->isRecording)
        {
            this->recorder->write(this->CaptureBuffer);
        }
        emit this->RenewImage(pix);
        //end=clock();
        //t=end-start;
        //cv::waitKey(1000/FPS-t);
    }

    if(isRecording)//检查是否还在录像
    {
        StopRecording();
    }
    this->Capture->release();//释放摄像头
    emit CameraStopped(); //摄像头已停止信号
}

QImage UVCCamera::cvMat2QImage(const Mat &mat)
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
