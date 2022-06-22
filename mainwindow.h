#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TabletCanvas;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(TabletCanvas *canvas);
    ~MainWindow();

private:
    TabletCanvas *m_canvas;
};
#endif // MAINWINDOW_H
