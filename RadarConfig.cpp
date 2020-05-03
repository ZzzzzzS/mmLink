#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::TCPConnectSlot()
{
    if(this->ui->RadarConnectButton->text()==QString("连接雷达"))
    {
        if(ui->ServerPort->text().toUInt()>65535||ui->ServerPort->text().toUInt()<=0)
        {
            QMessageBox::critical(this,"错误","请填写正确的服务器端口");
            return;
        }
        this->ui->RadarConnectButton->setText("正在连接");
        this->ui->RadarConnectButton->setEnabled(false);
        RadarSocket->connectToHost(QHostAddress(ui->ServerIP->text()),ui->ServerPort->text().toUInt());

    }
    else if(this->ui->RadarConnectButton->text()==QString("断开"))
    {

        this->ui->RadarConnectButton->setText("正在断开");
        this->ui->RadarConnectButton->setEnabled(false);
        RadarSocket->disconnectFromHost();
    }
}

void MainWindow::TCPErrorSlot(QAbstractSocket::SocketError socketError)
{
    qDebug()<<socketError;
    if(socketError==QAbstractSocket::ConnectionRefusedError)
    {
        QMessageBox::critical(this,"错误","远程服务器拒绝了连接");
        this->ui->RadarConnectButton->setText("连接雷达");
        this->ui->RadarConnectButton->setEnabled(true);
    }
    else if(socketError==QAbstractSocket::HostNotFoundError)
    {
        QMessageBox::critical(this,"错误","请填写正确的服务器地址");
        this->ui->RadarConnectButton->setText("连接雷达");
        this->ui->RadarConnectButton->setEnabled(true);
    }
    else if(socketError==QAbstractSocket::RemoteHostClosedError)
    {
        QMessageBox::critical(this,"连接已断开","服务器断开连接");
    }
    else if(socketError==QAbstractSocket::NetworkError)
    {
        this->ui->RadarConnectButton->setText("连接雷达");
        this->ui->RadarConnectButton->setEnabled(true);
        QMessageBox::critical(this,"错误","网络错误");
    }
    else
    {
        this->ui->RadarConnectButton->setText("连接雷达");
        this->ui->RadarConnectButton->setEnabled(true);
        QMessageBox::critical(this,"错误","错误代码："+QString::number(socketError));
    }
}

void MainWindow::TCPConnectSuccessedSlot()
{
    this->ui->RadarConnectButton->setText("断开");
    this->ui->RadarConnectButton->setEnabled(true);
    this->ui->ClinetIP->setText(this->RadarSocket->localAddress().toString());
    this->ui->ClinetPort->setText(QString::number(this->RadarSocket->localPort()));
}

void MainWindow::TCPDisconnectSuccessedSlot()
{
    this->ui->RadarConnectButton->setText("连接雷达");
    this->ui->RadarConnectButton->setEnabled(true);
    qDebug()<<this->RadarSocket->localPort()<<this->RadarSocket->localAddress();
}

void MainWindow::TCPReceiveSlot()
{
    bool ok=this->RadarSocket->ReadRadarData();
    if(ok)
    {
        mmWaveRadar::RadarHead_t value=this->RadarSocket->GetRadarHead();
        this->ui->CurrentSamplePoint->setText(QString::number(value.SamplePoint));
        this->ui->CurrentSampleRate->setText(QString::number(value.SampleRate));
        this->ui->CurrentFrameNumber->setText(QString::number(value.FrameNumber));
        this->ui->CurrentChirp->setText(QString::number(value.ChirpNumber));
    }
    else
    {
        qDebug("receive not correct");
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
