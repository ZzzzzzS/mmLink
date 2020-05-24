#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>

void SetLanguage(QTranslator& Translator);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator Translator;
    SetLanguage(Translator);

    MainWindow w;
    w.show();
    w.SetLogo();
    return a.exec();
}

void SetLanguage(QTranslator& Translator)
{
    QLocale local;
    qDebug()<<local.language();
    if(local.language()==QLocale::English)
    {
        Translator.load(":/Language/mmlink_us_EN.qm");
        qApp->installTranslator(&Translator);//翻译成英文
    }
    else
    {
        Translator.load(":/Language/mmlink_zh_CN.qm");
        qApp->installTranslator(&Translator);//翻译成中文
    }
}
