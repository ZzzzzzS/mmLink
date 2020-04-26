#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QHostAddress>
#include <QMessageBox>
#include <QList>
#include <QUrl>
#include <QImage>
#include <QThread>
#include "aboutwindow.h"

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
    void SetLogo();
private:
    Ui::MainWindow *ui;
    mmWaveRadar *RadarSocket;

    UVCCamera *Camera;
    QThread CameraThread;

    QwtPlotShow *RadarTimePlot;
    QwtPlotShow *RadarFreqPlot;
    QwtPlotShow *RadarPhasePlot;

    QImage *logo;


    void SaveVideo();

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void CameraOperate(QString);

private slots:
    void TCPConnectSlot();
    void TCPErrorSlot(QAbstractSocket::SocketError socketError);
    void TCPConnectSuccessedSlot();
    void TCPDisconnectSuccessedSlot();
    void TCPReceiveSlot();

    void CameraConnectSlot();
    void CameraZoomSlot(int value);
    void CameraRecordSlot();
    void RenewImageSlot(QPixmap image);
    void CameraStartedSlot();
    void CameraStoppedSlot();
    void CameraErrorSlot();
    void CameraInputSlot(int oldPos, int newPos);

    void UpdateParameterSlot();
    void CleanCacheSlot();
    void AboutSlot();
};

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#endif // MAINWINDOW_H
