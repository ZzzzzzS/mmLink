#include "uvccamera.h"
#include <QApplication>
#include <time.h>
#include <opencv2/core/cuda.hpp>

UVCCamera::UVCCamera(QObject *parent) : QObject(parent)
{
    this->recorder=new VideoWriter();
    this->Capture=new VideoCapture();

#if defined (USE_CUDA) && defined (Q_OS_WINDOWS)
    cuda::DeviceInfo info;
    this->isCUDASupport=info.isCompatible();
#endif
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
    this->StopCapture=false;
    this->isCapturing=false;
    qDebug("ok");
    emit CameraStarted();
    TimerSlot();
}

void UVCCamera::StopCamera()
{
    this->StopCapture=true;
}

void UVCCamera::StartRecording()
{
#if defined (USE_CUDA) && defined (Q_OS_WINDOWS)
    if(!isCUDASupport)
    {
        this->CaptureSize.width=this->Capture->get(cv::CAP_PROP_FRAME_WIDTH);
        this->CaptureSize.height=this->Capture->get(cv::CAP_PROP_FRAME_HEIGHT);
        this->recorder->open("CameraCapture.avi",VideoWriter::fourcc('D', 'I', 'V', 'X'),this->FPS,this->CaptureSize);
        this->isCapturing=true;
    }
    else
    {
        this->CaptureSize.width=this->Capture->get(cv::CAP_PROP_FRAME_WIDTH);
        this->CaptureSize.height=this->Capture->get(cv::CAP_PROP_FRAME_HEIGHT);
        const cv::String name="CameraCapture.avi";
        this->CUDARecorder=cudacodec::createVideoWriter(name,this->CaptureSize,this->FPS);
        this->isCapturing=true;
    }
#else
    this->CaptureSize.width=this->Capture->get(cv::CAP_PROP_FRAME_WIDTH);
    this->CaptureSize.height=this->Capture->get(cv::CAP_PROP_FRAME_HEIGHT);
    this->recorder->open("CameraCapture.avi",VideoWriter::fourcc('D', 'I', 'V', 'X'),this->FPS,this->CaptureSize);
    this->isCapturing=true;
#endif
}

void UVCCamera::StopRecording()
{
    this->isCapturing=false;
    this->recorder->release();
#if defined (USE_CUDA) && defined (Q_OS_WINDOWS)
    if(isCUDASupport)
        this->CUDARecorder.release();
#endif
}

void UVCCamera::TimerSlot()
{
    //time_t start,end;
    //int t=0;
    while(!StopCapture)
    {
        //start=clock();
        this->Capture->read(this->CaptureBuffer);
        QImage img = this->cvMat2QImage(this->CaptureBuffer);
        QPixmap pix=QPixmap::fromImage(img);
        if(this->isCapturing)
        {
#if defined (USE_CUDA) && defined (Q_OS_WINDOWS)
            if(isCUDASupport)
            {
                this->CUDARecorder->write(this->CaptureBuffer);
            }
            else
            {
                this->recorder->write(this->CaptureBuffer);
            }
#else
            this->recorder->write(this->CaptureBuffer);
#endif
        }
        emit this->RenewImage(pix);
        //end=clock();
        //t=end-start;
        //cv::waitKey(1000/FPS-t);
    }
    if(isCapturing)
    {
        StopRecording();
    }
    this->Capture->release();
    emit CameraStopped();
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


