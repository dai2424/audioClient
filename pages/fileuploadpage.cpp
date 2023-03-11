#include "fileuploadpage.h"
#include "qheaderview.h"
#include "net/tcpsession.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonObject>
#include <QFileDialog>

FileUploadPage::FileUploadPage(QWidget *parent, TcpSession *session)
    : QWidget{parent}, m_session(session)
{
    file_upload_ui();
}

void FileUploadPage::file_upload_ui()
{
    file_upload_table =new QTableWidget(this);
    file_upload_table->setGeometry(20,130,1191,651);
    creat_filetable();


    QLabel *file_name=new QLabel(this);
    file_name->setText("文件名称");
    file_name->setGeometry(20,20,91,21);

    QLabel *file_status=new QLabel(this);
    file_status->setText("状态");
    file_status->setGeometry(410,20,91,21);

    QLineEdit *file_nameedit=new QLineEdit(this);
    file_nameedit->setGeometry(120,10,241,41);
    file_nameedit->setPlaceholderText("请输入文件名称");

    QLineEdit *file_statusedit=new QLineEdit(this);
    file_statusedit->setGeometry(470,10,241,41);
    file_statusedit->setPlaceholderText("请输入状态");

    QPushButton *addBt=new QPushButton(this);
    addBt->setText("添加");
    addBt->setGeometry(30,70,91,38);
    addBt->setStyleSheet("QPushButton {font-size: bold 20px;color: #ffffff;background-color: #55aaff; font-family:黑体;}");
    addBt->setIcon(QIcon(":/pic/add.png"));
    //设置控件的布局方向，默认是Qt::LeftToRight，可设置Qt::RightToLeft 从右往左
    addBt->setLayoutDirection(Qt::LeftToRight);



    QPushButton *deleteBt=new QPushButton(this);
    deleteBt->setText("删除");
    deleteBt->setGeometry(150,70,91,38);
    deleteBt->setStyleSheet("QPushButton {font-size: bold 20px;color: #ffffff;background-color: #ff6365; font-family:黑体;}");
    deleteBt->setIcon(QIcon(":/pic/delete.png"));
    deleteBt->setLayoutDirection(Qt::LeftToRight);

    QPushButton *insquireBt=new QPushButton(this);
    insquireBt->setText("查找");
    insquireBt->setGeometry(740,10,91,38);
    insquireBt->setStyleSheet("QPushButton {font-size: bold 20px;color: #ffffff;background-color: #55aaff; font-family:黑体;}");
    insquireBt->setIcon(QIcon(":/pic/insquire.png"));
    insquireBt->setLayoutDirection(Qt::LeftToRight);

    QPushButton *resetBt=new QPushButton(this);
    resetBt->setText("重置");
    resetBt->setGeometry(860,10,91,38);

    connect(addBt,&QPushButton::clicked, this,&FileUploadPage::addfile);
    connect(deleteBt,&QPushButton::clicked, this,&FileUploadPage::delete_more_file);
    connect(insquireBt,&QPushButton::clicked, this,&FileUploadPage::inquirefile);
}

void FileUploadPage::creat_filetable()
{
    file_upload_table->setColumnCount(6);//设置列数
    file_upload_table->setRowCount(10);//设置行数

    QStringList m_Header;
    m_Header<<QString(" ")<<QString("文件名称")<<QString("文件路径")<<QString("审核状态")<<QString("上传时间")<<QString("操作");

    //filetable->horizontalHeader()->setStretchLastSection(true);

    file_upload_table->setHorizontalHeaderLabels(m_Header);//添加横向表头
    file_upload_table->verticalHeader()->setVisible(true);//纵向表头可视化
    file_upload_table->horizontalHeader()->setVisible(true);//横向表头可视化
    //filetable->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置编辑方式：禁止编辑表格
    //filetable->setSelectionBehavior(QAbstractItemView::SelectRows);//设置表格选择方式：设置表格为整行选中
    //ui->qTableWidget->setSelectionBehavior(QAbstractItemView::SelectColumns);//设置表格选择方式：设置表格为整列选中
    //filetable->setSelectionMode(QAbstractItemView::SingleSelection);//选择目标方式
    file_upload_table->setStyleSheet("selection-background-color:#dcdcdc");//设置选中颜色：粉色




    file_upload_table->setItem(0,0,new QTableWidgetItem("3121"));
    file_upload_table->setItem(0,1,new QTableWidgetItem("3121"));
    file_upload_table->setItem(0,2,new QTableWidgetItem("3121"));
    file_upload_table->setItem(0,3,new QTableWidgetItem("3121"));

    QPushButton *removeBt=new QPushButton();
    removeBt->setText("删除");
    removeBt->resize(91,38);
    file_upload_table->setCellWidget(0, 5, removeBt);

    removeBt->setStyleSheet("QPushButton {font-size: bold 20px;color: #ffffff;background-color: #ff6365; font-family:黑体;}");
    removeBt->setIcon(QIcon(":/pic/delete.png"));
    removeBt->setLayoutDirection(Qt::LeftToRight);



    QTableWidgetItem *checkBox = new QTableWidgetItem();
     checkBox->setCheckState(Qt::Checked);
     file_upload_table ->setItem(0, 0, checkBox);
     connect(file_upload_table, SIGNAL(cellChanged(int,int)), this, SLOT(changeTest(int, int)));




     connect(removeBt,&QPushButton::clicked,this,[=](){delete_one_file();});





}

void FileUploadPage::addfile()
{
    QFont ft;
    ft.setPointSize(12);
    QWidget *add_file=new QWidget;
    add_file->setWindowTitle("添加文件");
    QLabel *file_name=new QLabel(add_file);
    file_name->setText("*文件名:：");
    file_name->setGeometry(110,100,121,50);
    file_name->setFont(ft);

    QLabel *file_wav=new QLabel(add_file);
    file_wav->setText("*文件路径:");
    file_wav->setGeometry(110,200,121,50);
    file_wav->setFont(ft);

    QLineEdit *file_nameedit=new QLineEdit(add_file);
    file_nameedit->setGeometry(300,100,241,41);
    file_nameedit->setPlaceholderText("请输入文件名称");

    QLineEdit *file_wavedit=new QLineEdit(add_file);
    file_wavedit->setGeometry(300,200,241,41);

    QPushButton *close_pushButton=new QPushButton(add_file);
    close_pushButton->setGeometry(110,300,91,38);
    close_pushButton->setText("删除");

    QPushButton *sure_pushButton=new QPushButton(add_file);
    sure_pushButton->setGeometry(450,300,91,38);
    sure_pushButton->setText("确认");
    sure_pushButton->setStyleSheet("QPushButton {font-size: bold 20px;color: #ffffff;background-color: #55aaff; font-family:黑体;}");

    QPushButton *choose_fileBt=new QPushButton(add_file);
    choose_fileBt->setText("选择文件");
    choose_fileBt->setGeometry(580,200,91,38);
    connect(choose_fileBt,&QPushButton::clicked,[=](){
        QString path=choose_file();
        if(path!="")
            file_wavedit->setText(path);

    });


    add_file->show();

    connect(close_pushButton,&QPushButton::clicked, [=](){choose_fileBt->close();});
    connect(sure_pushButton,&QPushButton::clicked,[=](){
        if(file_nameedit->text()!=""){
            QJsonObject jsonObject;
            jsonObject.insert("request","addfile");
            jsonObject.insert("file_name",file_nameedit->text());
            jsonObject.insert("check_status","审核通过");
            jsonObject.insert( "upload_time",10);
            m_session->WriteJson(jsonObject);
          }
        else
            QMessageBox::information(this, "提示对话框","文件名不能为空！", QMessageBox::Ok);
        });    //根据QString分发Json


}

void FileUploadPage::delete_more_file()
{

}

void FileUploadPage::inquirefile()
{

}

QString FileUploadPage::choose_file()
{
    QString file_path = QFileDialog::getOpenFileName(this,"请选择本地数据...","./",tr("Image Files (*.mp3 *.wav))"));
    if(file_path.isEmpty()){

    }
    else{
      qDebug() << "选择的数据文件夹为："<< file_path;
      return file_path;
    }


}

void FileUploadPage::delete_one_file()
{
    QList<QTableWidgetItem*> items = file_upload_table->selectedItems();

    if(!items.empty()) //表格有某行被选中
    {
        int rowIndex = file_upload_table->currentRow();
        qDebug()<<"rowIndex"<<rowIndex;
        int choose= QMessageBox::question(this,"提示","是否确认删除",QMessageBox::Yes|QMessageBox::No);
        if(choose==QMessageBox::Yes){
            file_upload_table->setSelectionBehavior ( QAbstractItemView::SelectRows);
            file_upload_table->setSelectionMode ( QAbstractItemView::SingleSelection);
            file_upload_table->removeRow(rowIndex);
            QJsonObject jsonObject;
            jsonObject.insert("request","removefile");
            jsonObject.insert("file_name",file_upload_table->item(rowIndex,1)->text());

            m_session->WriteJson(jsonObject);

         }

    }
    else
         QMessageBox::information(this, "提示对话框","未选中行！", QMessageBox::Ok);

}

void FileUploadPage::changeTest(int row, int col)
{
    if(file_upload_table ->item(row, col)->checkState() == Qt::Checked) //选中
    {

    }
}
