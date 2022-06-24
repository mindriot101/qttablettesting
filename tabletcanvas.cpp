#include "tabletcanvas.h"

#include <QCoreApplication>
#include <QPainter>
#include <QPointingDevice>
#include <QTabletEvent>
#include <QtDebug>
#include <qnamespace.h>

TabletCanvas::TabletCanvas()
    : QWidget(nullptr), m_brush(m_color), m_pen(m_brush, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) {
    resize(500, 500);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_TabletTracking);
}

void TabletCanvas::updateCursor(const QTabletEvent *event) {
    QCursor cursor;
    if (event->type() != QEvent::TabletLeaveProximity) {
        cursor = QCursor(QPixmap(":/images/cursor-pencil.png"), 0, 0);
    }
    setCursor(cursor);
}

// like the update function of a game engine
void TabletCanvas::tabletEvent(QTabletEvent *event) {
    switch (event->type()) {
    case QEvent::TabletPress:
        switch (event->button()) {
        case Qt::MouseButton::LeftButton:
            if (!m_deviceDown) {
                m_deviceDown = true;
                lastPoint.pos = event->position();
                lastPoint.pressure = event->pressure();
                lastPoint.rotation = event->rotation();
            }
            break;
        default:
            break;
        }
        break;
    case QEvent::TabletMove:
        if (m_deviceDown) {
            qDebug() << "Device down";
            updateBrush(event);
            QPainter painter(&m_pixmap);
            /* painter.setTransform(m_transform); */
            paintPixmap(painter, event);
            lastPoint.pos = event->position();
            lastPoint.pressure = event->pressure();
            lastPoint.rotation = event->rotation();
        } else {
            if (event->buttons() & Qt::MouseButton::MiddleButton) {
                QTransform transform = m_transform.translate(0.1, 0.1);
                m_transform = transform;
            }
        }
        break;

    case QEvent::TabletRelease:
        if (m_deviceDown && event->buttons() == Qt::NoButton) {
            m_deviceDown = false;
        }
        update();

        // the pen has been removed, so add one to the line index
        m_line_index++;
        break;
    default:
        break;
    }

    event->accept();
}

void TabletCanvas::initPixmap() {
    qreal dpr = devicePixelRatio();
    QPixmap newPixmap = QPixmap(qRound(width() * dpr), qRound(height() * dpr));
    newPixmap.setDevicePixelRatio(dpr);
    newPixmap.fill(Qt::white);
    QPainter painter(&newPixmap);
    if (!m_pixmap.isNull())
        painter.drawPixmap(0, 0, m_pixmap);
    painter.end();
    m_pixmap = newPixmap;
}

void TabletCanvas::paintEvent(QPaintEvent *event) {
    if (m_pixmap.isNull())
        initPixmap();
    QPainter painter(this);
    /* painter.setTransform(m_transform); */
    QRect pixmapPortion =
        QRect(event->rect().topLeft() * devicePixelRatio(), event->rect().size() * devicePixelRatio());
    painter.drawPixmap(event->rect().topLeft(), m_pixmap, pixmapPortion);
}

void TabletCanvas::paintPixmap(QPainter &painter, QTabletEvent *event) {
    static qreal maxPenRadius = pressureToWidth(1.0);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(m_pen);
    painter.drawLine(lastPoint.pos, event->position());
    /* m_history[m_line_index].append(lastPoint.pos); */
    update(QRect(lastPoint.pos.toPoint(), event->position().toPoint())
               .normalized()
               .adjusted(-maxPenRadius, -maxPenRadius, maxPenRadius, maxPenRadius));
}

void TabletCanvas::updateBrush(const QTabletEvent *event) {
    m_color.setAlpha(255);
    m_pen.setWidthF(pressureToWidth(event->pressure()));
    m_brush.setColor(m_color);
    m_pen.setColor(m_color);
}

qreal TabletCanvas::pressureToWidth(qreal pressure) { return pressure * 10 + 1; }

void TabletCanvas::resizeEvent(QResizeEvent *) { initPixmap(); }
