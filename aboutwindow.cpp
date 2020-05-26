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

    this->Music=new QMediaPlayer(this);
    this->Music->setMedia(QUrl("qrc:/sound/aboutwindow"));
    this->SetLogo();
    this->SetInformation();
    this->Music->play();
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
    mmLinkVersion=tr("mmLink1.0\n")+tr("Powered By ZhouZishun")+"\n"+tr("Copyright©2020\nAll Rights Reserved")+"\n"+tr("School of Information\nScience and Technology,\nHIT at WeiHai");
    this->ui->mmLinkVersion->setText(mmLinkVersion);
}
