#include "displaysettings.h"

DisplaySettings::DisplaySettings(QObject *parent)
{
    DEVMODE *devMode;
    BOOL modeExist;
    this->devModes = new QMap<int, DEVMODE>;
    for (int i = 0; ; i++) {
        devMode = new DEVMODE;
        memset(devMode, 0, sizeof(DEVMODE));
        modeExist = EnumDisplaySettings(NULL, i, devMode);
        if (modeExist == 0) {
            break;
        }
        qDebug(qPrintable(QString("%1: %2 %3 %4 %5")
                          .arg(QString::number(i))
                          .arg(QString::number(devMode->dmPelsWidth))
                          .arg(QString::number(devMode->dmPelsHeight))
                          .arg(QString::number(devMode->dmBitsPerPel))
                          .arg(QString::number(devMode->dmDisplayFrequency))
                          ));
        devModes->insert(i, *devMode);
    }
}

int DisplaySettings::count()
{
    return devModes->count();
}

int DisplaySettings::getNowId()
{
    return this->getIdByInfo(this->getPelsWidth(), this->getPelsHeight(), this->getBitsPerPel(), this->getDisplayFrequency());
}

int DisplaySettings::getIdByInfo(int pelsWidth, int pelsHeight, int bitsPerPel, int displayFrequency)
{
    DEVMODE devMode;
    for (QMap<int, DEVMODE>::const_iterator i = devModes->begin(); i != devModes->end(); i++) {
        devMode = i.value();
        if (devMode.dmPelsWidth == (DWORD)pelsWidth
                && devMode.dmPelsHeight == (DWORD)pelsHeight
                && devMode.dmBitsPerPel == (DWORD)bitsPerPel
                && devMode.dmDisplayFrequency == (DWORD)displayFrequency) {
            return i.key();
        }
    }
    return this->getNowId();
}

int DisplaySettings::getPelsWidth()
{
    DEVMODE *devMode = new DEVMODE;
    memset(devMode, 0, sizeof(DEVMODE));
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, devMode)) {
        return devMode->dmPelsWidth;
    }
    return 0;
}

int DisplaySettings::getPelsHeight()
{
    DEVMODE *devMode = new DEVMODE;
    memset(devMode, 0, sizeof(DEVMODE));
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, devMode)) {
        return devMode->dmPelsHeight;
    }
    return 0;
}

int DisplaySettings::getBitsPerPel()
{
    DEVMODE *devMode = new DEVMODE;
    memset(devMode, 0, sizeof(DEVMODE));
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, devMode)) {
        return devMode->dmBitsPerPel;
    }
    return 0;
}

int DisplaySettings::getDisplayFrequency()
{
    DEVMODE *devMode = new DEVMODE;
    memset(devMode, 0, sizeof(DEVMODE));
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, devMode)) {
        return devMode->dmDisplayFrequency;
    }
    return 0;
}

int DisplaySettings::getPelsWidth(int i)
{
    return devModes->find(i).value().dmPelsWidth;
}

int DisplaySettings::getPelsHeight(int i)
{
    return devModes->find(i).value().dmPelsHeight;
}

int DisplaySettings::getBitsPerPel(int i)
{
    return devModes->find(i).value().dmBitsPerPel;
}

int DisplaySettings::getDisplayFrequency(int i)
{
    return devModes->find(i).value().dmDisplayFrequency;
}

bool DisplaySettings::applyDisplaySetting(int id)
{
    DEVMODE *devMode = new DEVMODE;
    BOOL modeExist;
    modeExist = EnumDisplaySettings(NULL, id, devMode);
    //devMode->dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
    devMode->dmFields = 0xffffffff;
    qDebug(qPrintable(QString("applyDisplaySetting():%1 %2 %3 %4")
                      .arg(QString::number(devMode->dmBitsPerPel))
                      .arg(QString::number(devMode->dmPelsWidth))
                      .arg(QString::number(devMode->dmPelsHeight))
                      .arg(QString::number(devMode->dmDisplayFrequency))
                      ));
    ChangeDisplaySettings(devMode, 0);
    return true;
}
