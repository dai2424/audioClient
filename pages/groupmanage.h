#ifndef GROUPMANAGE_H
#define GROUPMANAGE_H

#include "page.h"
#include <QWidget>
#include <QDialog>

class QLabel;
class QLineEdit;
class TcpSession;
class QPushButton;
class QListWidget;
class QTableWidget;

class GroupManage : public Page
{
    Q_OBJECT
public:
    explicit GroupManage(QWidget *parent = nullptr, TcpSession *session = nullptr);
    ~GroupManage();
//请求和响应函数
private:
    //请求分组
    void RequestGroups();
    void ResponseGroups(QJsonObject &data, TcpSession *session);
    //添加分组
    void RequestAddGroup(QString gpName, uint gpNo);
    //删除分组
    void RequestDeleteGroup(uint gpNo);
    //请求修改分组名
    void RequestUpdateGroup(uint groupNo, QString newName);
    //请求设备列表
    void RequestDevList(int currentRow);
    void ResponseDevList(QJsonObject &data, TcpSession *session);
    //删除分组下设备
    void ReqeustDeleteDevToGroup();
    //添加设备到分组
    void RequestAddDevToGroup(uint gpNo, uint devNo);
//功能
private:
    void RegisterFunc();
    void LogoutFunc();

//界面
private:
    void InitUI();

    //分组列表
    QPushButton *u_addGroupBtn;
    QPushButton *u_deleteGroupBtn;
    QPushButton *u_updateGroupBtn;
    QListWidget *u_groupList;

    //设备列表
    QTableWidget *u_devTable;
    QLineEdit *u_selectEdit;
    QPushButton *u_selectBtn;
    QPushButton *u_addDevToGroupBtn;

private slots:
    void NewAGD();  //新建AddGroupDialog对话框
    void NewADD();  //新建添加设备对话框
    void SlotDelGroup();
    void SlotUpdateGroup();

protected:
    void paintEvent(QPaintEvent *event) override;
};

/** 对话框 **/
class AddGroupDialog : public QDialog {
    Q_OBJECT
public:
    explicit AddGroupDialog(QWidget *parent = nullptr);

private:
    QPushButton *u_confirmBtn;
    QPushButton *u_cancelBtn;

    QLabel *u_gpNoLb;
    QLineEdit *u_gpNoEdit;
    QLabel *u_gpNameLb;
    QLineEdit *u_gpNameEdit;
private slots:
    void SlotConfirm(); //接受确认按钮信号的槽函数

signals:
    void SignalAddGroup(QString gpName, uint gpNo);
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GROUPMANAGE_H
