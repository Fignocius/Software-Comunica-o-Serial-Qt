#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingdialog.h"
#include"qmainwindow.h"
#include"qwidget.h"
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QDateTime>
#include <QString>

//construtor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

    ui->setupUi(this);
    ui->lcdNumber->display("--,--");
    serialBuffer = ""; //QString
    makePlot();
    timer = new QTimer(this); //Cria novo objeto do tipo string
    serial = new QSerialPort(this); //cria novo objeto do tipo QSeriaPort;
    settings = new SettingsDialog(this);// cria novo objeto do tipo configuração settings
    initConnections();
    connect(serial, SIGNAL(readyRead()), this, SLOT(readDataSerial()));// conecta o evento readyRead com readDataSerial
    // connect(timer, SIGNAL(timeout()), this, SLOT(realTimeDataSlot()));
   // timer->start(0);
}
//destrutor
MainWindow::~MainWindow()
{
    delete ui;
}

//Metodo abre porta serial para comunicação
void MainWindow::openSerialPort(){
       SettingsDialog::Settings p = settings->settings();
       serial->setPortName(p.name);
       serial->setBaudRate(p.baudRate);
       serial->setDataBits(p.dataBits);
       serial->setParity(p.parity);
       serial->setStopBits(p.stopBits);
       serial->setFlowControl(p.flowControl);
       if (serial->open(QIODevice::ReadWrite)) {
    //           console->setEnabled(true);
    //           console->setLocalEchoEnabled(p.localEchoEnabled);
               ui->actionConnect->setEnabled(false);
               ui->actionDisconnect->setEnabled(true);
               ui->actionConfigure->setEnabled(false);
               ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
       } else {
           QMessageBox::critical(this, tr("Error"), serial->errorString());

           ui->statusBar->showMessage(tr("Open error"));
       }
   }

//Metodo fecha porta serial

void MainWindow::closeSerialPort(){

    if (serial->isOpen()) //verifica se conexao esta aberta
       serial->close(); // fecha conexão
      // console->setEnabled(false);
       ui->actionConnect->setEnabled(true);
       ui->actionDisconnect->setEnabled(false);
       ui->actionConfigure->setEnabled(true);
       ui->statusBar->showMessage(tr("Disconnected"));
}


//Metodo sobre o programa

void MainWindow::about(){

    QMessageBox::about(this, tr("About Voltimeter v2.0"),
                           tr("The <b>Simple voltimeter serial</b> \n \r Developed By Diogo Meira Trindade -"
                              "Digital Processing Signals. "
                              "using Qt, with a menu bar, toolbars, QCustomPlot, and a status bar."));
}

//Metodo de leitura da porta serial
void MainWindow::readDataSerial()
{
    QStringList bufferSplit = serialBuffer.split("\n");
    if(bufferSplit.length() < 3){
          data = serial->readAll();
          serialBuffer += QString::fromUtf8(data.constData());
      }else{
          MainWindow::updateTela(bufferSplit[1]);
          MainWindow::realTimeDataSlot(bufferSplit[1]);
          serialBuffer= "";
      }

}

//Metodo inicia conexoes interface e sistema
void MainWindow::initConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
   // connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}


//Metodo de plotagem do grafico
void MainWindow::makePlot(){

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    ui->customPlot->graph(0)->setAntialiasedFill(false);
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(0)->setChannelFillGraph(ui->customPlot->graph(1));

    ui->customPlot->addGraph(); // blue dot
    ui->customPlot->graph(2)->setPen(QPen(Qt::blue));
    ui->customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->customPlot->addGraph(); // red dot
    ui->customPlot->graph(3)->setPen(QPen(Qt::red));
    ui->customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setAutoTickStep(false);
    ui->customPlot->xAxis->setTickStep(2);
    ui->customPlot->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // generate some data:
}

//Metodo para amostragem do valor para o grafico

void MainWindow::realTimeDataSlot(const QString value){
     // calculate two new data points:
     double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
     static double lastPointKey = 0;
     if (key-lastPointKey > 0.01) // at most add point every 10 ms
     {
       double value0 = value.toDouble();//qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
      // double value1 = 3;//qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
       // add data to lines:
       ui->customPlot->graph(0)->addData(key, value0);
     //  ui->customPlot->graph(1)->addData(key, value1);
       // set data of dots:
       ui->customPlot->graph(2)->clearData();
       ui->customPlot->graph(2)->addData(key, value0);
       ui->customPlot->graph(3)->clearData();
     //  ui->customPlot->graph(3)->addData(key, value1);
       // remove data of lines that's outside visible range:
       ui->customPlot->graph(0)->removeDataBefore(key-8);
       ui->customPlot->graph(1)->removeDataBefore(key-8);
       // rescale value (vertical) axis to fit the current data:
       ui->customPlot->graph(0)->rescaleValueAxis();
       ui->customPlot->graph(1)->rescaleValueAxis(true);
       lastPointKey = key;
     }
     // make key axis range scroll with the data (at a constant range size of 8):
     ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
     ui->customPlot->replot();

     // calculate frames per second:
     static double lastFpsKey;
     static int frameCount;
     ++frameCount;
     if (key-lastFpsKey > 2) // average fps over 2 seconds
     {
      ui->statusBar->showMessage(
             QString("%1 FPS, Total Data points: %2")
             .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
             .arg(ui->customPlot->graph(0)->data()->count()+ui->customPlot->graph(1)->data()->count())
             , 0);
       lastFpsKey = key;
       frameCount = 0;
     }
 }
//Atualizar displays

void MainWindow::updateTela(const QString sensor){
    //  console->putData(data);
    ui->progressBar->setValue(sensor.toFloat());
    ui->lcdNumber->display(sensor);
 //   ui->textBrowser->insertPlainText(QString(data));
}
