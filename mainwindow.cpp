#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataTimer = new QTimer;

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    //    ui->customPlot->addGraph(); // red line
    //    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    //    ui->customPlot->yAxis->setRange(-1.2, 1.2);
    ui->customPlot->yAxis->setRange(0, 5.3);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect( ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect( ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
//    dataTimer->setInterval(1000);
    dataTimer->start();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::realtimeDataSlot()
{
//        qDebug() << QRandomGenerator::global()->bounded(5.3);
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.002) // at most add point every 2 ms
    {
        // add data to lines:
        ui->customPlot->graph(0)->addData(key, QRandomGenerator::global()->bounded(5.3)-2.5);
        //        ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));

        lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot->replot();

    ui->statusbar->showMessage(
                QString("Total Data points: %1")
                .arg(ui->customPlot->graph(0)->data()->size())
                , 0);

}

