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

//连接摄像头的函数
void MainWindow::CameraConnectSlot()
{
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
    if(this->ui->CameraConnectButton->text()==tr("Connect Camera"))
    {
        if(this->ui->CameraAddress->text()==tr("Enter camera number or video stream address"))
        {
            if(QMessageBox::warning(this,tr("Using Default Camera"),tr("No input device specified, the default device will be used"),QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
            {
                this->ui->CameraAddress->setText("0");
            }
            else
            {
                return;
            }
        }
        this->CameraThread->start();//启动线程准备采集
        emit CameraInit(this->ui->CameraAddress->text());//发送信号，让摄像头初始化
        this->ui->CameraConnectButton->setText(tr("Connecting"));
        this->ui->CameraConnectButton->setEnabled(false);
    }
    else if(this->ui->CameraConnectButton->text()==tr("Disconnect"))
    {
        this->Camera->StopCamera();
        this->ui->CameraConnectButton->setText(tr("Connect Camera"));
        this->ui->CameraConnectButton->setText(tr("Disconnecting"));
        this->ui->CameraConnectButton->setEnabled(false);
    }
}

//摄像头连接成功信号
void MainWindow::CameraStartedSlot()
{
    this->ui->CameraConnectButton->setText(tr("Disconnect"));
    this->ui->CameraConnectButton->setEnabled(true);
    this->ui->CameraView->resize(this->ui->RadarPhaseData->size());
    this->ui->RecordButton->setEnabled(true);
    this->ui->RecordButton->setText(tr("Record Video"));
}

//摄像头断开成功信号
void MainWindow::CameraStoppedSlot()
{
    this->ui->CameraConnectButton->setText(tr("Connect Camera"));
    this->ui->CameraConnectButton->setEnabled(true);
    this->SetLogo();
    this->ui->RecordButton->setEnabled(false);
    this->ui->RecordButton->setText(tr("Record Video"));
}

//摄像头打开失败信号
void MainWindow::CameraErrorSlot()
{
    this->ui->CameraConnectButton->setText(tr("Connect Camera"));
    this->ui->CameraConnectButton->setEnabled(true);
    QMessageBox::critical(this,tr("fail to open camera"),tr("Please check the camera connection or video stream address"));
}

//摄像头采集信号
void MainWindow::CameraRecordSlot()
{
    if(this->ui->RecordButton->text()==tr("Record Video"))
    {
        this->Camera->StartRecording("./Data/VideoRecord-"+QDateTime::currentDateTime().toString("yyyyMMddHHmmss")+".avi");
        this->ui->RecordButton->setText(tr("stop recording"));
    }
    else if(this->ui->RecordButton->text()==tr("stop recording"))
    {
        this->Camera->StopRecording();
        this->ui->RecordButton->setText(tr("Record Video"));
    }
}

//刷新摄像头显示信号
void MainWindow::RenewImageSlot(QPixmap image)
{
    this->ui->CameraView->setPixmap(image.scaled(this->ui->CameraView->size(),Qt::KeepAspectRatio));
}
