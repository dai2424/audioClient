#include "tcpsession.h"
#include "dispatcher.h"

#include <QDataStream>
#include <QJsonDocument>
#include <QHostAddress>

TcpSession::TcpSession(QObject *parent)
    : QTcpSocket(parent){
    connect(this, &QTcpSocket::readyRead, this, &TcpSession::Dispatch);
}


void TcpSession::Dispatch() {
    QDataStream in(this);

    while(!in.atEnd()) {
        QByteArray array;
        qint64 size;
        in >> size;
        in >> array;
        qDebug() << size;
        QJsonDocument doc = QJsonDocument::fromJson(array);
        QJsonObject obj = doc.object();
        QString response = obj.value("response").toString();

        Dispatcher::GetDispatcher()->Dispatch(response, obj, this);
    }
}


void TcpSession::WriteJson(QJsonObject &obj) {
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray array = doc.toJson();

    QDataStream out(this);
    out.setVersion(QDataStream::Qt_5_15);
    out << (qint64)array.size();
    out.writeRawData(array.constData(), array.size());

    this->flush();
    this->waitForBytesWritten();
}

bool TcpSession::ConnectTo(QString address, int port)
{
    // 取消已有的连接
    this->abort();
    this->connectToHost(address, port);

    return this->waitForConnected(1000);
}
