#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include "mainwindow.h"
#include <QLineEdit>

#include "CVImageWidget.h"

MainWindow::MainWindow(CVImageWidget* imageWidget )
{

    // Create the MainGrid
    QGridLayout* mainGrid = new QGridLayout();

    //imageWidget = new CVImageWidget();
    mainGrid->addWidget((QWidget*) imageWidget,1,1);
    ((QWidget*) imageWidget)->show();


    QWidget *w = new QWidget;

    QPushButton *setRangeButton = new QPushButton("Set Range");
    QPushButton *button2 = new QPushButton("Two");
    QPushButton *button3 = new QPushButton("Three");
    QPushButton *button4 = new QPushButton("Four");
    QPushButton *button5 = new QPushButton("Five");

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(setRangeButton);
    layout->addWidget(button2);
    layout->addWidget(button3);
    layout->addWidget(button4);
    layout->addWidget(button5);



    w->setLayout(layout);
    w->show();

    mainGrid->addWidget(w,2,2);


    QWidget *l = new QWidget;

    QVBoxLayout *labelLayout = new QVBoxLayout;
    label1 = new QLabel("Temp change: nan");
    labelLayout -> addWidget(label1);


    //min value
    QLabel *minValueLabel = new QLabel("Minimal Value: ");
    minValueText = new QLineEdit();
    minValueText->setMaximumWidth(50);
    minValueText->setText("15");

    QObject::connect(minValueText, SIGNAL(editingFinished()),this, SLOT(minEditingFinished()));

    QHBoxLayout *minValueLayout = new QHBoxLayout;
    minValueLayout->addWidget(minValueLabel);
    minValueLayout->addWidget(minValueText);

    //max value
    QLabel *maxValueLabel = new QLabel("Maximal Value: ");
    maxValueText = new QLineEdit();
    maxValueText->setMaximumWidth(50);
    maxValueText->setText("40");

    QHBoxLayout *maxValueLayout = new QHBoxLayout;
    maxValueLayout->addWidget(maxValueLabel);
    maxValueLayout->addWidget(maxValueText);


    labelLayout->addItem(minValueLayout);
    labelLayout->addItem(maxValueLayout);


//    QWidget *t = new QWidget;
//    t->setLayout(minValueLayout);

    //labelLayout->addWidget(t);

    l->setLayout(labelLayout);
    l->show();

    mainGrid->addWidget(l,1,2);


    setCentralWidget(new QWidget);
    centralWidget()->setLayout(mainGrid);



    show();


    QObject::connect(minValueText, SIGNAL(editingFinished()),this, SLOT(minEditingFinished()));
    QObject::connect(maxValueText, SIGNAL(editingFinished()),this, SLOT(maxEditingFinished()));

}

void MainWindow::setMaxChangeValue(float value) {
    QString s = "Temp change: ";
    s.append(QString::number(value));
    label1->setText(s);
}

void MainWindow::minEditingFinished() {
    int min = minValueText->text().toInt();
    std::cout << "MainWindow::minEditingFinished, new min: " << min << std::endl;
    emit minValueChanged(min);
}

void MainWindow::maxEditingFinished() {
    int max = maxValueText->text().toInt();
    std::cout << "MainWindow::maxEditingFinished, new max: " << max << std::endl;
    emit maxValueChanged(max);
}

