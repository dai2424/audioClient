#include "fileupload.h"
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
#include <QFile>
#include <QFileDialog>

FileUpload::FileUpload(QWidget *parent, TcpSession *session)
    : QWidget(parent), m_session(session)
{
    bgWidget = new QWidget(this);
    bgWidget->setStyleSheet("background-color : white;"
                            "border-radius : 10px;");
    bgWidget->lower();

    toolWidget = new QWidget(this);

    filePath = new QLineEdit(this);
    filePath->setText("‪C:/Users/ASUS/Desktop/眉南边.mp3");
    fileName = new QLineEdit(this);
    clooseFileBtn = new QPushButton(this);
    clooseFileBtn->setText("选择文件");
    sendFileBtn = new QPushButton(this);
    sendFileBtn->setText("发送");

    connect(clooseFileBtn, &QPushButton::clicked, this, &FileUpload::ClooseFile);
    connect(sendFileBtn, &QPushButton::clicked, this, &FileUpload::sendJson);

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(filePath, 0, 0);
    layout->addWidget(fileName, 1, 0);
    layout->addWidget(clooseFileBtn, 0, 1);
    layout->addWidget(sendFileBtn, 1, 1);
    toolWidget->setLayout(layout);

}

void FileUpload::RegisterFunc()
{

}

void FileUpload::LogoutFunc()
{

}

void FileUpload::sendJson()
{
    QFile file(filePath->text());
    file.open(QIODevice::ReadOnly);
    QByteArray fileData = file.readAll().toBase64();
    file.close();
    qDebug() << fileData.size();
    QJsonObject obj;
    QString data = QString(fileData);
    qDebug() << data.length();
    obj.insert("request", "fileUpload");
    obj.insert("data", data);
    m_session->WriteJson(obj);
}

void FileUpload::ClooseFile()
{
    filePath->setText(
            QFileDialog::getOpenFileName(this, "选择文件", "","音频文件( *mp3 )")
        );
};

void FileUpload::paintEvent(QPaintEvent *event)
{
    toolWidget->move(this->width() * 0.15, this->height() * 0.15);
    toolWidget->resize(this->width() * 0.7, this->height() * 0.7);
    bgWidget->move(this->width() * 0.15, this->height() * 0.15);
    bgWidget->resize(this->width() * 0.7, this->height() * 0.7);
    QWidget::paintEvent(event);
}
