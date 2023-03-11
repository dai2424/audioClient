#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <QMap>


class TcpSession;
class QStackedWidget;
class MenuButton;

class Menu : public QMainWindow
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);


//Func
public:
    void addModule(QString);
    void addFunc(QString, QString);

private:
    TcpSession *m_session;
    QMap<QString, int> m_pageMappper;
    QMap<QString, MenuButton*> m_moduleMapper;

//UI
private:
    QWidget *u_topNavBar;
    QStackedWidget *u_pages;


    void initUI();
private slots:
    void SlotSetPage(QString);

signals:

};

#endif // MENU_H
