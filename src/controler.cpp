#include "controler.h"

#include <QTextCodec>
#include <QCoreApplication>
#include <QThread>

Controler::Controler(QObject *parent) :
    QObject(parent)
{
    initCodec();
    initSettings();
    initAutoRun();
    initDeamon();
}

void Controler::start(bool deamon)
{
    this->mainWindow = new MainWindow();
    mainWindow->bindDisplaySettings(this->displaySettings);
    mainWindow->bindSettings(this->settings);
    mainWindow->initData();
    if (!deamon) {
        mainWindow->show();
    }
    QObject::connect(this->mainWindow, SIGNAL(startDeamon()), this->deamon, SLOT(start()));
    QObject::connect(this->mainWindow, SIGNAL(stopDeamon()), this->deamon, SLOT(stop()));
    QObject::connect(this->mainWindow, SIGNAL(updateAutoRun()), this, SLOT(updateAutoRun()));

    this->tray = new Tray();
    tray->show();
    QObject::connect(tray, SIGNAL(exitClicked()), this, SIGNAL(quit()));
    QObject::connect(tray, SIGNAL(showOrHideClicked()), mainWindow, SLOT(showOrHide()));

    if (deamon) {
        QTimer::singleShot(0, this->mainWindow, SLOT(deamonRun()));
    }
}

void Controler::initCodec()
{
    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
}

void Controler::initSettings()
{
    this->displaySettings = new DisplaySettings();
    this->settings = new QSettings("Malash", "DisplayLocker");
    settings->beginGroup("Info");
    settings->setValue("Version", settings->value("Version", "0.2.0"));
    settings->endGroup();

    settings->beginGroup("Ui");
    settings->setValue("AutoRun", settings->value("AutoRun", true));
    settings->setValue("ShowTray", settings->value("ShowTray", true));
    settings->endGroup();

    settings->beginGroup("Deamon");
    settings->setValue("mSec", settings->value("msec", 1000));
    settings->endGroup();

    settings->beginGroup("DisplaySettings");
    settings->setValue("PelsHeight", settings->value("PelsHeight", displaySettings->getPelsHeight()));
    settings->setValue("PelsWidth", settings->value("PelsWidth", displaySettings->getPelsWidth()));
    settings->setValue("BitsPerPel", settings->value("BitsPerPel", displaySettings->getBitsPerPel()));
    settings->setValue("DisplayFrequency", settings->value("DisplayFrequency", displaySettings->getDisplayFrequency()));
    settings->endGroup();

    settings->sync();
}

void Controler::initAutoRun()
{
    updateAutoRun();
}

void Controler::initDeamon()
{
    this->deamon = new Deamon();
    deamonThread = new QThread(this);
    deamon->moveToThread(deamonThread);
    deamon->bindDisplaySettings(this->displaySettings);
    deamon->bindSettings(this->settings);
    deamonThread->start();
}

void Controler::updateAutoRun()
{
    settings->beginGroup("Ui");
    bool autostart = settings->value("AutoRun").toBool();
    settings->endGroup();
    //QSettings *reg = new QSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QSettings *reg = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    //qDebug("%s", reg->value("DisplayLocker").toString(), "NULL");
    if (autostart) {
        QString str = QCoreApplication::arguments().at(0);
        str = str.replace("/", "\\");
        str += " -d";
        if (reg->value("DisplayLocker").toString().compare(str) != 0) {
            qDebug("updateAutorun()");
            reg->setValue("DisplayLocker", str);
        }
    } else {
        reg->remove("DisplayLocker");
    }
    reg->sync();
}
