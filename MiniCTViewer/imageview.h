#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>

class QWheelEvent;

class ImageView : public QGraphicsView
{
public:
    ImageView(QGraphicsScene *scene, QWidget *parent = nullptr);

    bool zoomBy(double factor);

    void zoomAtCenter(double factor);

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // IMAGEVIEW_H
