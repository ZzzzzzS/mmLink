#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>
#include <QDesktopServices>
#include <QMediaPlayer>
#include <opencv2/opencv.hpp>
#include <opencv2/core/cuda.hpp>

namespace Ui {
class AboutWindow;
}

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = 0);
    ~AboutWindow();

private slots:
    void OpenCVSlot();
    void FeedBackSlot();
    void OpenSourceSlot();
#ifdef USE_CUDA
    void CUDASlot();
#endif
private:
    Ui::AboutWindow *ui;
    QMediaPlayer *Music;
    void SetLogo();
    void SetInformation();
#ifdef USE_CUDA
    void SetCUDAInformation();
    bool isCUDASupport;
    cv::Ptr<cv::cuda::DeviceInfo> CUDADevice;
#endif
};


#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // ABOUTWINDOW_H
