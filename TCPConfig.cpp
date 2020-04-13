#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::TCPConnectSlot()
{
    if(this->ui->ConnectButton->text()==QString("连接"))
    {
        qDebug("ok");
        RadarSocket->connectToHost(QHostAddress(ui->ServerIP->text()),ui->ServerPort->text().toUInt());
        this->ui->ConnectButton->setText("正在连接");
        this->ui->ConnectButton->setEnabled(false);
    }
    else if(this->ui->ConnectButton->text()==QString("断开"))
    {

        this->ui->ConnectButton->setText("正在断开");
        this->ui->ConnectButton->setEnabled(false);
        RadarSocket->disconnectFromHost();
    }
}

void MainWindow::TCPErrorSlot(QAbstractSocket::SocketError socketError)
{
    qDebug()<<socketError;
    if(socketError==QAbstractSocket::ConnectionRefusedError)
    {
        QMessageBox::critical(this,"错误","远程服务器拒绝了连接");
        this->ui->ConnectButton->setText("连接");
        this->ui->ConnectButton->setEnabled(true);
    }
    else if(socketError==QAbstractSocket::HostNotFoundError)
    {
        QMessageBox::critical(this,"错误","请填写正确的服务器地址");
        this->ui->ConnectButton->setText("连接");
        this->ui->ConnectButton->setEnabled(true);
    }
    else if(socketError==QAbstractSocket::RemoteHostClosedError)
    {
        QMessageBox::critical(this,"连接已断开","服务器断开连接");
    }
}

void MainWindow::TCPConnectSuccessedSlot()
{
    this->ui->ConnectButton->setText("断开");
    this->ui->ConnectButton->setEnabled(true);
}

void MainWindow::TCPDisconnectSuccessedSlot()
{
    this->ui->ConnectButton->setText("连接");
    this->ui->ConnectButton->setEnabled(true);
}

void MainWindow::TCPReceiveSlot()
{
    qDebug()<<RadarSocket->readAll();
    this->RadarSocket->write("receive");
}
