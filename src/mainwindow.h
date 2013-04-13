#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "displaysettings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void bindDisplaySettings(DisplaySettings *displaySettings);
    void bindSettings(QSettings *settings);
    void initData();
    void changeEvent(QEvent *event);
    ~MainWindow();

protected:
    void updateSettings();
    
private:
    Ui::MainWindow *ui;
    DisplaySettings *displaySettings;
    QSettings *settings;
    QTimer *timerInfo;

signals:
    void startDeamon();
    void stopDeamon();
    void updateAutoRun();

public slots:
    void about();
    void apply();
    void lock(bool clicked);
    void autoRun(bool autorun);
    void settingsCellActivated(int row, int column);
    void showOrHide();
    void deamonRun();
    void updateInfo();
};

#endif // MAINWINDOW_H
