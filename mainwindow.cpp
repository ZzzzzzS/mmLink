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
    //设置相机
    this->CameraThread=new QThread();
    this->Camera=new UVCCamera();
    this->Camera->moveToThread(CameraThread);
    this->ui->RecordButton->setEnabled(false);

    //设置Qwt
    this->RadarTimePlot=new QwtPlotShow(this->ui->RadarTimeData,this);
    this->RadarFreqPlot=new QwtPlotShow(this->ui->RadarFrequentData,this);
    this->RadarPhasePlot=new QwtPlotShow(this->ui->RadarPhaseData,this);

    //UI信号
    QObject::connect(this->ui->action_aboutQt,SIGNAL(triggered()),this,SLOT(AboutSlot()));
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
    QObject::connect(this->ui->RecordButton,SIGNAL(clicked()),this,SLOT(CameraRecordSlot()));
    QObject::connect(this->Camera,SIGNAL(RenewImage(QPixmap)),this,SLOT(RenewImageSlot(QPixmap)));
    QObject::connect(this,SIGNAL(CameraInit(QString)),this->Camera,SLOT(StartCamera(QString)));
    QObject::connect(this->Camera,SIGNAL(CameraStarted()),this,SLOT(CameraStartedSlot()));
    QObject::connect(this->Camera,SIGNAL(CameraStopped()),this,SLOT(CameraStoppedSlot()));
    QObject::connect(this->Camera,SIGNAL(CameraStartFailed()),this,SLOT(CameraErrorSlot()));
    //雷达参数相关信号
    QObject::connect(this->ui->UpdateButton,SIGNAL(clicked()),this,SLOT(UpdateParameterSlot()));
    QObject::connect(this->RadarSocket->FreqDomain,SIGNAL(FFTComplete()),this,SLOT(RenewRadarDataSlot()));
}

MainWindow::~MainWindow()
{
    this->CameraThread->terminate();
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
    if(this->ui->CameraConnectButton->text()!=tr("Disconnect"))
        this->SetLogo();
}

void MainWindow::SetLogo()
{
    QPixmap value=QPixmap::fromImage(*(this->logo));
    this->ui->CameraView->setPixmap(value.scaled(this->ui->RadarPhaseData->size(),Qt::KeepAspectRatio));

}

void MainWindow::AboutSlot()
{
    AboutWindow *WindowBase=new AboutWindow(this);
    WindowBase->exec();
    delete WindowBase;
}

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
