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

bool DataManagement::ConvertFrame(int Frame)
{
   return true;
}

void DataManagement::SaveData(QVector<short> Data)
{
    QMutexLocker locker(&this->Lock);
    bool ok;
    RadarQuery->prepare("INSERT INTO Data VALUES (NULL, @package)");
    for(QVector<short>::iterator i=Data.begin();i!=Data.end();i++)
    {
        RadarQuery->bindValue("@package",*i);
        ok=RadarQuery->exec();
        if(!ok)
        {
           qDebug()<<RadarQuery->lastError();
        }
    }
}
