#include "Application.h"
#include <QtWidgets/QApplication>
#include <QLayout>

#include <Scene/OpenGLWidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Application w;

    PFD::Scene::OpenGLWidget oglWidget;

    w.layout()->addWidget(&oglWidget);

    w.show();
    return a.exec();
}
