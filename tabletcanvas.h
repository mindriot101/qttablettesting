#ifndef TABLETCANVAS_H
#define TABLETCANVAS_H

#include <QPen>
#include <QWidget>
#include <cstdint>
#include <qtransform.h>

class TabletCanvas : public QWidget {
    Q_OBJECT
  public:
    enum Valuator {
        PressureValuator,
        TangentialPressureValuator,
        TiltValuator,
        VTiltValuator,
        HTiltValuator,
        NoValuator
    };
    Q_ENUM(Valuator);

    TabletCanvas();

    bool saveImage(const QString &file);
    bool loadImage(const QString &file);
    void clear();
    void setColor(const QColor &c) {
        if (c.isValid())
            m_color = c;
    }
    QColor color() const { return m_color; }
    void setTabletDevice(QTabletEvent *event) { updateCursor(event); }

  protected:
    void tabletEvent(QTabletEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

  private:
    void initPixmap();
    void paintPixmap(QPainter &painter, QTabletEvent *event);
    Qt::BrushStyle brushPattern(qreal value);
    static qreal pressureToWidth(qreal pressure);
    void updateBrush(const QTabletEvent *event);
    void updateCursor(const QTabletEvent *event);

    QColor m_color = Qt::red;
    QPixmap m_pixmap;
    QBrush m_brush;
    QPen m_pen;
    bool m_deviceDown = false;

    uint64_t m_line_index = 0;

    QVector<QVector<QPointF>> m_history;

    QTransform m_transform;

    struct Point {
        QPointF pos;
        qreal pressure = 0;
        qreal rotation = 0;
    } lastPoint;
};

#endif // TABLETCANVAS_H
