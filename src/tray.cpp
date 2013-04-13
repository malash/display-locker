#include "tray.h"

#include <QTimer>

Tray::Tray(QObject *parent) :
    QSystemTrayIcon(parent)
{
    Q_INIT_RESOURCE(res);
    this->setIcon(QIcon(":/icon.png"));
    menuMain = new QMenu();
    actionShowOrHide = new QAction(menuMain);
    actionShowOrHide->setText(tr("��ʾ/����"));
    menuMain->addAction(actionShowOrHide);
    actionExit = new QAction(menuMain);
    actionExit->setText(tr("�˳�"));
    menuMain->addAction(actionExit);
    setContextMenu(menuMain);

    connect(this->actionExit, SIGNAL(triggered()), this, SIGNAL(exitClicked()));
    connect(this->actionShowOrHide, SIGNAL(triggered()), this, SIGNAL(showOrHideClicked()));
    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onActivated(QSystemTrayIcon::ActivationReason)));
}

void Tray::onActivated(QSystemTrayIcon::ActivationReason activationReason)
{
    if (activationReason == QSystemTrayIcon::DoubleClick) {
        QTimer::singleShot(0, this, SIGNAL(showOrHideClicked()));
    }
}
