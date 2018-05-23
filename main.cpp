#include <QApplication>
#include <memory>

#include "Window.h"
#include "MainWindow.h"


int main(int argc, char **argv) {
    QApplication application(argc, argv);

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapInterval(100);
    format.setVersion(3, 3);

    MainWindow window;
    window.show();

    return application.exec();


}