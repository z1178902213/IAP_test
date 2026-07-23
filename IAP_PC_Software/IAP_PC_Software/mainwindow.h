#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QFile>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QTcpSocket>
#include <QTextCodec>
#include <QByteArray>
#include <QFileDialog>
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_serialOpen_pushButton_clicked();
    void readData();
    void on_serialSend_pushButton_clicked();
    void on_loaderOpenFile_pushButton_clicked();
    void on_loader_pushButton_clicked();
    void send_number_uint32(quint32 num);
    void load_file();
    void on_ethConnect_pushButton_clicked();
    void ethConnected();
    void ethReadyRead();
    void ethDisconnected();
    void ethError();
    void wifiConnected();
    void wifiReadyRead();
    void wifiDisconnected();
    void wifiError();

    void on_wifiConnect_pushButton_clicked();

    void on_netSend_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *my_serial = nullptr;
    QByteArray m_recvBuf;
    QByteArray fileBuffer;
    char loadFlag = 0;
    QTcpSocket *ethSocket;
    QTcpSocket *wifiSocket;
    QTimer timer;
};
#endif // MAINWINDOW_H
