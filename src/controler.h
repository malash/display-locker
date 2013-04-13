#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <QSettings>
#include <QThread>

#include "mainwindow.h"
#include "displaysettings.h"
#include "deamon.h"
#include "tray.h"

class Controler : public QObject
{
    Q_OBJECT
public:
    explicit Controler(QObject *parent = 0);
    void start(bool deamon = false);

private:
    MainWindow *mainWindow;
    DisplaySettings *displaySettings;
    QSettings *settings;
    Deamon *deamon;
    QThread *deamonThread;
    Tray *tray;
    void initCodec();
    void initSettings();
    void initAutoRun();
    void initDeamon();

signals:
    void quit();
    
public slots:
    void updateAutoRun();
};

#endif // CONTROLER_H
