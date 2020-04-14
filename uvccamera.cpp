#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::UpdateAvailableCamerasSlot()
{
    AvailableCameras.clear();
    AvailableCameras=QCameraInfo::availableCameras();
    this->ui->CameraBox->clear();
    QList<QCameraInfo>::iterator i;
    for (i=this->AvailableCameras.begin();i!=this->AvailableCameras.end();i++)
    {
        qDebug()<<i->deviceName()<<endl<<i->description();
        this->ui->CameraBox->addItem(i->description());
    }
}

void MainWindow::CameraConnectSlot()
{
    if(this->ui->CameraConnectButton->text()=="连接摄像头")
    {
        QObject::disconnect(this->CurrentCamera,SIGNAL(error(QCamera::Error)),this,SLOT(CameraErrorSlot(QCamera::Error)));
        if(this->CurrentCamera!=nullptr)
        {
            delete this->CurrentCamera;
            this->CurrentCamera=nullptr;
        }

        if(this->AvailableCameras.count()==0)
            return;

        for (const QCameraInfo &cameraInfo : this->AvailableCameras)
        {
            if (cameraInfo.description() == this->ui->CameraBox->currentText())
            {
                qDebug()<<cameraInfo.description();
                this->CurrentCamera=new QCamera(cameraInfo);
            }
        }
        this->CameraView->resize(this->ui->CameraWidget->size());

        this->CurrentCamera->setViewfinder(this->CameraView);
        this->CurrentCameraFocus=this->CurrentCamera->focus();
        if(this->CurrentCamera->isCaptureModeSupported(QCamera::CaptureVideo))
            this->CurrentCamera->setCaptureMode(QCamera::CaptureVideo);
        else
            QMessageBox::warning(this,"警告","此摄像头录像效果可能不佳");
        this->CurrentCamera->start();

        this->ui->CameraConnectButton->setText("断开");
        this->ui->RecordButton->setEnabled(true);
        this->ui->Focurs->setMaximum(this->CurrentCameraFocus->maximumDigitalZoom()*100);
        this->ui->Focurs->setMinimum(0);
    }
    else if(this->ui->CameraConnectButton->text()=="断开")
    {
        QObject::connect(this->CurrentCamera,SIGNAL(error(QCamera::Error)),this,SLOT(CameraErrorSlot(QCamera::Error)));
        this->CurrentCamera->stop();
        if(this->CurrentCamera!=nullptr)
        {
            delete this->CurrentCamera;
            this->CurrentCamera=nullptr;
        }
        this->ui->CameraConnectButton->setText("连接摄像头");
        this->ui->RecordButton->setEnabled(false);
        if(this->ui->RecordButton->text()=="停止录制")
        {
            this->SaveVideo();
            this->ui->RecordButton->setText("录制视频");
        }
    }
}


void MainWindow::CameraErrorSlot(QCamera::Error value)
{
    qDebug()<<"camera error"<<value;
    QMessageBox::critical(this,"错误","摄像头发生了一个错误");
    this->ui->CameraConnectButton->setText("断开");
    CameraConnectSlot();
}

void MainWindow::CameraZoomSlot(int value)
{
    if(this->ui->CameraConnectButton->text()=="断开")
    {
        qDebug()<<value;
        this->CurrentCameraFocus->zoomTo(1,value/100);
    }
}

void MainWindow::CameraRecordSlot()
{
    if(this->ui->RecordButton->text()=="录制视频")
    {
        this->CameraRecorder=new QMediaRecorder(this->CurrentCamera);
        this->ui->RecordButton->setText("停止录制");
    }
    else if(this->ui->RecordButton->text()=="停止录制")
    {
        this->SaveVideo();
        this->ui->RecordButton->setText("录制视频");
    }
}
