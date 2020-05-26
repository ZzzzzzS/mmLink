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
    while(this->RadarQuery->next())
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

void DataManagement::SaveData(QVector<short> Data)
{
    QMutexLocker locker(&this->Lock);
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
