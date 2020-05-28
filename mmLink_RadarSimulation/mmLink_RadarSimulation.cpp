#include "mmLink_RadarSimulation.h"
#include "ui_mmLink_RadarSimulation.h"
#include <QtEndian>


mmLink_RadarSimulation::mmLink_RadarSimulation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mmLink_RadarSimulation)
{
    ui->setupUi(this);

    tcpServer = new QTcpServer(this);
    ui->edtIP->setText(QNetworkInterface().allAddresses().at(1).toString());   //获取本地IP
    ui->btnConnect->setEnabled(true);
    ui->btnSend->setEnabled(false);

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
}

mmLink_RadarSimulation::~mmLink_RadarSimulation()
{
    delete ui;
}
// newConnection -> newConnectionSlot 新连接建立的槽函数
void mmLink_RadarSimulation::NewConnectionSlot()
{
    currentClient = tcpServer->nextPendingConnection();
    tcpClient.append(currentClient);
    ui->cbxConnection->addItem(tr("%1:%2").arg(currentClient->peerAddress().toString().split("::ffff:")[1])\
                                          .arg(currentClient->peerPort()));
    connect(currentClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
}

// 客户端数据可读信号，对应的读数据槽函数
void mmLink_RadarSimulation::ReadData()
{
    // 由于readyRead信号并未提供SocketDecriptor，所以需要遍历所有客户端
    for(int i=0; i<tcpClient.length(); i++)
    {
        QByteArray buffer = tcpClient[i]->readAll();
        if(buffer.isEmpty())    continue;

        static QString IP_Port, IP_Port_Pre;
        IP_Port = tr("[%1:%2]:").arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1])\
                                     .arg(tcpClient[i]->peerPort());

        // 若此次消息的地址与上次不同，则需显示此次消息的客户端地址
        if(IP_Port != IP_Port_Pre)
            ui->edtRecv->append(IP_Port);

        ui->edtRecv->append(buffer.toHex());

        //更新ip_port
        IP_Port_Pre = IP_Port;
    }
}
// disconnected -> disconnectedSlot 客户端断开连接的槽函数
void mmLink_RadarSimulation::disconnectedSlot()
{
    //由于disconnected信号并未提供SocketDescriptor，所以需要遍历寻找
    for(int i=0; i<tcpClient.length(); i++)
    {
        if(tcpClient[i]->state() == QAbstractSocket::UnconnectedState)
        {
            // 删除存储在combox中的客户端信息
            ui->cbxConnection->removeItem(ui->cbxConnection->findText(tr("%1:%2")\
                                  .arg(tcpClient[i]->peerAddress().toString().split("::ffff:")[1])\
                                  .arg(tcpClient[i]->peerPort())));
            // 删除存储在tcpClient列表中的客户端信息
             tcpClient[i]->destroyed();
             tcpClient.removeAt(i);
        }
    }
}
// 监听--断开
void mmLink_RadarSimulation::on_btnConnect_clicked()
{
    if(ui->btnConnect->text()=="监听")
    {
        bool ok = tcpServer->listen(QHostAddress::Any, ui->edtPort->text().toInt());
        if(ok)
        {
            ui->btnConnect->setText("断开");
            ui->btnSend->setEnabled(true);
        }
    }
    else
    {
        for(int i=0; i<tcpClient.length(); i++)//断开所有连接
        {
            tcpClient[i]->disconnectFromHost();
            bool ok = tcpClient[i]->waitForDisconnected(1000);
            if(!ok)
            {
                // 处理异常
            }
            tcpClient.removeAt(i);  //从保存的客户端列表中取去除
        }
        tcpServer->close();     //不再监听端口
        ui->btnConnect->setText("监听");
        ui->btnSend->setEnabled(false);
    }
}
// 发送数据


void mmLink_RadarSimulation::ConstructSendbuffer()
{
    memset(&this->SendData,0,sizeof(UnionData_t));
    QString Payload=this->ui->Payload->text();
    QStringList Payloadlist=Payload.split(",");
    qToBigEndian((20+2*Payloadlist.size()),&this->SendData.RadarData.RadarHead.Length);
    qToBigEndian(this->ui->Slope->text().toInt(),&this->SendData.RadarData.RadarHead.Slope);
    qToBigEndian((short)this->ui->SampleRate->text().toInt(),&this->SendData.RadarData.RadarHead.SampleRate);
    qToBigEndian((short)this->ui->SamplePoint->text().toInt(),&this->SendData.RadarData.RadarHead.SamplePoint);
    qToBigEndian((short)this->ui->ChirpNumber->text().toInt(),&this->SendData.RadarData.RadarHead.ChirpNumber);
    if(this->ui->FirstFlag->isChecked())
    {
        qToBigEndian((short)1,&this->SendData.RadarData.RadarHead.FirstFlag);
    }
    else
    {
        qToBigEndian((short)0,&this->SendData.RadarData.RadarHead.FirstFlag);
    }

    int j=0;
    for (QStringList::Iterator i=Payloadlist.begin();i!=Payloadlist.end();i++)
    {
        qToBigEndian((short)i->toInt(),&this->SendData.RadarData.RadarPayload[j++]);
        //qDebug("%d",this->SendData.RadarData.RadarPayload[j]);
    }
    for(j=0;j<qFromBigEndian(this->SendData.RadarData.RadarHead.Length);j++)
    {
        qDebug("%d",this->SendData.SendBuffer[j]);
    }
    this->ui->Length->setText(QString::number(20+2*Payloadlist.size()));
}

void mmLink_RadarSimulation::on_btnSend_clicked()
{
    this->ConstructSendbuffer();
    QByteArray data2(this->SendData.SendBuffer,qFromBigEndian(this->SendData.RadarData.RadarHead.Length));

    for (int i=0;i<data2.length();i++)
    {
        if(data2[i]>='0'&&data2[i]<='9')
        {
            data2[i]=data2[i]-'0';
            qDebug("ok");
        }
        if(data2[i]>='a'&&data2[i]<='f')
        {
            data2[i]=data2[i]-'a'+10;
        }
    }
    data2[3]=data2.length();

    if(ui->cbxConnection->currentIndex() == 0)
    {
        for(int i=0; i<tcpClient.length(); i++)
            tcpClient[i]->write(data2); //qt5除去了.toAscii()
    }
    //指定连接
    else
    {
        QString clientIP = ui->cbxConnection->currentText().split(":")[0];
        int clientPort = ui->cbxConnection->currentText().split(":")[1].toInt();
//        qDebug() << clientIP;
//        qDebug() << clientPort;
        for(int i=0; i<tcpClient.length(); i++)
        {
            if(tcpClient[i]->peerAddress().toString().split("::ffff:")[1]==clientIP\
                            && tcpClient[i]->peerPort()==clientPort)
            {
                tcpClient[i]->write(data2);
                return; //ip:port唯一，无需继续检索
            }
        }
    }
}

void mmLink_RadarSimulation::on_btnClear_clicked()
{
    ui->edtRecv->clear();
}

