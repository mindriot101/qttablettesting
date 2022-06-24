#include "mainwindow.h"

#include <QCoreApplication>

#include "tabletcanvas.h"

MainWindow::MainWindow(TabletCanvas *canvas) : m_canvas(canvas) {
    setWindowTitle(tr("Tablet Example"));
    setCentralWidget(m_canvas);
    QCoreApplication::setAttribute(Qt::AA_CompressHighFrequencyEvents);
}

MainWindow::~MainWindow() {}
