#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::TCPConnectSlot()
{
    if(this->ui->RadarConnectButton->text()==QString(tr("Connect Radar")))
    {
        if(ui->ServerPort->text().toUInt()>65535||ui->ServerPort->text().toUInt()<=0)
        {
            QMessageBox::critical(this,tr("Error"),tr("Please fill in the correct server port"));
            return;
        }
        this->ui->RadarConnectButton->setText(tr("Connecting"));
        this->ui->RadarConnectButton->setEnabled(false);
        this->RadarData->DatabaseTime=QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
        QString SQLAddr=QString(qApp->applicationDirPath()+"/Data/DataRecords-")+this->RadarData->DatabaseTime+".db";
        qDebug()<<SQLAddr;
        this->RadarData->CloseDatabase();//先关闭打开的数据库
        this->RadarData->ConnectSQL(SQLAddr);
        RadarSocket->connectToHost(QHostAddress(ui->ServerIP->text()),ui->ServerPort->text().toUInt());

    }
    else if(this->ui->RadarConnectButton->text()==QString(tr("Disconnect")))
    {

        this->ui->RadarConnectButton->setText(tr("Disconnecting"));
        this->ui->RadarConnectButton->setEnabled(false);
        RadarSocket->disconnectFromHost();
    }
}

void MainWindow::TCPErrorSlot(QAbstractSocket::SocketError socketError)
{
    qDebug()<<socketError;
    if(socketError==QAbstractSocket::ConnectionRefusedError)
    {
        QMessageBox::critical(this,tr("Error"),tr("The remote server rejected the connection"));
        this->ui->RadarConnectButton->setText(tr("Connect Radar"));
        this->ui->RadarConnectButton->setEnabled(true);
    }
    else if(socketError==QAbstractSocket::HostNotFoundError)
    {
        QMessageBox::critical(this,tr("Error"),tr("Please fill in the correct server address"));
        this->ui->RadarConnectButton->setText(tr("Connect Radar"));
        this->ui->RadarConnectButton->setEnabled(true);
    }
    else if(socketError==QAbstractSocket::RemoteHostClosedError)
    {
        QMessageBox::critical(this,tr("disconnected"),tr("server disconnected"));
    }
    else if(socketError==QAbstractSocket::NetworkError)
    {
        this->ui->RadarConnectButton->setText(tr("Connect Radar"));
        this->ui->RadarConnectButton->setEnabled(true);
        QMessageBox::critical(this,tr("Error"),tr("Network Error"));
    }
    else
    {
        this->ui->RadarConnectButton->setText(tr("Connect Radar"));
        this->ui->RadarConnectButton->setEnabled(true);
        QMessageBox::critical(this,tr("Error"),tr("Error Number: ")+QString::number(socketError));
    }
}

void MainWindow::TCPConnectSuccessedSlot()
{
    this->ui->RadarConnectButton->setText(tr("Disconnect"));
    this->ui->RadarConnectButton->setEnabled(true);
    this->ui->ClinetIP->setText(this->RadarSocket->localAddress().toString());
    this->ui->ClinetPort->setText(QString::number(this->RadarSocket->localPort()));
}

void MainWindow::TCPDisconnectSuccessedSlot()
{
    this->ui->RadarConnectButton->setText(tr("Connect Radar"));
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
        this->RadarSocket->RadarBufferCompress();//分析收到的数据，并组合成一个雷达帧
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

    if(!this->RadarSocket->UpdateRadarParameter())
        QMessageBox::warning(this,tr("radar parameter error"),tr("Please fill in the correct radar parameters"));
}

void MainWindow::RenewRadarDataSlot()
{
    QVector<double> Xaxis(this->RadarSocket->TimeData.size());
    for(int i=0;i<Xaxis.size();i++)
    {
        Xaxis[i]=i;
    }
    QVector<double> Time=QVector<double>::fromStdVector(this->RadarSocket->TimeData);
    QVector<double> Freq=QVector<double>::fromStdVector(this->RadarSocket->FreqDomain->FFTMagnitude);
    QVector<double> Phase=QVector<double>::fromStdVector(this->RadarSocket->FreqDomain->FFTPhase);
    bool AutoScale=this->ui->AxisBox->isChecked();//判断是否需要自动缩放坐标轴
    this->RadarTimePlot->addNewDataSlot(Xaxis,Time,AutoScale);
    this->RadarFreqPlot->addNewDataSlot(Xaxis,Freq,AutoScale);
    this->RadarPhasePlot->addNewDataSlot(Xaxis,Phase,AutoScale);
    qDebug()<<"Time:"<<Time<<"Magnitude:"<<Freq<<"Phase:"<<Phase;
}

void MainWindow::ConvertFrameSlot()
{
    int number=this->ui->FlameRate->text().toInt();
    if(number<=0)//判断帧率是否输入正确
    {
        QMessageBox::critical(this,tr("Frame Rate is Not Correct"),tr("Please Input Correct Frame Rate"),QMessageBox::Ok);
        return;
    }
    if(!this->RadarData->ConvertFrame(number,qApp->applicationDirPath()+"/Data/"+this->RadarData->DatabaseTime+".bin"))
    {
        QMessageBox::critical(this,tr("Error"),tr("Fail to Convert Frame Rate"),QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(this,tr("Succee"),tr("Convert Complete"),QMessageBox::Ok);
    }
}

void MainWindow::SetEndianSlot()
{
    if(this->ui->EndianButton->text()==tr("Set Big Endian"))
    {
        this->RadarSocket->RadarEndian=mmWaveRadar::BigEndian;
        this->ui->EndianButton->setText(tr("Set Little Endian"));
        QMessageBox::information(this,tr("Big Endian"),tr("Currently in big endian mode"),QMessageBox::Ok);
    }
    else if(this->ui->EndianButton->text()==tr("Set Little Endian"))
    {
        this->RadarSocket->RadarEndian=mmWaveRadar::LittleEndian;
        this->ui->EndianButton->setText(tr("Set Big Endian"));
        QMessageBox::information(this,tr("Little Endian"),tr("Currently in little endian mode"),QMessageBox::Ok);
    }
}

void MainWindow::GetRadarEndian()
{
    if(this->RadarSocket->RadarEndian==mmWaveRadar::BigEndian)
    {
        this->ui->EndianButton->setText(tr("Set Little Endian"));
    }
    else
    {
        this->ui->EndianButton->setText(tr("Set Big Endian"));
    }
}
