#include "deamon.h"

Deamon::Deamon(QObject *parent) :
    QObject(parent)
{
    this->timer = new QTimer(this);
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(update()));
}

void Deamon::bindDisplaySettings(DisplaySettings *displaySettings)
{
    this->displaySettings = displaySettings;
}

void Deamon::bindSettings(QSettings *settings)
{
    this->settings = settings;
}

void Deamon::start()
{
    settings->beginGroup("DisplaySettings");
    int pelsHeight = settings->value("PelsHeight", displaySettings->getPelsHeight()).toInt();
    int pelsWidth = settings->value("PelsWidth", displaySettings->getPelsWidth()).toInt();
    int bitsPerPel = settings->value("BitsPerPel", displaySettings->getBitsPerPel()).toInt();
    int displayFrequency = settings->value("DisplayFrequency", displaySettings->getDisplayFrequency()).toInt();
    settings->endGroup();

    this->id = this->displaySettings->getIdByInfo(pelsWidth, pelsHeight, bitsPerPel, displayFrequency);
    id = id != -1 ? id : displaySettings->getNowId();

    settings->beginGroup("Deamon");
    qDebug("msec:%d", this->settings->value("msec").toInt());
    this->timer->start(this->settings->value("msec").toInt());
    settings->endGroup();
}

void Deamon::stop()
{
    this->timer->stop();
}

void Deamon::update()
{
    qDebug(qPrintable(QString("update():now=%1,locked=%2").arg(this->displaySettings->getNowId()).arg(this->id)));
    if (id != displaySettings->getNowId()) {
        displaySettings->applyDisplaySetting(id);
    }
}
