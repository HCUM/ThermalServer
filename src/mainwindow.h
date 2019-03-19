#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "CVImageWidget.h"
#include <QLineEdit>
#include "Settings.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "CVImageWidget.h"
#include <QCheckBox>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(CVImageWidget *imageWidget);

    QPushButton *recalibrate = NULL;
    QPushButton *exportData = NULL;
    QPushButton *recordData = NULL;
    QCheckBox *horizontalBox = NULL;

    QTimer *timer = NULL;


    signals:
    void minValueChanged(int minValue);
    void maxValueChanged(int maxValue);


public slots:
    void setMaxChangeValue(float value);
    void checkboxChanged(int value);

private slots:
    void minEditingFinished();
    void maxEditingFinished();
    void record();

private:
    bool recording = false;
    void startRecording();

    QLabel* label1;
    QLineEdit *minValueText;
    QLineEdit *maxValueText;
};

#endif // MAINWINDOW_H
