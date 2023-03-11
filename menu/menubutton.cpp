#include "menubutton.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QMenu>
#include <QWidgetAction>

MenuButton::MenuButton(QWidget *parent)
    :QPushButton(parent)
{
    QMenu *menu = new QMenu(this);
    menu->setMinimumWidth(this->width());
    this->setMenu(menu);
}

void MenuButton::AddAction(QString lable)
{
    QWidgetAction *act = new QWidgetAction(this);
    QPushButton *btn = new QPushButton(this);
    btn->setText(lable);
    act->setDefaultWidget(btn);
    act->setCheckable(true);
    this->menu()->addAction(act);

    connect(btn, &QPushButton::clicked, this, &MenuButton::SlotChangePage);

}

void MenuButton::mouseMoveEvent(QMouseEvent *event)
{
    const QRect customBtnRect = this->geometry();
    const QPoint mousePos = event->pos() + customBtnRect.topLeft();

    if (customBtnRect.contains(mousePos))
    {
        if (this->menu())
        {
            this->menu()->popup(this->mapToGlobal(QPoint(0, 0)));
        }
    }
}

void MenuButton::resizeEvent(QResizeEvent *event)
{
    this->menu()->setMinimumWidth(this->width());
    QPushButton::resizeEvent(event);
}

void MenuButton::SlotChangePage()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    this->menu()->hide();
    emit changedPage(btn->text());
}
