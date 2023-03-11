#ifndef FILEUPLOADPAGE_H
#define FILEUPLOADPAGE_H

#include <QWidget>

class TcpSession;
class QTableWidget;

class FileUploadPage : public QWidget
{
    Q_OBJECT
public:
    explicit FileUploadPage(QWidget *parent = nullptr, TcpSession *session = nullptr);

private:
    QTableWidget *file_upload_table;
    TcpSession *m_session;
    void file_upload_ui();
    void creat_filetable();
    void addfile();
    void delete_more_file();
    void inquirefile();
    QString choose_file();
    void delete_one_file();
    void changeTest(int row, int col);
signals:

};

#endif // FILEUPLOADPAGE_H
