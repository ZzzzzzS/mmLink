﻿#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->RadarSocket=new QTcpSocket(this);

    this->CameraView=new QCameraViewfinder(this->ui->CameraWidget);
    this->UpdateAvailableCamerasSlot();
    this->CurrentCamera=nullptr;
    this->ui->RecordButton->setEnabled(false);



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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    this->CameraView->resize(this->ui->CameraWidget->size());
}


