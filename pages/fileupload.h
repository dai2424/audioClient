#ifndef FILEUPLOAD_H
#define FILEUPLOAD_H

#include <QObject>
#include <QWidget>

class QLabel;
class QLineEdit;
class TcpSession;
class QPushButton;
class QListWidget;
class QTableWidget;

class FileUpload : public QWidget
{
    Q_OBJECT
public:
    explicit FileUpload(QWidget *parent = nullptr, TcpSession *session = nullptr);
//功能
private:
    void RegisterFunc();
    void LogoutFunc();
    TcpSession *m_session;

//界面
private:
    QWidget *bgWidget;
    QWidget *toolWidget;
    QLineEdit *filePath;
    QLineEdit *fileName;
    QPushButton *clooseFileBtn;
    QPushButton *sendFileBtn;

    void sendJson();
    void ClooseFile();
//请求和响应
private:




signals:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // FILEUPLOAD_H
