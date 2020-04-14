#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::SaveVideo()
{
    if(QMessageBox::Save==QMessageBox::information(this,"保存视频","您是否想要保存录制的视频?",QMessageBox::Save|QMessageBox::No,QMessageBox::Save))
    {
        qDebug()<<"save ok";
    }
}
