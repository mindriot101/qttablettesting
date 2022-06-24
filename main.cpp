#include "mainwindow.h"

#include "tabletapplication.h"
#include "tabletcanvas.h"

int main(int argc, char *argv[]) {
    TabletApplication app(argc, argv);
    TabletCanvas *canvas = new TabletCanvas;
    app.setCanvas(canvas);

    MainWindow mainWindow(canvas);
    mainWindow.resize(500, 500);
    mainWindow.show();
    return app.exec();
}
