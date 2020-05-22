#ifndef UVCCAMERA_H
#define UVCCAMERA_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/cudacodec.hpp>
#include <QTime>
#include <QPixmap>

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
    cv::VideoCapture *Capture;
    cv::VideoWriter *recorder;
    cv::Size CaptureSize;
    int FPS;
    cv::Mat CaptureBuffer;
    bool isCapturing;
    bool isRecording;
    void CameraLoop();
    QImage cvMat2QImage(const cv::Mat& mat);
};

#endif // UVCCAMERA_H
