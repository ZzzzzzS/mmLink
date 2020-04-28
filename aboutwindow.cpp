#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include "license.h"
#include <QUrl>
#include <QStringList>
#include <QPixmap>
#include <QMessageBox>
#include <QSysInfo>
#include <opencv2/opencv.hpp>

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->setWindowTitle(tr("关于mmLink"));
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
    LicenseBox.setWindowTitle(tr("开源声明"));
    LicenseBox.setText(license);
    LicenseBox.exec();
}

void AboutWindow::SetInformation()
{
    QString QtVersion;
    QtVersion=tr("主机名：")+QSysInfo::machineHostName()+"\n";
    QtVersion+=tr("Qt版本：")+tr(qVersion())+"\n";
    QtVersion+=tr("OpenCV版本:")+QString::fromStdString(cv::getVersionString())+"\n";
    QtVersion+=(tr("操作系统版本：")+QSysInfo::productType()+" "+QSysInfo::productVersion()+" "+QSysInfo::currentCpuArchitecture());
    QtVersion+=("\n"+tr("内核版本：")+QSysInfo::kernelType()+" "+QSysInfo::kernelVersion());
    this->ui->QtVersion->setText(QtVersion);
    QString mmLinkVersion;
    mmLinkVersion=tr("mmLink1.0\n")+tr("Powered By 周子顺\n")+tr("Copyright©2020 All Rights Reserved")+"\n"+tr("哈尔滨工业大学(威海) 信息科学与工程学院");
    this->ui->mmLinkVersion->setText(mmLinkVersion);
}
