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
    this->ui->ClinetIP->setText("连接后显示");
    this->ui->ClinetPort->setText("不可用");
    this->logo=new QImage("hitlogo.png");
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
    this->SetLogo();
    //设置雷达
    this->RadarSocket=new mmWaveRadar(this);
    //设置相机
    this->Camera=new UVCCamera();
    this->Camera->moveToThread(&CameraThread);
    //设置Qwt
    this->RadarTimePlot=new QwtPlotShow(this->ui->RadarTimeData,this);
    this->RadarFreqPlot=new QwtPlotShow(this->ui->RadarFrequentData,this);
    this->RadarPhasePlot=new QwtPlotShow(this->ui->RadarPhaseData,this);

    //UI信号
    QObject::connect(this->ui->action_aboutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    QObject::connect(this->ui->CleanCacheButton,SIGNAL(clicked()),this,SLOT(CleanCacheSlot()));
    QObject::connect(this->ui->CameraAddress,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(CameraInputSlot(int,int)));
    //TCP连接相关信号
    QObject::connect(this->ui->RadarConnectButton,SIGNAL(clicked()),this,SLOT(TCPConnectSlot()));
    QObject::connect(this->RadarSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(TCPErrorSlot(QAbstractSocket::SocketError)));
    QObject::connect(this->RadarSocket,SIGNAL(connected()),this,SLOT(TCPConnectSuccessedSlot()));
    QObject::connect(this->RadarSocket,SIGNAL(disconnected()),this,SLOT(TCPDisconnectSuccessedSlot()));
    QObject::connect(this->RadarSocket,SIGNAL(readyRead()),this,SLOT(TCPReceiveSlot()));
    //Camera相关信号
    QObject::connect(this->ui->CameraConnectButton,SIGNAL(clicked()),this,SLOT(CameraConnectSlot()));
    QObject::connect(this->ui->Focurs,SIGNAL(sliderMoved(int)),this,SLOT(CameraZoomSlot(int)));
    QObject::connect(this->ui->RecordButton,SIGNAL(clicked()),this,SLOT(CameraRecordSlot()));
    QObject::connect(this->Camera,SIGNAL(RenewImage(QPixmap)),this,SLOT(RenewImageSlot(QPixmap)));
    QObject::connect(this,SIGNAL(CameraOperate(QString)),this->Camera,SLOT(StartCamera(QString)));
    QObject::connect(this->Camera,SIGNAL(CameraStarted()),this,SLOT(CameraStartedSlot()));
    QObject::connect(this->Camera,SIGNAL(CameraStopped()),this,SLOT(CameraStoppedSlot()));
    QObject::connect(this->Camera,SIGNAL(CameraStartFailed()),this,SLOT(CameraErrorSlot()));
    //雷达参数相关信号
    QObject::connect(this->ui->UpdateButton,SIGNAL(clicked()),this,SLOT(UpdateParameterSlot()));

}

MainWindow::~MainWindow()
{
    this->CameraThread.terminate();
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
}

void MainWindow::SetLogo()
{
    QPixmap value=QPixmap::fromImage(*(this->logo));
    this->ui->CameraView->setPixmap(value.scaled(this->ui->RadarPhaseData->size(),Qt::KeepAspectRatio));

}

void MainWindow::CameraInputSlot(int oldPos, int newPos)
{
    if(this->ui->CameraAddress->text()=="输入摄像头编号或视频流地址")
    {
        this->ui->CameraAddress->clear();
        return;
    }
    else if(this->ui->CameraAddress->text()=="输入摄像头编号或视频流地")
    {
        this->ui->CameraAddress->clear();
        return;
    }
}

void MainWindow::UpdateParameterSlot()
{
    mmWaveRadar::RadarParameter_t value;

    value.SampleRate=this->ui->SampleRate->text().toInt();
    value.SamplePoint=this->ui->SamplePoint->text().toInt();
    value.ChirpNumber=this->ui->ChirpNumber->text().toInt();
    value.Slope=this->ui->Slope->text().toInt();
    this->RadarSocket->SetRadarParameter(value);

    if(this->RadarSocket->isParameterLegal())
        this->RadarSocket->write(this->RadarSocket->UpdateRadarParameter());
    else
        QMessageBox::warning(this,"雷达参数错误","请填写正确的雷达参数");
}

void MainWindow::CleanCacheSlot()
{
    qDebug()<<"clear cache";
    this->RadarSocket->ClearRadarCache();
    this->RadarFreqPlot->ClearSlot();
    this->RadarTimePlot->ClearSlot();
    this->RadarPhasePlot->ClearSlot();
}


void MainWindow::CameraConnectSlot()
{
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
    if(this->ui->CameraConnectButton->text()=="连接摄像头")
    {
        if(this->ui->CameraAddress->text()=="输入摄像头编号或视频流地址")
        {
            QMessageBox::warning(this,"将使用默认摄像头","未指定输入设备,将使用默认设备");
            this->ui->CameraAddress->setText("0");
        }
        this->CameraThread.start();
        emit CameraOperate(this->ui->CameraAddress->text());
        this->ui->CameraConnectButton->setText("正在连接");
        this->ui->CameraConnectButton->setEnabled(false);
    }
    else if(this->ui->CameraConnectButton->text()=="断开")
    {
        this->Camera->StopCamera();
        this->ui->CameraConnectButton->setText("连接摄像头");
        this->ui->CameraConnectButton->setText("正在断开");
        this->ui->CameraConnectButton->setEnabled(false);
    }
}

void MainWindow::CameraStartedSlot()
{
    this->ui->CameraConnectButton->setText("断开");
    this->ui->CameraConnectButton->setEnabled(true);
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
    this->ui->RecordButton->setEnabled(true);
    this->ui->RecordButton->setText("录制视频");
}

void MainWindow::CameraStoppedSlot()
{
    this->ui->CameraConnectButton->setText("连接摄像头");
    this->ui->CameraConnectButton->setEnabled(true);
    this->SetLogo();
    this->ui->RecordButton->setEnabled(false);
    this->ui->RecordButton->setText("录制视频");
}

void MainWindow::CameraErrorSlot()
{
    this->ui->CameraConnectButton->setText("连接摄像头");
    this->ui->CameraConnectButton->setEnabled(true);
    QMessageBox::critical(this,"摄像头打开失败","请检查摄像头连接或视频流地址是否正确");
}

void MainWindow::CameraZoomSlot(int value)
{

}

void MainWindow::CameraRecordSlot()
{
    if(this->ui->RecordButton->text()=="录制视频")
    {
        this->Camera->StartRecording();
        this->ui->RecordButton->setText("停止录制");
    }
    else if(this->ui->RecordButton->text()=="停止录制")
    {
        this->Camera->StopRecording();
        this->ui->RecordButton->setText("录制视频");
    }
}

void MainWindow::RenewImageSlot(QPixmap image)
{
    this->ui->CameraView->setPixmap(image.scaled(this->ui->CameraView->size(),Qt::KeepAspectRatio));
}
