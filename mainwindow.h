#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QList>
#include <QUrl>
#include <QImage>

#include "mmwaveradar.h"

#include "uvccamera.h"

#include "qwtplotshow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace cv;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    mmWaveRadar *RadarSocket;

    UVCCamera *Camera;

    QwtPlotShow *RadarTimePlot;
    QwtPlotShow *RadarFreqPlot;
    QwtPlotShow *RadarPhasePlot;

    QImage *logo;
    void SetLogo();

    void SaveVideo();

protected:
    void resizeEvent(QResizeEvent *event);


private slots:
    void TCPConnectSlot();
    void TCPErrorSlot(QAbstractSocket::SocketError socketError);
    void TCPConnectSuccessedSlot();
    void TCPDisconnectSuccessedSlot();
    void TCPReceiveSlot();

    void UpdateAvailableCamerasSlot();
    void CameraConnectSlot();
    void CameraZoomSlot(int value);
    void CameraRecordSlot();
    void RenewImageSlot(QPixmap image);

    void UpdateParameterSlot();
    void CleanCacheSlot();
};

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // MAINWINDOW_H
