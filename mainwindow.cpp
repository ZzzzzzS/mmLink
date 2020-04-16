#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置雷达
    this->RadarSocket=new mmWaveRadar(this);
    //设置相机
    this->CameraView=new QCameraViewfinder(this->ui->CameraWidget);
    this->UpdateAvailableCamerasSlot();
    this->CurrentCamera=nullptr;
    this->ui->RecordButton->setEnabled(false);
    //设置Qwt
    this->RadarTimePlot=new QwtPlotShow(this->ui->RadarTimeData,this);
    this->RadarFreqPlot=new QwtPlotShow(this->ui->RadarFrequentData,this);
    this->RadarPhasePlot=new QwtPlotShow(this->ui->RadarPhaseData,this);

    //UI信号
    QObject::connect(this->ui->action_aboutQt,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
    //TCP连接相关信号
    QObject::connect(this->ui->RadarConnectButton,SIGNAL(clicked()),this,SLOT(TCPConnectSlot()));
    QObject::connect(this->RadarSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(TCPErrorSlot(QAbstractSocket::SocketError)));
    QObject::connect(this->RadarSocket,SIGNAL(connected()),this,SLOT(TCPConnectSuccessedSlot()));
    QObject::connect(this->RadarSocket,SIGNAL(disconnected()),this,SLOT(TCPDisconnectSuccessedSlot()));
    QObject::connect(this->RadarSocket,SIGNAL(readyRead()),this,SLOT(TCPReceiveSlot()));
    //Camera相关信号
    QObject::connect(this->ui->CameraConnectButton,SIGNAL(clicked()),this,SLOT(CameraConnectSlot()));
    QObject::connect(this->ui->Focurs,SIGNAL(sliderMoved(int)),this,SLOT(CameraZoomSlot(int)));
    QObject::connect(this->ui->CameraRenewButton,SIGNAL(clicked()),this,SLOT(UpdateAvailableCamerasSlot()));
    QObject::connect(this->ui->RecordButton,SIGNAL(clicked()),this,SLOT(CameraRecordSlot()));
    //雷达参数相关信号
    QObject::connect(this->ui->UpdateButton,SIGNAL(clicked()),this,SLOT(UpdateParameterSlot()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->CameraView->resize(this->ui->CameraWidget->size());
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


