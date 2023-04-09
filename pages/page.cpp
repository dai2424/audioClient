#include "page.h"
#include "net/tcpsession.h"
#include "net/dispatcher.h"

Page::Page(QWidget *parent, TcpSession *session)
    :QWidget{parent}, m_session{session}
{

}

void Page::ActFuncs()
{
    Dispatcher::GetDispatcher()->ActFunc(m_funcStVec);
}

void Page::FreezeFuncs()
{
    Dispatcher::GetDispatcher()->FreezeFunc(m_funcStVec);
}
