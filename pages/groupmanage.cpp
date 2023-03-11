#include "groupmanage.h"
#include "net/tcpsession.h"
#include "net/dispatcher.h"


#include <QPushButton>
#include <QListWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <functional>
#include <QJsonArray>
#include <QLabel>
#include <QLineEdit>

GroupManage::GroupManage(QWidget *parent, TcpSession *session)
    : QWidget{parent}, m_session(session)
{
    InitUI();
    RegisterFunc();

    RequestGroups();

}

GroupManage::~GroupManage()
{
    LogoutFunc();
}

void GroupManage::RegisterFunc()
{
    using namespace std::placeholders;
    //类型别名
    using Func = std::function<void(QJsonObject &, TcpSession *)>;
    //引用分发器的指针
    Dispatcher *dispatcher = Dispatcher::GetDispatcher();

    Func responseGroups = std::bind(&GroupManage::ResponseGroups, this, _1, _2);
    dispatcher->RegitserFunc("reGetGroupList", {responseGroups, "ResponseGroups"});
}

void GroupManage::LogoutFunc()
{
    //引用分发器的指针
    Dispatcher *dispatcher = Dispatcher::GetDispatcher();

     dispatcher->LogoutFunc("reGetGroupList", "ResponseGroups");
}

/** 请求和响应函数 **/
void GroupManage::RequestGroups()
{
    QJsonObject requestObj;
    requestObj.insert("request", "getGroupList");
    m_session->WriteJson(requestObj);

}

void GroupManage::ResponseGroups(QJsonObject &data, TcpSession *session)
{
    Q_UNUSED(session);

    //清空已有数据
    u_groupList->clear();

    QJsonArray devJsonArray = data.value("grouplist").toArray();
    for(auto devJson : devJsonArray) {
        QJsonObject devObj =  devJson.toObject();
        QString groupName = devObj.value("groupName").toString();
        uint groupNo = (uint)devObj.value("groupNo").toInt();

        QListWidgetItem *item = new QListWidgetItem();
        item->setText(groupName);
        item->setData(Qt::UserRole, groupNo);
        u_groupList->addItem(item);
    }
}

void GroupManage::RequestAddGroup(QString gpName, uint gpNo)
{
    QJsonObject requestObj;
    requestObj.insert("request", "addGroup");
    requestObj.insert("groupName", gpName);
    requestObj.insert("groupNo", (int)gpNo);

    m_session->WriteJson(requestObj);

    RequestGroups();
}

void GroupManage::RequestDeleteGroup(uint gpNo)
{
    QJsonObject requestObj;
    requestObj.insert("request", "deleteGroup");
    requestObj.insert("groupNo", (int)gpNo);
    m_session->WriteJson(requestObj);

    RequestGroups();
}


/** 界面 **/
void GroupManage::InitUI()
{
    //界面主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    this->setLayout(mainLayout);

/* 分组列表 */
    //列表
    u_groupList = new QListWidget(this);
    u_groupList->setMaximumWidth(300);

    //按钮
    u_addGroupBtn = new QPushButton(QString("添加"));
    u_addGroupBtn->setMaximumHeight(70);
    u_deleteGroupBtn = new QPushButton(QString("删除"));
    u_deleteGroupBtn->setMaximumHeight(70);

    connect(u_addGroupBtn, &QPushButton::clicked, this, &GroupManage::NewAGD);
    connect(u_deleteGroupBtn, &QPushButton::clicked, this, &GroupManage::SlotDelGroup);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignLeft);
    btnLayout->addWidget(u_addGroupBtn);
    btnLayout->addWidget(u_deleteGroupBtn);

    //布局
    QVBoxLayout *groupLayout = new QVBoxLayout();
    groupLayout->addLayout(btnLayout);
    groupLayout->addWidget(u_groupList);

    //向上级布局添加
    mainLayout->addLayout(groupLayout);
    mainLayout->setStretchFactor(groupLayout, 3);

 /* 设备表 */
    QVBoxLayout *devLayout = new QVBoxLayout();
    u_devTable = new QTableWidget(this);
    devLayout->addWidget(u_devTable);

    mainLayout->addLayout(devLayout);
    mainLayout->setStretchFactor(devLayout, 7);

}

void GroupManage::NewAGD()
{
    AddGroupDialog *dialog = new AddGroupDialog(this);
    connect(dialog, &AddGroupDialog::SignalAddGroup,
            this, &GroupManage::RequestAddGroup);
    dialog->show();
}

void GroupManage::SlotDelGroup()
{
    uint gpNo = u_groupList->currentItem()->data(Qt::UserRole).toUInt();
    RequestDeleteGroup(gpNo);
}


/** 对话框类 **/

AddGroupDialog::AddGroupDialog(QWidget *parent)
    :QDialog(parent)
{
    this->setWindowTitle(tr("添加分组"));

    //主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setMargin(50);

    //分组名
    u_gpNameLb = new QLabel(tr("分组名"));
    u_gpNameEdit = new QLineEdit(this);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(u_gpNameLb, 0,  Qt::AlignLeft);
    nameLayout->addWidget(u_gpNameEdit);
    mainLayout->addLayout(nameLayout);

    //分组编号
    u_gpNoLb = new QLabel(tr("分组编号"));
    u_gpNoEdit = new QLineEdit(this);

    QHBoxLayout *noLayout = new QHBoxLayout();
    noLayout->addWidget(u_gpNoLb);
    noLayout->addWidget(u_gpNoEdit);
    mainLayout->addLayout(noLayout);

    //按钮
    u_confirmBtn = new QPushButton(tr("确认"));
    u_cancelBtn = new QPushButton(tr("取消"));
    u_confirmBtn->setMaximumWidth(100);
    u_cancelBtn->setMaximumWidth(100);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(u_confirmBtn);
    btnLayout->addWidget(u_cancelBtn);
    mainLayout->addLayout(btnLayout);

    connect(u_confirmBtn, &QPushButton::clicked, this, &AddGroupDialog::SlotConfirm);
    connect(u_cancelBtn, &QPushButton::clicked, this, &QDialog::close);
}

void AddGroupDialog::SlotConfirm()
{
    QString gpName = u_gpNameEdit->text();
    uint gpNo = u_gpNoEdit->text().toUInt();
    emit SignalAddGroup(gpName, gpNo);
    this->close();
}

void AddGroupDialog::resizeEvent(QResizeEvent *event)
{
    u_gpNameLb->setFixedWidth(u_gpNoLb->width());

    QDialog::resizeEvent(event);
}











