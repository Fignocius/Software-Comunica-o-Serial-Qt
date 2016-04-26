#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>

QT_USE_NAMESPACE

namespace Ui {
class SettingsDialog;
}

class QIntValidator;


class SettingsDialog : public QDialog{
    Q_OBJECT

    public:
        struct Settings {
            QString name;
            qint32 baudRate;
            QString stringBaudRate;
            QSerialPort::DataBits dataBits;
            QString stringDataBits;
            QSerialPort::Parity parity;
            QString stringParity;
            QSerialPort::StopBits stopBits;
            QString stringStopBits;
            QSerialPort::FlowControl flowControl;
            QString stringFlowControl;
            bool localEchoEnabled;
        };

        explicit SettingsDialog(QWidget *parent = 0);
        ~SettingsDialog();

        Settings settings() const;

    private slots:
        void showPortInfo(int idx);
        void apply();
        void checkCustomBaudRatePolicy(int idx);
        void checkCustomDevicePathPolicy(int idx);

    private:
        void fillPortsParameters();
        void fillPortsInfo();
        void updateSettings();

    private:
        Ui::SettingsDialog *ui;
        Settings currentSettings;
        QIntValidator *intValidator;
};

#endif // SETTINGDIALOG_H
