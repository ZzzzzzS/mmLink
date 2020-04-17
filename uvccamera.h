#ifndef UVCCAMERA_H
#define UVCCAMERA_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QTimer>
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
    cv::Size CaptureSize;

    void StopCamera();
    void StartRecording();
    void StopRecording();
    QImage cvMat2QImage(const Mat& mat);
signals:
    void RenewImage(QPixmap image);
public slots:
    void StartCamera();
private slots:
private:
    QTimer *RefreshTimer;
    int FPS;
    Mat CaptureBuffer;
    bool isCapturing;
    bool StopCapture;
    void TimerSlot();

};

#endif // UVCCAMERA_H
