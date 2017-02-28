#include "CVImageWidget.h"



void CVImageWidget::showImage(const cv::Mat& image) {
    // Convert the image to the RGB888 format
    switch (image.type()) {
        case CV_8UC1:
            cvtColor(image, _tmp, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(image, _tmp, CV_BGR2RGB);
            break;
    }

    // QImage needs the data to be stored continuously in memory
    assert(_tmp.isContinuous());
    // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
    // (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
    // has three bytes.
    _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);//.scaled(image.cols*2, image.rows*2, Qt::KeepAspectRatio);


    _qimageS = _qimage.scaled(image.cols*2, image.rows*2, Qt::KeepAspectRatio);


    this->setFixedSize(image.cols*2, image.rows*2);
    update();
}

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
    try {
        QPainter painter(this);
        painter.setPen(QPen(QColor(0,255,0), 1, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.drawLine(firstPoint,lastPoint);
    } catch (...) {
            std::cout << "catched some exception while drawing line." << std::endl;
    }

}