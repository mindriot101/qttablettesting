#ifndef TABLETAPPLICATION_H_
#define TABLETAPPLICATION_H_

#include <QApplication>

class TabletCanvas;

class TabletApplication : public QApplication {
  Q_OBJECT
public:
  using QApplication::QApplication;

  bool event(QEvent *event) override;
  void setCanvas(TabletCanvas *canvas) { m_canvas = canvas; }

private:
  TabletCanvas *m_canvas = nullptr;
};

#endif // TABLETAPPLICATION_H_
