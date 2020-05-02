#ifndef UVCCAMERA_H
#define UVCCAMERA_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/cudacodec.hpp>
#include <QTime>
#include <QPixmap>

using namespace cv;

class UVCCamera : public QObject
{
    Q_OBJECT
public:
    explicit UVCCamera(QObject *parent = nullptr);
    ~UVCCamera();
    VideoCapture *Capture;
    VideoWriter *recorder;
    Size CaptureSize;

#if defined (USE_CUDA) && defined (Q_OS_WINDOWS)
public:
    Ptr<cudacodec::VideoWriter> CUDARecorder;
private:
    bool isCUDASupport;
#endif

public:
    void StopCamera();
    void StartRecording();
    void StopRecording();
    QImage cvMat2QImage(const Mat& mat);
signals:
    void RenewImage(QPixmap image);
    void CameraStarted();
    void CameraStopped();
    void CameraStartFailed();
public slots:
    void StartCamera(QString);
private slots:
private:
    int FPS;
    Mat CaptureBuffer;
    bool isCapturing;
    bool StopCapture;
    void TimerSlot();

};

#endif // UVCCAMERA_H
