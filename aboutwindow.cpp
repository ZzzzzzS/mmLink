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
    this->setWindowTitle(tr("关于mmLink"));
    this->setFixedSize(this->size());

    QObject::connect(ui->ExitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    QObject::connect(ui->OpenCVButton,SIGNAL(clicked(bool)),this,SLOT(OpenCVSlot()));
    QObject::connect(ui->QtButton,SIGNAL(clicked(bool)),qApp,SLOT(aboutQt()));
    QObject::connect(ui->FeedBackButton,SIGNAL(clicked(bool)),this,SLOT(FeedBackSlot()));
    QObject::connect(ui->OpenSourceButton,SIGNAL(clicked(bool)),this,SLOT(OpenSourceSlot()));

#ifdef USE_CUDA
    this->CUDADevice=cv::makePtr<cv::cuda::DeviceInfo>();
    this->isCUDASupport=this->CUDADevice->isCompatible();
    QObject::connect(ui->CUDAButton,SIGNAL(clicked(bool)),this,SLOT(CUDASlot()));
    this->SetCUDAInformation();
#else
    this->ui->CUDAButton->hide();
    this->ui->CUDAImage->hide();
    this->ui->CUDAVersion->hide();
#endif

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
#ifdef USE_CUDA
    tmp.load("://image/cuda.png");
    value=QPixmap::fromImage(tmp);
    this->ui->CUDAImage->setPixmap(value.scaled(this->ui->CUDAImage->size(),Qt::KeepAspectRatio));
#endif
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
    LicenseBox.setWindowTitle(tr("开源声明"));

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
    QtVersion=tr("此软件授权给")+QSysInfo::machineHostName()+tr("使用")+"\n";
    QtVersion+=tr("Qt版本：")+tr(qVersion())+"\n";
    QtVersion+=tr("OpenCV版本:")+QString::fromStdString(cv::getVersionString())+"\n";
    QtVersion+=(tr("操作系统版本：")+QSysInfo::productType()+" "+QSysInfo::productVersion()+" "+QSysInfo::currentCpuArchitecture());
    QtVersion+=("\n"+tr("内核版本：")+QSysInfo::kernelType()+" "+QSysInfo::kernelVersion());
    this->ui->QtVersion->setText(QtVersion);
    QString mmLinkVersion;
    mmLinkVersion=tr("mmLink1.0\n")+tr("Powered By 周子顺")+"\n"+tr("Copyright©2020\nAll Rights Reserved")+"\n"+tr("哈尔滨工业大学(威海)\n信息科学与工程学院");
    this->ui->mmLinkVersion->setText(mmLinkVersion);
}

#ifdef USE_CUDA
void AboutWindow::SetCUDAInformation()
{
    if(!isCUDASupport)
    {
        this->ui->CUDAVersion->setText(tr("当前设备不支持CUDA加速"));
    }
    else
    {
        QString CUDAInfo;
        CUDAInfo+=tr("显卡型号：");
        CUDAInfo+=QString::fromStdString(this->CUDADevice->name())+"\n";
        CUDAInfo+=tr("显存大小：");
        unsigned long int mem=this->CUDADevice->totalMemory();
        mem/=1048576;
        CUDAInfo+=(QString::number(mem)+"MB\n");
        CUDAInfo+=tr("CUDA计算能力：");
        CUDAInfo+=(QString::number(this->CUDADevice->majorVersion())+"."+QString::number(this->CUDADevice->minorVersion())+"\n");

        this->ui->CUDAVersion->setText(CUDAInfo);
    }
}

void AboutWindow::CUDASlot()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://developer.nvidia.com/about-cuda")));
}

#endif
