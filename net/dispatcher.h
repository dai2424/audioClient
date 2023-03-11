#ifndef __DISPATCHER_H__
#define __DISPATCHER_H__

#include "tcpsession.h"
#include<map>
#include<functional>
#include<QJsonObject>
#include<vector>
#include<QTcpSocket>
#include<QString>

class Dispatcher
{
private:
    /**
     * 定义映射的pair类型
     * function 处理函数
     * QString 为函数标识用于模块推出后注销函数
     */
    using Func = std::function<void(QJsonObject&, TcpSession*)>;
    using pFuncQString = std::pair<Func, QString>;
    std::multimap<QString, pFuncQString> mapper;    //分发器数据结构

    // 私有化构造
    Dispatcher();
    ~Dispatcher();
    Dispatcher(Dispatcher &) = delete;
    Dispatcher(Dispatcher &&) = delete;
    Dispatcher operator=(const Dispatcher&) = delete;

public:
    static Dispatcher* GetDispatcher(); //获取分发器实例

    void RegitserFunc(QString, pFuncQString&&);   //注册函数功能
    void LogoutFunc(QString response, QString func);   //注销函数功能

    void Dispatch(QString& , QJsonObject &, TcpSession*);    //根据QString分发Json

    //返回函数列表,可能会造成空悬指针,不建议使用
    std::vector<Func> GetFuncs(QString&);

private:
    static Dispatcher* dispatcher;
};

#endif
