#ifndef mmLink_RadarSimulation_H
#define mmLink_RadarSimulation_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QMessageBox>
namespace Ui {
class mmLink_RadarSimulation;
}

class mmLink_RadarSimulation : public QMainWindow
{
    Q_OBJECT

public:
    explicit mmLink_RadarSimulation(QWidget *parent = 0);
    ~mmLink_RadarSimulation();

private:
    Ui::mmLink_RadarSimulation *ui;
    QTcpServer *tcpServer;
    QList<QTcpSocket*> tcpClient;
    QTcpSocket *currentClient;

    //发送的参数
        typedef struct
        {
            int Length;
            int Slope;
            short DataID;
            short FirstFlag;
            short SampleRate;
            short SamplePoint;
            short ChirpNumber;
            short FrameNumber;
        }RadarHead_t;

        typedef struct
        {
            RadarHead_t RadarHead;
            short RadarPayload[(65535-20)/2+1];
        }RadarData_t;

        typedef union
        {
            RadarData_t RadarData;
            char SendBuffer[65535];
        }UnionData_t;

    UnionData_t SendData;
    void ConstructSendbuffer();

private slots:
    void NewConnectionSlot();
    void disconnectedSlot();
    void ReadData();

    void on_btnConnect_clicked();
    void on_btnSend_clicked();
    void on_btnClear_clicked();
};

#endif // mmLink_RadarSimulation_H
