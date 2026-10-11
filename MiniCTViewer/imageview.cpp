#include "imageview.h"

#include <QGraphicsScene>
#include <QWheelEvent>
#include <cmath>

ImageView::ImageView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::NoAnchor);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

bool ImageView::zoomBy(double factor)
{
    if (!scene() || scene()->items().isEmpty()) {
        return false;
    }

    double nextScale = transform().m11() * factor;

    if (nextScale < 0.01 || nextScale > 20.0) {
        return false;
    }

    scale(factor, factor);
    return true;
}

void ImageView::zoomAtCenter(double factor)
{
    QPoint centerPosition = viewport()->rect().center();
    QPointF before = mapToScene(centerPosition);

    if(!zoomBy(factor)) return;

    QPointF after = mapToScene(centerPosition);
    QPointF offset = after - before;

    translate(offset.x(), offset.y());
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if (!scene() || scene()->items().isEmpty()) {
        event->ignore();
        return;
    }

    int delta = event->angleDelta().y();

    if (delta == 0) {
        event->ignore();
        return;
    }

    QPoint mousePosition = event->position().toPoint();
    QPointF before = mapToScene(mousePosition);

    double factor = std::pow(1.25, delta / 120.0);

    if (!zoomBy(factor)) {
        event->accept();
        return;
    }

    QPointF after = mapToScene(mousePosition);
    QPointF offset = after - before;

    translate(offset.x(), offset.y());

    event->accept();
}