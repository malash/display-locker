#ifndef TRAY_H
#define TRAY_H

#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>

class Tray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit Tray(QObject *parent = 0);
    QMenu *menuMain;
    QAction *actionExit;
    QAction *actionShowOrHide;

protected:
    QIcon icon;
    QSystemTrayIcon *trayIcon;
    
signals:
    void exitClicked();
    void showOrHideClicked();

public slots:
    void onActivated(QSystemTrayIcon::ActivationReason activationReason);
    
};

#endif // TRAY_H
