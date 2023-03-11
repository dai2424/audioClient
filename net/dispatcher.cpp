#include "dispatcher.h"
#include<iostream>
Dispatcher* Dispatcher::dispatcher = nullptr;

Dispatcher::Dispatcher(){

}

Dispatcher* Dispatcher::GetDispatcher() {
    if(dispatcher == nullptr) {
        dispatcher = new Dispatcher();
    }

    return dispatcher;
}

Dispatcher::~Dispatcher() {
    mapper.clear();
    free(dispatcher);
}

void Dispatcher::RegitserFunc(QString response, pFuncQString&& pfunc) {
    mapper.insert({response, pfunc});   //隐式转换为std::pair

    return;
}

void Dispatcher::LogoutFunc(QString response, QString func) {
    //返回首尾迭代器
    auto range = mapper.equal_range(response);

    for(auto it = range.first; it != range.second; ++it ) {
        //如果找到对应功能
        if(it->second.second == func) {
            mapper.erase(it);
            break;
        }
    }

    return;
}


void Dispatcher::Dispatch(QString& identification, QJsonObject &obj, TcpSession *session) {
    auto range = mapper.equal_range(identification);
    for(auto it = range.first; it != range.second; ++it) {
        it->second.first(obj, session);
    }
}


std::vector<Dispatcher::Func> Dispatcher::GetFuncs(QString& response) {
    std::vector<Func> funcList;

    auto range = mapper.equal_range(response);
    QJsonObject s;
    for(auto it = range.first; it != range.second; ++it) {
        funcList.emplace_back(it->second.first);
    }
    return funcList;
}

