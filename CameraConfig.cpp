#include "mainwindow.h"
#include "ui_mainwindow.h"

//连接摄像头的函数
void MainWindow::CameraConnectSlot()
{
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
    if(this->ui->CameraConnectButton->text()=="连接摄像头")
    {
        if(this->ui->CameraAddress->text()=="输入摄像头编号或视频流地址")
        {
            if(QMessageBox::warning(this,"将使用默认摄像头","未指定输入设备,将使用默认设备",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            {
                this->ui->CameraAddress->setText("0");
                qDebug("ok");
            }
            else
            {
                return;
            }
        }
        this->CameraThread->start();//启动线程准备采集
        emit CameraInit(this->ui->CameraAddress->text());//发送信号，让摄像头初始化
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

//摄像头连接成功信号
void MainWindow::CameraStartedSlot()
{
    this->ui->CameraConnectButton->setText("断开");
    this->ui->CameraConnectButton->setEnabled(true);
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
    this->ui->RecordButton->setEnabled(true);
    this->ui->RecordButton->setText("录制视频");
}

//摄像头断开成功信号
void MainWindow::CameraStoppedSlot()
{
    this->ui->CameraConnectButton->setText("连接摄像头");
    this->ui->CameraConnectButton->setEnabled(true);
    this->SetLogo();
    this->ui->RecordButton->setEnabled(false);
    this->ui->RecordButton->setText("录制视频");
}

//摄像头打开失败信号
void MainWindow::CameraErrorSlot()
{
    this->ui->CameraConnectButton->setText("连接摄像头");
    this->ui->CameraConnectButton->setEnabled(true);
    QMessageBox::critical(this,"摄像头打开失败","请检查摄像头连接或视频流地址是否正确");
}

//摄像头采集信号
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

//刷新摄像头显示信号
void MainWindow::RenewImageSlot(QPixmap image)
{
    this->ui->CameraView->setPixmap(image.scaled(this->ui->CameraView->size(),Qt::KeepAspectRatio));
}
