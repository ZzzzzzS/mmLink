/****************************************************************************
MIT License

Copyright (c) 2020 ZhouZishun

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*****************************************************************************/

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
