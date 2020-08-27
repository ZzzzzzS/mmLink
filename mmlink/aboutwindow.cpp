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

#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QUrl>
#include <QStringList>
#include <QPixmap>
#include <QMessageBox>
#include <QSysInfo>
#include <QFile>
#include <QTextStream>

using namespace cv::cuda;

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->setWindowTitle(tr("About mmLink"));
    this->setFixedSize(this->size());

    QObject::connect(ui->ExitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    QObject::connect(ui->OpenCVButton,SIGNAL(clicked(bool)),this,SLOT(OpenCVSlot()));
    QObject::connect(ui->QtButton,SIGNAL(clicked(bool)),qApp,SLOT(aboutQt()));
    QObject::connect(ui->FeedBackButton,SIGNAL(clicked(bool)),this,SLOT(FeedBackSlot()));
    QObject::connect(ui->OpenSourceButton,SIGNAL(clicked(bool)),this,SLOT(OpenSourceSlot()));

#ifdef Q_OS_WIN //用的WSL2编译Linux，不支持播放声音
    this->Music=new QMediaPlayer(this);
    this->Music->setMedia(QUrl("qrc:/sound/aboutwindow"));
    this->Music->play();
#endif
    this->SetLogo();
    this->SetInformation();
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::SetLogo()
{
    QImage tmp("://image/opencv.png");
    QPixmap value=QPixmap::fromImage(tmp);
    this->ui->OpenCVImage->setPixmap(value.scaled(this->ui->OpenCVImage->size(),Qt::KeepAspectRatio));
    tmp.load("://image/qt.png");
    value=QPixmap::fromImage(tmp);
    this->ui->QtImage->setPixmap(value.scaled(this->ui->QtImage->size(),Qt::KeepAspectRatio));
    tmp.load("://image/logo.png");
    value=QPixmap::fromImage(tmp);
    this->ui->mmLinkImage->setPixmap(value.scaled(this->ui->mmLinkImage->size(),Qt::KeepAspectRatio));
}

void AboutWindow::OpenCVSlot()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://opencv.org/")));
}

void AboutWindow::FeedBackSlot()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/ZzzzzzS/mmLink")));
}

void AboutWindow::OpenSourceSlot()
{
    QMessageBox LicenseBox;
    LicenseBox.setWindowTitle(tr("License"));

    QFile license;
    license.setFileName(":/LICENSE/LICENSE");
    license.open(QIODevice::ReadOnly);
    QTextStream buffer(&license);

    LicenseBox.setText(buffer.readAll());
    LicenseBox.exec();
}

void AboutWindow::SetInformation()
{
    QString QtVersion;
    QtVersion=tr("This software is licensed to ")+QSysInfo::machineHostName()+tr("use")+"\n";
    QtVersion+=tr("Qt Version: ")+tr(qVersion())+"\n";
    QtVersion+=tr("OpenCV Version: ")+QString::fromStdString(cv::getVersionString())+"\n";
    QtVersion+=(tr("OS Version: ")+QSysInfo::productType()+" "+QSysInfo::productVersion()+" "+QSysInfo::currentCpuArchitecture());
    QtVersion+=("\n"+tr("kernal Version: ")+QSysInfo::kernelType()+" "+QSysInfo::kernelVersion());
    this->ui->QtVersion->setText(QtVersion);
    QString mmLinkVersion;
    mmLinkVersion=tr("mmLink\n")+tr("Powered By ZhouZishun")+"\n"+tr("Copyright©2020\nAll Rights Reserved")+"\n"+tr("School of Information\nScience and Technology,\nHIT at WeiHai");
    this->ui->mmLinkVersion->setText(mmLinkVersion);
}
