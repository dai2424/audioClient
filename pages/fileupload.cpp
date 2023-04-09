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
#include <QAction>
#include <qcombobox.h>

FileUpload::FileUpload(QWidget *parent, TcpSession *session)
    : Page{parent, session}
{
    InitUI();
    RegisterFunc();
}

void FileUpload::RegisterFunc()
{
    using namespace std::placeholders;
    //类型别名
    using Func = std::function<void(QJsonObject &, TcpSession *)>;
    //引用分发器的指针
    Dispatcher *dispatcher = Dispatcher::GetDispatcher();
}

void FileUpload::LogoutFunc()
{

}


void FileUpload::InitUI()
{
    /* 设置背景 */
    u_bgWidget = new QWidget(this);
    u_bgWidget->lower();    //至于顶层

    u_bgWidget->setStyleSheet("QWidget {"
                              "background-color : white;"
                              "border-radius : 15px;"
                              "}");

    /* 设置组件布局Widget */
    u_toolWidget = new QWidget(this);

    u_filePathEdit = new QLineEdit(this);
    u_fileNameEdit = new QLineEdit(this);
    u_fileTypeBox = new QComboBox(this);
    u_sendBtn = new QPushButton(this);

    //添加提示文字
    u_filePathEdit->setPlaceholderText(QString("请选择文件路径"));
    u_fileNameEdit->setPlaceholderText(QString("请输入文件名"));
    u_fileTypeBox->addItem(QString(".mp3"));
    u_fileTypeBox->addItem(QString(".txt"));

    //为u_filePathEdit添加图标按钮
    QPushButton *selectFileBtn = new QPushButton(this);
    selectFileBtn->setIcon(QIcon(":/image/selectFile.png"));
    QHBoxLayout *filePathLayout = new QHBoxLayout();
    filePathLayout->setContentsMargins(0, 3, 3, 3);
    filePathLayout->addStretch();
    filePathLayout->addWidget(selectFileBtn);
    u_filePathEdit->setLayout(filePathLayout);

    //为u_toolWidget添加布局
    QVBoxLayout *toolLayout = new QVBoxLayout();
    toolLayout->addWidget(u_filePathEdit);
    toolLayout->addWidget(u_fileNameEdit);
    toolLayout->addWidget(u_fileTypeBox);
    toolLayout->addWidget(u_sendBtn, 1, Qt::AlignHCenter);
    u_toolWidget->setLayout(toolLayout);

    //设置样式
    u_filePathEdit->setStyleSheet("QLineEdit{"
                                  "border : 1px solid #ccc;"
                                  "border-radius : 5px;"
                                  "height : 30px;"
                                  "padding : 0px 27px 0px 5px;"
                                  "font-size : 18px;"
                                  "}");
    selectFileBtn->setStyleSheet("QPushButton{"
                                 "width : 24px;"
                                 "height : 24px;"
                                 "border : none;"
                                 "border-radius : 3px;"
                                 "}"
                                 "QPushButton::hover{"
                                 "background-color : #bbbec4;"
                                 "}");

    u_fileNameEdit->setStyleSheet("QLineEdit{"
                                  "border : 1px solid #ccc;"
                                  "border-radius : 5px;"
                                  "height : 30px;"
                                  "padding : 0px 27px 0px 5px;"
                                  "font-size : 18px;"
                                  "}");
    u_fileTypeBox->setStyleSheet("QComboBox{"
                                 "border : 1px solid #ccc;"
                                 "border-radius : 5px;"
                                 "height : 30px;"
                                 "font-size : 18px;"
                                 "background: transparent;"
                                 "}"
                                 "QComboBox::down-arrow{"
                                 "width : 24px;"
                                 "background: transparent;"
                                 "padding: 0px 0px 0px 0px;"
                                 "image: url(:/image/arrow-down.png);"
                                 "}"
                                 "QComboBox::drop-down{"
                                 "width : 24px;"
                                 "border-left : 1px solid #ccc;"
                                 "}");
    u_sendBtn->setText("上传");
    u_sendBtn->setMaximumWidth(300);
    u_sendBtn->setMinimumWidth(100);
    u_sendBtn->setMinimumHeight(30);
    u_sendBtn->setStyleSheet("QPushButton{"
                             "border : neno;"
                             "border-radius : 5px;"
                             "background-color : #ff623e;"
                             "color : rgb(255, 255, 255);"
                             "font: 15px 黑体;"
                             "}");
    /* 关联信号函数 */
    //点击图标选择文件路径
    connect(selectFileBtn, &QPushButton::clicked, this, &FileUpload::ClooseFile);
    //点击上传发送请求
    connect(u_sendBtn, &QPushButton::clicked, this, &FileUpload::UpLoadFile);
}

void FileUpload::ClooseFile()
{
    u_filePathEdit->setText(
            QFileDialog::getOpenFileName(this, "选择文件", "","音频文件( *mp3 )")
    );
}

void FileUpload::UpLoadFile()
{
    //读取文件
    QFile file(u_filePathEdit->text());
    file.open(QIODevice::ReadOnly);
    QByteArray fileData = file.readAll().toBase64();
    file.close();

    QJsonObject obj;
    QString data = QString(fileData);
    obj.insert("request", "fileUpload");
    obj.insert("data", data);
    obj.insert("fileName", u_fileNameEdit->text());
    obj.insert("fileType", u_fileTypeBox->currentText());
    m_session->WriteJson(obj);
};

void FileUpload::paintEvent(QPaintEvent *event)
{
    u_toolWidget->move(this->width() * 0.25, this->height() * 0.25);
    u_toolWidget->resize(this->width() * 0.5, this->height() * 0.5);
    u_bgWidget->move(this->width() * 0.15, this->height() * 0.15);
    u_bgWidget->resize(this->width() * 0.7, this->height() * 0.7);
    QWidget::paintEvent(event);
}
