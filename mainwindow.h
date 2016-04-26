#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "settingdialog.h"
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QDateTime>
#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>
#include <QCoreApplication>
#include <QString>

QT_USE_NAMESPACE


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

 void openSerialPort();
 void closeSerialPort();
 void about();
 void readDataSerial();
 void makePlot();


private slots:
 void updateTela(const QString);
 void realTimeDataSlot(const QString);
private:
 void initConnections();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    SettingsDialog *settings;
    QTimer  *timer;
    QByteArray data;
    QString serialBuffer;
};

#endif // MAINWINDOW_H
