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
#include <QTranslator>
#include <QMetaType>
#include <QVector>
#include <QDateTime>
#include <QDir>
#include "aboutwindow.h"
#include <QDesktopServices>

#include "mmwaveradar.h"
#include "datamanagement.h"

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
    DataManagement *RadarData;
    QThread *RadarDataThread;

    UVCCamera *Camera;
    QThread *CameraThread;

    QwtPlotShow *RadarTimePlot;
    QwtPlotShow *RadarFreqPlot;
    QwtPlotShow *RadarPhasePlot;

    QImage *logo;

    void GetRadarEndian();

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void CameraInit(QString);

private slots:
    void TCPConnectSlot();
    void TCPErrorSlot(QAbstractSocket::SocketError socketError);
    void TCPConnectSuccessedSlot();
    void TCPDisconnectSuccessedSlot();
    void TCPReceiveSlot();
    void RenewRadarDataSlot();
    void ConvertFrameSlot();
    void SetEndianSlot();

    void CameraConnectSlot();
    void CameraRecordSlot();
    void RenewImageSlot(QPixmap image);
    void CameraStartedSlot();
    void CameraStoppedSlot();
    void CameraErrorSlot();
    void CameraInputSlot(int oldPos, int newPos);

    void UpdateParameterSlot();

    void OpenFolderSlot();
    void AboutSlot();
};

#endif // MAINWINDOW_H
