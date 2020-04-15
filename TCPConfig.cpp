#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::TCPConnectSlot()
{
    if(this->ui->RadarConnectButton->text()==QString("连接雷达"))
    {
        if(ui->ServerPort->text().toUInt()>65535)
        {
            QMessageBox::critical(this,"错误","请填写正确的服务器地址");
            return;
        }

        RadarSocket->connectToHost(QHostAddress(ui->ServerIP->text()),ui->ServerPort->text().toUInt());
        this->ui->RadarConnectButton->setText("正在连接");
        this->ui->RadarConnectButton->setEnabled(false);
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
}

void MainWindow::TCPConnectSuccessedSlot()
{
    this->ui->RadarConnectButton->setText("断开");
    this->ui->RadarConnectButton->setEnabled(true);
}

void MainWindow::TCPDisconnectSuccessedSlot()
{
    this->ui->RadarConnectButton->setText("连接雷达");
    this->ui->RadarConnectButton->setEnabled(true);
}

void MainWindow::TCPReceiveSlot()
{
    qDebug()<<RadarSocket->readAll();
    this->RadarSocket->write("receive");
}
