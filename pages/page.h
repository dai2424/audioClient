#ifndef PAGE_H
#define PAGE_H

#include <QObject>
#include <QWidget>

class TcpSession;

class Page : public QWidget
{
    Q_OBJECT
public:
    explicit Page(QWidget *parent = nullptr, TcpSession *session = nullptr);

public:
    void ActFuncs();
    void FreezeFuncs();
    using FuncSignature = std::pair<QString, QString>;
    QVector<FuncSignature> m_funcStVec;
    TcpSession *m_session;
};

#endif // PAGE_H
