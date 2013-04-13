#include <QtGui/QApplication>
#include <QStringListIterator>
#include "controler.h"

int main(int argc, char *argv[])
{
    QApplication *app = new QApplication(argc, argv);
    //app->setQuitOnLastWindowClosed(false);
    Controler *controller = new Controler();
    bool deamon = false;
    for (int i = 1; i < argc; i++) {
        qDebug(argv[i]);
        if (strcmp(argv[i], "-d") == 0) {
            deamon = true;
        }
    }
    QObject::connect(controller, SIGNAL(quit()), app, SLOT(quit()));
    controller->start(deamon);
    return app->exec();
}
