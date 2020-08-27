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


//注意编译的时候需要lrelease一下，不然没法产生翻译所需的.qm文件，就只能显示英文界面或者编译失败
void SetLanguage(QTranslator& Translator)
{
    QLocale local;
    qDebug()<<local.language();
    if(local.language()==QLocale::Chinese)
    {
        Translator.load(":/Language/mmlink_zh_CN.qm");
        qApp->installTranslator(&Translator);//翻译成英文
    }
    else
    {
        Translator.load(":/Language/mmlink_us_EN.qm");
        qApp->installTranslator(&Translator);//翻译成中文
    }
}
