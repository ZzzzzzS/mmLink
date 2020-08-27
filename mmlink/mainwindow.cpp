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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置UI
    this->ui->CurrentChirp->setReadOnly(true);
    this->ui->CurrentSampleRate->setReadOnly(true);
    this->ui->CurrentFrameNumber->setReadOnly(true);
    this->ui->CurrentSamplePoint->setReadOnly(true);
    this->ui->ClinetIP->setReadOnly(true);
    this->ui->ClinetPort->setReadOnly(true);
    this->ui->ClinetIP->setText(tr("N/A"));
    this->ui->ClinetPort->setText(tr("N/A"));
    this->logo=new QImage("://image/hitlogo.png");
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
    this->SetLogo();
    //设置雷达
    this->RadarSocket=new mmWaveRadar(this);
    this->RadarData=new DataManagement();
    this->RadarDataThread=new QThread();//雷达保存数据库在另一个线程里
    qRegisterMetaType<QVector<short>>("QVector_short");//注册类型，信号槽传递未知参数类型需要
    this->RadarData->moveToThread(this->RadarDataThread);
    this->RadarDataThread->start();
    QDir dir;//创建一个Data目录，用来保证后面创建文件不出错
    if(!dir.exists(qApp->applicationDirPath()+"/Data"))
    {
        dir.mkdir(qApp->applicationDirPath()+"/Data");
    }
    this->GetRadarEndian();
    //设置相机
    this->CameraThread=new QThread();
    this->Camera=new UVCCamera();
    this->Camera->moveToThread(this->CameraThread);
    this->ui->RecordButton->setEnabled(false);

    //设置Qwt
    this->RadarTimePlot=new QwtPlotShow(this->ui->RadarTimeData,this);
    this->RadarFreqPlot=new QwtPlotShow(this->ui->RadarFrequentData,this);
    this->RadarPhasePlot=new QwtPlotShow(this->ui->RadarPhaseData,this);

    //UI信号
    QObject::connect(this->ui->action_aboutQt,SIGNAL(triggered()),this,SLOT(AboutSlot()));
    QObject::connect(this->ui->DataFolderButton,SIGNAL(clicked()),this,SLOT(OpenFolderSlot()));
    QObject::connect(this->ui->CameraAddress,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(CameraInputSlot(int,int)));
    QObject::connect(this->ui->EndianButton,SIGNAL(clicked()),this,SLOT(SetEndianSlot()));
    //TCP连接相关信号
    QObject::connect(this->ui->RadarConnectButton,SIGNAL(clicked()),this,SLOT(TCPConnectSlot()));
    QObject::connect(this->RadarSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(TCPErrorSlot(QAbstractSocket::SocketError)));
    QObject::connect(this->RadarSocket,SIGNAL(connected()),this,SLOT(TCPConnectSuccessedSlot()));
    QObject::connect(this->RadarSocket,SIGNAL(disconnected()),this,SLOT(TCPDisconnectSuccessedSlot()));
    QObject::connect(this->RadarSocket,SIGNAL(readyRead()),this,SLOT(TCPReceiveSlot()));
    //Camera相关信号
    QObject::connect(this->ui->CameraConnectButton,SIGNAL(clicked()),this,SLOT(CameraConnectSlot()));
    QObject::connect(this->ui->RecordButton,SIGNAL(clicked()),this,SLOT(CameraRecordSlot()));
    QObject::connect(this->Camera,SIGNAL(RenewImage(QPixmap)),this,SLOT(RenewImageSlot(QPixmap)));
    QObject::connect(this,SIGNAL(CameraInit(QString)),this->Camera,SLOT(StartCamera(QString)));
    QObject::connect(this->Camera,SIGNAL(CameraStarted()),this,SLOT(CameraStartedSlot()));
    QObject::connect(this->Camera,SIGNAL(CameraStopped()),this,SLOT(CameraStoppedSlot()));
    QObject::connect(this->Camera,SIGNAL(CameraStartFailed()),this,SLOT(CameraErrorSlot()));
    //雷达参数相关信号
    QObject::connect(this->ui->ConvertFrameButton,SIGNAL(clicked()),this,SLOT(ConvertFrameSlot()));
    QObject::connect(this->ui->UpdateButton,SIGNAL(clicked()),this,SLOT(UpdateParameterSlot()));
    QObject::connect(this->RadarSocket->FreqDomain,SIGNAL(FFTComplete()),this,SLOT(RenewRadarDataSlot()));
    QObject::connect(this->RadarSocket,SIGNAL(PushBackData(QVector<short>)),this->RadarData,SLOT(SaveData(QVector<short>)));
}

MainWindow::~MainWindow()
{
    this->CameraThread->terminate();
    delete ui;
}

//窗体变化的时候让视频显示自动符合大小，这个函数的实现方法很烂，也暂时没找到更好的实现窗口大小变化的方法
void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
    if(this->ui->CameraConnectButton->text()!=tr("Disconnect"))
        this->SetLogo();
}

//设置视频显示界面那个hit的logo
void MainWindow::SetLogo()
{
    QPixmap value=QPixmap::fromImage(*(this->logo));
    this->ui->CameraView->setPixmap(value.scaled(this->ui->RadarPhaseData->size(),Qt::KeepAspectRatio));

}

//打开关于界面
void MainWindow::AboutSlot()
{
    AboutWindow *WindowBase=new AboutWindow(this);
    WindowBase->exec();
    delete WindowBase;
}

//这个可要可不要的功能，很无聊的一个函数
void MainWindow::CameraInputSlot(int oldPos, int newPos)
{
    if(this->ui->CameraAddress->text()==tr("Enter camera number or video stream address"))
    {
        this->ui->CameraAddress->clear();
        return;
    }
    else if(this->ui->CameraAddress->text()==tr("Enter camera number or video stream addres"))
    {
        this->ui->CameraAddress->clear();
        return;
    }
}

void MainWindow::OpenFolderSlot()
{
    if(!QDesktopServices::openUrl(QUrl::fromLocalFile(qApp->applicationDirPath()+"/Data")))
    {
        QMessageBox::critical(this,tr("Error"),tr("Fail to Open Folder"),QMessageBox::Ok);
    }
}
