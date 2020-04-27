#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QUrl>
#include <QStringList>



AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->MODMusic=new QProcess(this);
    this->Music=new QMediaPlayer(this);
    this->Music->setMedia(QUrl::fromLocalFile("aboutconfig"));
    QObject::connect(ui->BlogButton,SIGNAL(clicked(bool)),this,SLOT(BlogSlot()));
    QObject::connect(ui->QtButton,SIGNAL(clicked(bool)),qApp,SLOT(aboutQt()));
    QObject::connect(ui->FeedBackButton,SIGNAL(clicked(bool)),this,SLOT(FeedBackSlot()));
    //this->MODMusic->setProgram("contest.cmd");
    //this->MODMusic->setArguments(QStringList("audioconfig.xm"));
    //this->MODMusic->start();
    this->Music->play();
}

AboutWindow::~AboutWindow()
{
    delete this->MODMusic;
    delete ui;
}

void AboutWindow::BlogSlot()
{
  QDesktopServices::openUrl(QUrl(QLatin1String("https://opencv.org/")));
}

void AboutWindow::FeedBackSlot()
{
  QDesktopServices::openUrl(QUrl(QLatin1String("https://github.com/ZzzzzzS/mmLink")));
}

QProcess::~QProcess()
{
    this->kill();
}
