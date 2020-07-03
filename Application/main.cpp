#include "Application.h"
#include <QtWidgets/QApplication>
#include <QSurfaceFormat>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Application w;

    //QString file_name = QFileDialog::getOpenFileName(NULL, "打开GLTF文件", ".", "*.gltf *.glb");
    //file_name.toStdWString();

    
    QSurfaceFormat format;
    format.setVersion(4, 6);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    //menuOpen->addAction()

    w.setWindowState(Qt::WindowState::WindowMaximized);

    w.show();
    return a.exec();
}
