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
    void StopCamera();
    void StartRecording();
    void StopRecording();

signals:
    void RenewImage(QPixmap image);
    void CameraStarted();
    void CameraStopped();
    void CameraStartFailed();
public slots:
    void StartCamera(QString);

private:
    VideoCapture *Capture;
    VideoWriter *recorder;
    Size CaptureSize;
    int FPS;
    Mat CaptureBuffer;
    bool isCapturing;
    bool isRecording;
    void CameraLoop();
    QImage cvMat2QImage(const Mat& mat);
};

#endif // UVCCAMERA_H
