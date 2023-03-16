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
#include <QHeaderView>
#include <QInputDialog>

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

    Func responseDevList = std::bind(&GroupManage::ResponseDevList, this, _1, _2);
    dispatcher->RegitserFunc("reGetGroupDevs", {responseDevList, "ResponseDevList"});
}

void GroupManage::LogoutFunc()
{
    //引用分发器的指针
    Dispatcher *dispatcher = Dispatcher::GetDispatcher();

    dispatcher->LogoutFunc("reGetGroupList", "ResponseGroups");
    dispatcher->LogoutFunc("regetGroupDevs", "ResponseDevList");
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

void GroupManage::RequestUpdateGroup(uint groupNo, QString newName)
{
    QJsonObject requestObj;
    requestObj.insert("request", "updateGroupName");
    requestObj.insert("groupNo", (int)groupNo);
    requestObj.insert("groupName", newName);

    m_session->WriteJson(requestObj);
}

void GroupManage::RequestDevList(int currentRow)
{
    auto item = u_groupList->item(currentRow);
    if(!item) { //无效的
        return;
    }
    else {
        uint gpNo = item->data(Qt::UserRole).toUInt();
        QJsonObject requestObj;
        requestObj.insert("request", "getGroupDevs");
        requestObj.insert("groupNo", (int)gpNo);

        m_session->WriteJson(requestObj);
    }
}

void GroupManage::ResponseDevList(QJsonObject &data, TcpSession *session)
{
    Q_UNUSED(session);
    QJsonArray devValArry = data.value("devList").toArray();
    \
    //清空表格
    u_devTable->setRowCount(0);

    //插入数据
    for(auto devVal : devValArry) {
        QJsonObject devObj = devVal.toObject();
        int rowCnt = u_devTable->rowCount();
        u_devTable->insertRow(rowCnt);

        QTableWidgetItem *nameItem = new QTableWidgetItem(devObj.value("devName").toString());
        nameItem->setTextAlignment(Qt::AlignCenter);
        u_devTable->setItem(rowCnt, 0, nameItem);
        QTableWidgetItem *noItem = new QTableWidgetItem(QString::number(devObj.value("devNo").toInt()));
        noItem->setTextAlignment(Qt::AlignCenter);
        u_devTable->setItem(rowCnt, 1, noItem);
        QPushButton *deleteBtn = new QPushButton(u_devTable);
        deleteBtn->setText(tr("删除"));
        u_devTable->setCellWidget(rowCnt, 2, deleteBtn);

        connect(deleteBtn, &QPushButton::clicked, this, &GroupManage::ReqeustDeleteDevToGroup);
    }
}

void GroupManage::ReqeustDeleteDevToGroup()
{
    uint groupNo = u_groupList->currentItem()->data(Qt::UserRole).toUInt();
    int index = u_devTable->currentRow();
    uint devNo = u_devTable->item(index, 1)->text().toUInt();

    QJsonObject questObj;
    questObj.insert("request", "deleteDevToGroup");
    questObj.insert("groupNo", (int)groupNo);
    questObj.insert("devNo", (int)devNo);
    m_session->WriteJson(questObj);

    RequestDevList(u_groupList->currentRow());
}

void GroupManage::RequestAddDevToGroup(uint gpNo, uint devNo)
{
    QJsonObject requestObj;
    requestObj.insert("request", "addDevToGroup");
    requestObj.insert("devNo", (int)devNo);
    requestObj.insert("groupNo", (int)gpNo);

    m_session->WriteJson(requestObj);
    RequestDevList(u_groupList->currentRow());
}


/** 界面 **/
void GroupManage::InitUI()
{
    //界面主布局
    QHBoxLayout *mainLayout = new QHBoxLayout();
    this->setLayout(mainLayout);

/* 分组列表 */
    //列表
    u_groupList = new QListWidget(this);
    u_groupList->setMaximumWidth(300);

    connect(u_groupList, &QListWidget::currentRowChanged, this, &GroupManage::RequestDevList);
    //按钮
    u_addGroupBtn = new QPushButton(QString("添加"));
    u_addGroupBtn->setMaximumWidth(80);
    u_deleteGroupBtn = new QPushButton(QString("删除"));
    u_deleteGroupBtn->setMaximumWidth(80);
    u_updateGroupBtn = new QPushButton(QString("修改"));
    u_updateGroupBtn->setMaximumWidth(80);

    connect(u_addGroupBtn, &QPushButton::clicked, this, &GroupManage::NewAGD);
    connect(u_deleteGroupBtn, &QPushButton::clicked, this, &GroupManage::SlotDelGroup);
    connect(u_updateGroupBtn, &QPushButton::clicked, this, &GroupManage::SlotUpdateGroup);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignLeft);
    btnLayout->addWidget(u_addGroupBtn);
    btnLayout->addWidget(u_deleteGroupBtn);
    btnLayout->addWidget(u_updateGroupBtn);

    //布局
    QVBoxLayout *groupLayout = new QVBoxLayout();
    groupLayout->addLayout(btnLayout);
    groupLayout->addWidget(u_groupList);

    //向上级布局添加
    mainLayout->addLayout(groupLayout);
    mainLayout->setStretchFactor(groupLayout, 3);

 /* 设备表 */
    u_devTable = new QTableWidget(this);
    u_devTable->setColumnCount(3);
    QStringList tableLabels;
    tableLabels << "设备名" << "设备号" << "操作";
    u_devTable->setHorizontalHeaderLabels(tableLabels);
    u_devTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section{"
            "border-top:0px solid #E5E5E5;"
            "border-left:0px solid #E5E5E5;"
            "border-right:0.5px solid #E5E5E5;"
            "border-bottom:0.5px solid #E5E5E5;"
            "background-color:white;"
            "padding:4px;"
        "}"
    );
    u_devTable->verticalHeader()->setStyleSheet(
        "QHeaderView::section{"
            "border-top:0px solid #E5E5E5;"
            "border-left:0px solid #E5E5E5;"
            "border-right:0.5px solid #E5E5E5;"
            "border-bottom:0.5px solid #E5E5E5;"
            "background-color:white;"
            "padding:4px;"
        "}"
    );
    u_devTable->setStyleSheet(
        "QTableCornerButton::section{"
            "border-top:0px solid #E5E5E5;"
            "border-left:0px solid #E5E5E5;"
            "border-right:0.5px solid #E5E5E5;"
            "border-bottom:0.5px solid #E5E5E5;"
            "background-color:white;"
        "}"
    );
    QHeaderView *hv = u_devTable->horizontalHeader();
    hv->setStretchLastSection(true);
    hv->setSectionResizeMode(QHeaderView::Fixed);

    //搜索栏 和 新增按钮 布局
    u_selectEdit = new QLineEdit(this);
    u_selectBtn = new QPushButton(this);
    u_selectBtn->setText(tr("搜索"));
    u_addDevToGroupBtn = new QPushButton(this);
    u_addDevToGroupBtn->setText(tr("新增"));
    connect(u_addDevToGroupBtn, &QPushButton::clicked, this, &GroupManage::NewADD);

    QHBoxLayout *selectLayout = new QHBoxLayout();
    selectLayout->addWidget(u_addDevToGroupBtn);
    selectLayout->addWidget(u_selectEdit);
    selectLayout->addWidget(u_selectBtn);


    QVBoxLayout *devLayout = new QVBoxLayout();
    devLayout->addLayout(selectLayout);
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

void GroupManage::NewADD()
{
    bool isOk = false;

    QString devNoS = QInputDialog::getText(this,
                                      "添加设备到到当前分组",
                                      "请输入设备名",
                                      QLineEdit::Normal,
                                      "1000",
                                      &isOk
                                      );
    if(isOk) {
        int devNo = devNoS.toInt();
        if(devNo <= 0) return;
        uint groupNo = u_groupList->currentItem()->data(Qt::UserRole).toUInt();
        RequestAddDevToGroup(groupNo, (uint)devNo);
    }

}

void GroupManage::SlotDelGroup()
{
    uint gpNo = u_groupList->currentItem()->data(Qt::UserRole).toUInt();
    RequestDeleteGroup(gpNo);
}

void GroupManage::SlotUpdateGroup()
{
    QListWidgetItem *curItem = u_groupList->currentItem();
    if(curItem) {
        bool isOk = false;
        QString groupName = QInputDialog::getText(this,
                                          "修改分组名",
                                          "请输入新的分组名",
                                          QLineEdit::Normal,
                                          curItem->text(),
                                          &isOk
                                          );
        if(isOk) {
            uint groupNo = curItem->data(Qt::UserRole).toUInt();
            RequestUpdateGroup(groupNo, groupName);
            curItem->setText(groupName);
        }
    }
}

//重写父类函数
void GroupManage::paintEvent(QPaintEvent *event)
{
    //调整u_devTable中列的占比大小
    QHeaderView *devTableHv = u_devTable->horizontalHeader();
    devTableHv->resizeSection(0, devTableHv->width() * (0.4));
    devTableHv->resizeSection(1, devTableHv->width() * (0.4));
    devTableHv->resizeSection(2, devTableHv->width() * (0.2));

    QWidget::paintEvent(event);
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











