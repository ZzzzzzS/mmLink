#include "uvccamera.h"

UVCCamera::UVCCamera(QObject *parent) : QObject(parent)
{
    this->RefreshTimer=new QTimer(this);
    this->recorder=new VideoWriter();
    QObject::connect(this->RefreshTimer,SIGNAL(timeout()),this,SLOT(TimerSlot()));
}

UVCCamera::~UVCCamera()
{

}

void UVCCamera::StartCamera(int number)
{
    this->Capture=new VideoCapture(number);
    this->FPS=this->Capture->get(CAP_PROP_FPS);
    this->RefreshTimer->start(1000/this->FPS);
}

void UVCCamera::StopCamera()
{
    this->RefreshTimer->stop();
    this->Capture->release();
}

void UVCCamera::StartRecording()
{
    //this->Capture->get()
    this->CaptureSize.width=this->Capture->get(cv::CAP_PROP_FRAME_WIDTH);
    this->CaptureSize.height=this->Capture->get(cv::CAP_PROP_FRAME_HEIGHT);
    this->recorder->open("CameraCapture.avi",VideoWriter::fourcc('D', 'I', 'V', 'X'),this->FPS,this->CaptureSize);
    this->isCapturing=true;
}

void UVCCamera::StopRecording()
{
    this->isCapturing=false;
    this->recorder->release();
}

void UVCCamera::TimerSlot()
{
    this->Capture->read(this->CaptureBuffer);
    QImage img = this->cvMat2QImage(this->CaptureBuffer);
    QPixmap pix=QPixmap::fromImage(img);
    if(this->isCapturing)
    {
        this->recorder->write(this->CaptureBuffer);
    }
    emit this->RenewImage(pix);
}

QImage UVCCamera::cvMat2QImage(const Mat &mat)
{
    if (mat.type() == CV_8UC1)					// 单通道
        {
            QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
            image.setColorCount(256);				// 灰度级数256
            for (int i = 0; i < 256; i++)
            {
                image.setColor(i, qRgb(i, i, i));
            }
            uchar *pSrc = mat.data;					// 复制mat数据
            for (int row = 0; row < mat.rows; row++)
            {
                uchar *pDest = image.scanLine(row);
                memcpy(pDest, pSrc, mat.cols);
                pSrc += mat.step;
            }
            return image;
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


