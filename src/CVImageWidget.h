#pragma once
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include <QMouseEvent>
#include <iostream>
#include "Settings.h"


class CVImageWidget : public QWidget
{
Q_OBJECT

public:
    explicit CVImageWidget(QWidget *parent = 0) : QWidget(parent) {}

    QSize sizeHint() const { return _qimage.size()*2; }
    QSize minimumSizeHint() const { return _qimage.size(); }

    QPoint lastPoint;
    QPoint firstPoint;

public slots:
    // void setThermalImage(unsigned short *img);
    void showImage(const cv::Mat& image);


signals:
    void valueChanged(QPoint start, QPoint end);

protected:
    std::string TAG = "[CVImageWidget]: ";

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
    bool scribbling;
};
