#include "CVImageWidget.h"

void CVImageWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        firstPoint = event->pos();
        lastPoint = firstPoint;
        scribbling = true;
        std::cout << "click" << std::endl;
    }
}

void CVImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
    lastPoint = event->pos();
}

void CVImageWidget::mouseReleaseEvent(QMouseEvent *event){

    std::cout << firstPoint.x() << "," << firstPoint.y()<< std::endl;
    std::cout << lastPoint.x() << "," << lastPoint.y()<< std::endl;

    emit valueChanged(firstPoint,lastPoint);


}


void CVImageWidget::drawLineTo()
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(0,255,0), 1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    painter.drawLine(firstPoint,lastPoint);
}