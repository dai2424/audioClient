#ifndef FILEUPLOAD_H
#define FILEUPLOAD_H

#include "page.h"

#include <QObject>
#include <QWidget>

class QComboBox;
class QLabel;
class QLineEdit;
class TcpSession;
class QPushButton;
class QListWidget;
class QTableWidget;

class FileUpload : public Page
{
    Q_OBJECT
public:
    explicit FileUpload(QWidget *parent = nullptr, TcpSession *session = nullptr);
//功能
private:
    void RegisterFunc();
    void LogoutFunc();

//界面
private:
    QWidget *u_bgWidget;
    QWidget *u_toolWidget;
    QLineEdit *u_filePathEdit;
    QLineEdit *u_fileNameEdit;
    QPushButton *u_sendBtn;
    QComboBox *u_fileTypeBox;

    void InitUI();
    void ClooseFile();
//请求和响应
private:
    void UpLoadFile();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // FILEUPLOAD_H
