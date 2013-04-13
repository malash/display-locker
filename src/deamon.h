#ifndef DEAMON_H
#define DEAMON_H

#include <QObject>
#include <QTimer>
#include <QSettings>
#include "displaysettings.h"

class Deamon : public QObject
{
    Q_OBJECT
public:
    explicit Deamon(QObject *parent = 0);
    void bindDisplaySettings(DisplaySettings *displaySettings);
    void bindSettings(QSettings *settings);

private:
    QTimer *timer;
    DisplaySettings *displaySettings;
    QSettings *settings;
    int id;
    
signals:
    
public slots:
    void start();
    void stop();

protected slots:
    void update();
};

#endif // DEAMON_H
