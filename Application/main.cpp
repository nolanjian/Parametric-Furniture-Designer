#include <Application/Application.h>
#include <QtWidgets/QApplication>
#include <QSurfaceFormat>
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

	QSurfaceFormat format;
	format.setVersion(4, 6);
	format.setDepthBufferSize(24);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    PFD::GUI::Application w;

    if (w.Initialize())
    {
		w.setWindowState(Qt::WindowState::WindowMaximized);
		w.show();
        return a.exec();
    }
    return 0;
}
