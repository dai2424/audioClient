#ifndef TCPSESSION_H
#define TCPSESSION_H

#include <QTcpSocket>

class TcpSession : public QTcpSocket
{
    Q_OBJECT
public:
    TcpSession(QObject *parent = nullptr);

    //发送数据
    void WriteJson(QJsonObject& obj);

    //建立连接
    bool ConnectTo(QString address, int port);
private:
    void Dispatch();
};

#endif // TCPSESSION_H
