#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "CVImageWidget.h"
#include <QLineEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     MainWindow(CVImageWidget* imageWidget);

signals:
    void minValueChanged(int minValue);
    void maxValueChanged(int maxValue);


public slots:
    void setMaxChangeValue(float value);

private slots:
    void minEditingFinished();
    void maxEditingFinished();

private:
    QLabel* label1;
    QLineEdit *minValueText;
    QLineEdit *maxValueText;
};

#endif // MAINWINDOW_H
