#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidgetSettings->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidgetSettings->horizontalHeader()->setStretchLastSection(true);
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    QObject::connect(ui->actionAutoRun, SIGNAL(toggled(bool)), this, SLOT(autoRun(bool)));
    this->timerInfo = new QTimer();
    QObject::connect(timerInfo, SIGNAL(timeout()), this, SLOT(updateInfo()));
    timerInfo->start(3000);
    QTimer::singleShot(0, this, SLOT(updateInfo()));
}

void MainWindow::bindDisplaySettings(DisplaySettings *displaySettings)
{
    this->displaySettings = displaySettings;
}

void MainWindow::bindSettings(QSettings *settings)
{
    this->settings = settings;
    this->settings->beginGroup("Ui");
    ui->actionAutoRun->setChecked(this->settings->value("AutoRun").toBool());
    this->settings->endGroup();
}

void MainWindow::initData()
{
    int count = displaySettings->count();
    ui->tableWidgetSettings->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetSettings->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetSettings->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetSettings->setRowCount(count);
    for (int i = 0; i < count; i++) {
        ui->tableWidgetSettings->setItem(i, 0, new QTableWidgetItem(QString::number(displaySettings->getPelsWidth(i))));
        ui->tableWidgetSettings->setItem(i, 1, new QTableWidgetItem(QString::number(displaySettings->getPelsHeight(i))));
        ui->tableWidgetSettings->setItem(i, 2, new QTableWidgetItem(QString::number(displaySettings->getBitsPerPel(i))));
        ui->tableWidgetSettings->setItem(i, 3, new QTableWidgetItem(QString::number(displaySettings->getDisplayFrequency(i))));
    }
    ui->tableWidgetSettings->setCurrentCell(displaySettings->getNowId(), 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSettings()
{
    int row = ui->tableWidgetSettings->currentRow();
    settings->beginGroup("DisplaySettings");
    settings->setValue("PelsWidth", ui->tableWidgetSettings->item(row, 0)->text().toInt());
    settings->setValue("PelsHeight", ui->tableWidgetSettings->item(row, 1)->text().toInt());
    settings->setValue("BitsPerPel", ui->tableWidgetSettings->item(row, 2)->text().toInt());
    settings->setValue("DisplayFrequency", ui->tableWidgetSettings->item(row, 3)->text().toInt());
    settings->endGroup();
    settings->sync();
}

void MainWindow::apply()
{
    int lastId = displaySettings->getNowId();
    int id = ui->tableWidgetSettings->currentRow();
    displaySettings->applyDisplaySetting(id);
    qDebug(qPrintable(QString::number(id)));

    QMessageBox *confirm = new QMessageBox(QMessageBox::Warning, tr("显示设置"), tr("是否保留这些设置？15秒后恢复原设置"), QMessageBox::Ok | QMessageBox::Cancel);
//    QTimer *timer = new QTimer(confirm);
//    connect(timer, SIGNAL(timeout()), confirm, SLOT(reject()));
//    timer->start(3000, true);
    QTimer::singleShot(15000, confirm->button(QMessageBox::Cancel), SLOT(click()));
    //QMessageBox::StandardButton confirm = QMessageBox::warning(NULL,  QMessageBox::No);
    int confirmInfo = confirm->exec();
    if (confirmInfo == QMessageBox::Ok) {
        ui->pushButtonApply->setEnabled(false);
        ui->pushButtonLock->setEnabled(true);
    } else if (confirmInfo == QMessageBox::Cancel) {
        displaySettings->applyDisplaySetting(lastId);
        ui->pushButtonLock->setEnabled(false);
    };
    QTimer::singleShot(0, this, SLOT(updateInfo()));
}

void MainWindow::lock(bool clicked)
{
    if (clicked) {
        this->updateSettings();
        emit startDeamon();
        ui->pushButtonLock->setText(tr("已锁定"));
        ui->tableWidgetSettings->setEnabled(false);
    } else {
        emit stopDeamon();
        ui->pushButtonLock->setText(tr("锁定"));
        ui->tableWidgetSettings->setEnabled(true);
    }
}

void MainWindow::autoRun(bool autorun)
{
    this->settings->beginGroup("Ui");
    settings->setValue("AutoRun", autorun);
    settings->endGroup();
    emit this->updateAutoRun();
}

void MainWindow::settingsCellActivated(int row, int column)
{
    if (row == displaySettings->getNowId()) {
        ui->pushButtonApply->setEnabled(false);
        ui->pushButtonLock->setEnabled(true);
    } else {
        ui->pushButtonApply->setEnabled(true);
        ui->pushButtonLock->setEnabled(false);
    }
}

void MainWindow::showOrHide()
{
    this->setVisible(!this->isVisible());
}

void MainWindow::deamonRun()
{
    QTimer::singleShot(0, ui->pushButtonLock, SLOT(click()));
}

void MainWindow::updateInfo()
{
    QString str = tr("当前屏幕设置：<br />"
                     "分辨率 %1x%2<br />"
                     "颜色位数 %3<br />"
                     "刷新率 %4");
    str = str.arg(this->displaySettings->getPelsWidth());
    str = str.arg(this->displaySettings->getPelsHeight());
    str = str.arg(this->displaySettings->getBitsPerPel());
    str = str.arg(this->displaySettings->getDisplayFrequency());
    ui->labelInfoNow->setText(str);
}

void MainWindow::about()
{
    QMessageBox::about(NULL, tr("关于"), tr("<h1>Display Locker</h1><h2>v0.2.0</h2><a href=\"http://malash.me/project/display-locker/\">Display Locker</a> is powered by <a href=\"http://malash.me/\">Malash</a>"));
}

void MainWindow::changeEvent(QEvent* event){
    if(event->type()==QEvent::WindowStateChange){
        if(windowState() & Qt::WindowMinimized){
           QTimer::singleShot(0, this, SLOT(hide()));
        }
    }
    QMainWindow::changeEvent(event);
}
