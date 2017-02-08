#pragma once
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include <QMouseEvent>


class CVImageWidget : public QWidget
{
Q_OBJECT

public:
    explicit CVImageWidget(QWidget *parent = 0) : QWidget(parent) {}

    QSize sizeHint() const { return _qimage.size()*2; }
    QSize minimumSizeHint() const { return _qimage.size(); }

public slots:

    void showImage(const cv::Mat& image) {
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

        repaint();
    }

signals:
    void valueChanged(QPoint start, QPoint end);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent* /*event*/) {
        // Display the image
        QPainter painter(this);
        painter.drawImage(QPoint(0,0), _qimageS);
        drawLineTo();
        painter.end();
    }

    QImage _qimage;
    QImage _qimageS;

    cv::Mat _tmp;

    void drawLineTo();

    QPoint lastPoint;
    QPoint firstPoint;
    bool scribbling;
};
