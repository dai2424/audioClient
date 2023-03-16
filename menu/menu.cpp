#include "menu.h"
#include "menubutton.h"
#include "net/tcpsession.h"

#include "pages/groupmanage.h"
#include "pages/fileupload.h"

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QLine>


Menu::Menu(QWidget *parent)
    : QMainWindow{parent}
{
    this->resize(800, 600);
    m_session = new TcpSession(this);

    m_session->ConnectTo("127.0.0.1", 8888);

    initUI();

}

void Menu::initUI()
{
    //set mainWidget and mainWidget's layout
    QWidget *mainWidget = new QWidget(this);
    this->setCentralWidget(mainWidget);
    QVBoxLayout *mwLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mwLayout);

    //set topNavBar
    u_topNavBar = new QWidget(mainWidget);
    u_topNavBar->setMinimumHeight(70);
    QHBoxLayout *topLayout = new QHBoxLayout(u_topNavBar);
    topLayout->setAlignment(Qt::AlignLeft);
    u_topNavBar->setLayout(topLayout);

    //set u_pages
    u_pages = new QStackedWidget(mainWidget);

    //split line
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Plain);

    mwLayout->addWidget(u_topNavBar);
    mwLayout->addWidget(line);
    mwLayout->addWidget(u_pages);
}

void Menu::SlotSetPage(QString pageName)
{
    qDebug() << pageName;
    //创建和切换页面
    if(m_pageMappper.count(pageName)) {
        u_pages->setCurrentIndex(m_pageMappper.value(pageName));
    }
    else {

        if(pageName == "分组管理") {
            GroupManage *pages = new GroupManage(this, m_session);
            int index = u_pages->addWidget(pages);
            m_pageMappper.insert("分组管理", index);
            u_pages->setCurrentIndex(index);
        }
        if(pageName == "节目管理") {    //示例代码

        }
        if(pageName == "文件上传") {    //示例代码
            FileUpload *pages = new FileUpload(this, m_session);
            int index = u_pages->addWidget(pages);
            m_pageMappper.insert("文件上传", index);
            u_pages->setCurrentIndex(index);
        }
        if(pageName == "文件审核") {    //示例代码

        }
        if(pageName == "分组管理") {    //示例代码

        }
    }
}

void Menu::addModule(QString moduleName)
{
    MenuButton *module = new MenuButton();
    module->setText(moduleName);
    module->setMinimumSize(100, 50);
    auto layout = qobject_cast<QHBoxLayout *>(u_topNavBar->layout());
    layout->addWidget(module, 0, Qt::AlignLeft);

    connect(module, &MenuButton::changedPage, this, &Menu::SlotSetPage);
    m_moduleMapper.insert(moduleName, module);
}

void Menu::addFunc(QString moduleName, QString funcName)
{
    MenuButton * const moduleBtn = m_moduleMapper.value(moduleName);
    moduleBtn->AddAction(funcName);
}

