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
    void StartCamera(int number);
    void StopCamera();
    void StartRecording();
    void StopRecording();
    QImage cvMat2QImage(const Mat& mat);
signals:
    void RenewImage(QPixmap image);
private slots:
    void TimerSlot();
private:
    QTimer *RefreshTimer;
    int FPS;
    Mat CaptureBuffer;

};

#endif // UVCCAMERA_H
