#ifndef DATAMANAGEMENT_H
#define DATAMANAGEMENT_H

#include <QObject>
#include <QVector>
#include <QMutex>
#include <QMutexLocker>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QtDebug>

class DataManagement : public QObject
{
    Q_OBJECT
public:
    explicit DataManagement(QObject *parent = nullptr);
    bool ConnectSQL(QString DatabaseName);
    bool ConvertFrame(int Frame);
public slots:
    void SaveData(QVector<short> Data);

signals:

private:
    QSqlDatabase RadarDatabase;
    QSqlQuery *RadarQuery;
    QMutex Lock;

};

#endif // DATAMANAGEMENT_H
