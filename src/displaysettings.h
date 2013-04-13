#ifndef DISPLAYSETTINGS_H
#define DISPLAYSETTINGS_H

#include <QObject>
#include <QMap>
#include <windows.h>

class DisplaySettings : public QObject
{
    Q_OBJECT

public:
    explicit DisplaySettings(QObject *parent = 0);
    int count();
    int getNowId();
    int getIdByInfo(int pelsWidth, int pelsHeight, int bitsPerPel, int displayFrequency);
    int getPelsWidth();
    int getPelsHeight();
    int getBitsPerPel();
    int getDisplayFrequency();
    int getPelsWidth(int i);
    int getPelsHeight(int i);
    int getBitsPerPel(int i);
    int getDisplayFrequency(int i);
    bool applyDisplaySetting(int id);

private:
    QMap<int, DEVMODE> *devModes;
};

#endif // DISPLAYSETTINGS_H
