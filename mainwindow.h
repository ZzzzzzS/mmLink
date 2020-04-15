#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QList>
#include <QUrl>

#include <QTcpSocket>
#include <QAbstractSocket>

#include "mmwaveradar.h"

#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QCameraViewfinderSettings>
#include <QCameraFocus>

#include <QMediaRecorder>
#include <QVideoEncoderSettings>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
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

    QCameraViewfinder *CameraView;
    QList<QCameraInfo> AvailableCameras;
    QCamera *CurrentCamera;
    QCameraFocus *CurrentCameraFocus;

    QMediaRecorder *CameraRecorder;

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
    void CameraErrorSlot(QCamera::Error value);
    void CameraZoomSlot(int value);
    void CameraRecordSlot();

    void UpdateParameterSlot();
};

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // MAINWINDOW_H
