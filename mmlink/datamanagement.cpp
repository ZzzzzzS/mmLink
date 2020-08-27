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

#include "datamanagement.h"

DataManagement::DataManagement(QObject *parent) : QObject(parent)
{
    this->RadarDatabase=QSqlDatabase::addDatabase("QSQLITE");//指定数据库类型
    this->RadarQuery=new QSqlQuery(this->RadarDatabase);
}

bool DataManagement::ConnectSQL(QString DatabaseName)
{
    this->RadarDatabase.setDatabaseName(DatabaseName);//设定数据库名称
    if(!this->RadarDatabase.open())//打开数据库
    {
        qDebug("fail to open sql");
        return false;
    }
    if(!this->RadarQuery->exec("CREATE TABLE IF NOT EXISTS Data (Id INTEGER PRIMARY KEY AUTOINCREMENT, Package BLOB)"))
    {
        qDebug("fail to creat table");
        return false;
    }
    if(!this->RadarQuery->exec("CREATE UNIQUE INDEX IF NOT EXISTS IdIndex ON Data (Id)"))
    {
        qDebug("fail on sql operation");
        return false;
    }
    return true;
}


//转换帧率并储存为一个bin文件
bool DataManagement::ConvertFrame(int Frame,QString FileName)
{
    if(!this->RadarDatabase.isOpen())
    {
        qDebug()<<"Database is not open";
        return false;
    }
    QFile file(FileName);
    bool ok=file.open(QIODevice::WriteOnly);
    if(!ok)
    {
        return false;
    }
    QDataStream stream(&file);
    if(!this->RadarQuery->exec("SELECT * FROM Data"))
    {
        qDebug("fail on sql operation");
        return false;
    }
    int counter=Frame;
    short var;
    while(this->RadarQuery->next()) //开始转换帧率并写入到bin文件
    {
        if(counter++>=Frame)
        {
            var=this->RadarQuery->value(0).toInt();
            stream<<var;
            counter=0;
        }
    }
    file.close();
   return true;
}

void DataManagement::SaveData(QVector<short> Data) //保存数据
{
    QMutexLocker locker(&this->Lock);//工作在另一个线程，而且边接收信息边保存，为了保险加锁保护下
    //其实我觉得不加锁也没问题，qt的信号槽传参跨线程的话是要把数据复制一遍的
    QByteArray Value;
    bool ok;
    RadarQuery->prepare("INSERT INTO Data VALUES (NULL,:package)");
    for(QVector<short>::iterator i=Data.begin();i!=Data.end();i++)
    {
        Value=QByteArray::number(*i);
        RadarQuery->bindValue(":package",Value);
        ok=RadarQuery->exec();
        if(!ok)
        {
           qDebug()<<RadarQuery->lastError();
        }
    }
}

void DataManagement::CloseDatabase()
{
    if(this->RadarDatabase.isOpen())
        this->RadarDatabase.close();
}
