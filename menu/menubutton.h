#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>

class MenuButton : public QPushButton
{
    Q_OBJECT
public:
    MenuButton(QWidget *parent = nullptr);

    void AddAction(QString);
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void SlotChangePage();
signals:
    void changedPage(QString);
};

#endif // MENUBUTTON_H
