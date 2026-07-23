#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , my_serial(new QSerialPort(this))
{
    ui->setupUi(this);

    ethSocket = new QTcpSocket(this);
    wifiSocket = new QTcpSocket(this);

    my_serial->setReadBufferSize(0);

    connect(my_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(ethSocket, &QTcpSocket::connected, this, &MainWindow::ethConnected);
    connect(ethSocket, &QTcpSocket::readyRead, this, &MainWindow::ethReadyRead);
    connect(ethSocket, &QTcpSocket::disconnected, this, &MainWindow::ethDisconnected);
    connect(ethSocket, &QTcpSocket::errorOccurred, this, &MainWindow::ethError);
    connect(wifiSocket, &QTcpSocket::connected, this, &MainWindow::wifiConnected);
    connect(wifiSocket, &QTcpSocket::readyRead, this, &MainWindow::wifiReadyRead);
    connect(wifiSocket, &QTcpSocket::disconnected, this, &MainWindow::wifiDisconnected);
    connect(wifiSocket, &QTcpSocket::errorOccurred, this, &MainWindow::wifiError);

    const QList ports = QSerialPortInfo::availablePorts();

    for(const QSerialPortInfo &port: ports){
        ui->serial_comboBox->addItem(port.portName());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_serialOpen_pushButton_clicked()
{
    if(ui->serialOpen_pushButton->isChecked()){
        my_serial->setBaudRate(ui->baudrate_comboBox->currentText().toInt());
        my_serial->setDataBits((QSerialPort::DataBits)ui->wordLength_comboBox->currentText().toInt());
        my_serial->setStopBits((QSerialPort::StopBits)ui->stop_comboBox->currentText().toInt());
        my_serial->setPortName(ui->serial_comboBox->currentText());
        my_serial->setFlowControl(QSerialPort::NoFlowControl);
        my_serial->setParity(QSerialPort::NoParity);

        qDebug() << "===== 串口配置信息 =====";
        qDebug() << "端口名:" << my_serial->portName();
        qDebug() << "波特率:" << my_serial->baudRate();
        qDebug() << "数据位:" << my_serial->dataBits();
        qDebug() << "停止位:" << my_serial->stopBits();
        qDebug() << "校验位:" << my_serial->parity();
        qDebug() << "流控模式:" << my_serial->flowControl();
        qDebug() << "是否打开:" << my_serial->isOpen();
        qDebug() << "========================";

        if (my_serial->open(QIODevice::ReadWrite)) {
            qDebug("==> 打开串口成功\n");
            ui->serialOpen_pushButton->setChecked(true);
            ui->serialOpen_pushButton->setText("停止串口");
            ui->serialSend_pushButton->setDisabled(false);
        }else{
            qDebug("==> 打开串口失败\n");
        }
    }else{
        my_serial->close();
        ui->serialOpen_pushButton->setChecked(false);
        ui->serialOpen_pushButton->setText("打开串口");
        ui->serialSend_pushButton->setDisabled(true);
    }
}

void MainWindow::readData()
{
    QByteArray chunk = my_serial->readAll();

    if(my_serial->waitForReadyRead(50))
    {
        chunk.append(my_serial->readAll());
    }

    qDebug() << "接收数据："<< chunk.toHex();
    m_recvBuf += chunk;
    int pos;
    while ((pos = m_recvBuf.indexOf('\n')) != -1)
    {
        QByteArray line = m_recvBuf.left(pos + 1);
        m_recvBuf = m_recvBuf.mid(pos + 1);

        QString text = QTextCodec::codecForName("GBK")->toUnicode(line);
        ui->serialRec_textBrowser->moveCursor(QTextCursor::End);
        ui->serialRec_textBrowser->insertPlainText(text);
        if(text.contains("烧录模式")){
            ui->loader_pushButton->setDisabled(false);
        }else{
            ui->loader_pushButton->setDisabled(true);
        }
        if(text.contains("请发送bin文件")){
            load_file();
        }
    }
}

void MainWindow::send_number_uint32(quint32 num){
    char filesize[4] = {0};
    filesize[0] = (num >> 24) & 0xFF;
    filesize[1] = (num >> 16) & 0xFF;
    filesize[2] = (num >> 8) & 0xFF;
    filesize[3] = (num >> 0);
    my_serial->write(filesize, 4);
}

void MainWindow::on_serialSend_pushButton_clicked()
{
    auto pdata = ui->serialSend_textEdit->toPlainText();
    if(ui->serialSendSrt_radioButton->isChecked()){
        QByteArray sendData = ui->serialSend_textEdit->toPlainText().toLocal8Bit().replace("\n", "\r\n");
        qDebug() << "输出数据为：" << sendData.toHex();
        my_serial->write(sendData, ui->serialSend_textEdit->toPlainText().size());
    }else{
        quint32 num = ui->serialSend_textEdit->toPlainText().toUInt();
        send_number_uint32(num);
    }
}


void MainWindow::on_loaderOpenFile_pushButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, tr("选择BIN文件"), QDir::homePath(), tr("镜像文件(*.bin);;所有文件 (*.*)"));
    if (filePath.isEmpty())
    {
        qDebug() << "用户取消选择";
        return;
    }

    // 2. 创建QFile对象
    QFile file(filePath);

    // 3. 获取文件大小
    qint64 fileSize = file.size();
    ui->loader_textBrowser->insertPlainText("\n打开文件：" + filePath+ "\n文件大小：" + QString::number(fileSize) + "B");
    qDebug() << "文件路径:" << filePath;
    qDebug() << "文件大小(字节):" << fileSize;

    // 4. 打开文件：只读模式
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "打开失败:" << file.errorString();
        return;
    }
    ui->loader_textBrowser->insertPlainText("\n打开文件成功\n");
    fileBuffer = file.readAll();
}


void MainWindow::on_loader_pushButton_clicked()
{
    int filesize = fileBuffer.size();
    send_number_uint32(filesize);
}

void MainWindow::load_file(){
    ui->loader_textBrowser->append("正在发送文件...");
    int filesize = fileBuffer.size();
    const char *ptr = fileBuffer.constData();
    my_serial->write(ptr, filesize);
}

void MainWindow::ethConnected(){
    qDebug() << "eth连接成功";
    ui->ethConnect_pushButton->setText("断开");
    ui->ethConnect_pushButton->setChecked(true);
}
void MainWindow::ethReadyRead(){
    qDebug() << "eth接收准备";
    QByteArray buf = ethSocket->readAll();
    ui->eth_textBrowser->append(QString::fromLocal8Bit(buf));
}
void MainWindow::ethDisconnected(){
    qDebug() << "eth断开成功";
    ui->ethConnect_pushButton->setText("连接");
    ui->ethConnect_pushButton->setChecked(false);
}

void MainWindow::ethError(){
    qDebug() << "eth操作失败";
}
void MainWindow::wifiConnected(){
    qDebug() << "wifi连接成功";
    ui->wifiConnect_pushButton->setText("断开");
    ui->wifiConnect_pushButton->setChecked(true);
}
void MainWindow::wifiReadyRead(){
    qDebug() << "wifi接收准备";
    QByteArray buf = wifiSocket->readAll();
    ui->wifi_textBrowser->append(QString::fromLocal8Bit(buf));
}
void MainWindow::wifiDisconnected(){
    qDebug() << "wifi断开成功";
    ui->wifiConnect_pushButton->setText("连接");
    ui->wifiConnect_pushButton->setChecked(false);
}

void MainWindow::wifiError(){
    qDebug() << "WiFi操作失败";
}

void MainWindow::on_ethConnect_pushButton_clicked()
{
    if(ui->ethConnect_pushButton->isChecked()){
        ethSocket->connectToHost(ui->ethIP_lineEdit->text(), ui->ethPort_lineEdit->text().toInt());
    }else{
        ethSocket->disconnectFromHost();
    }
}

void MainWindow::on_wifiConnect_pushButton_clicked()
{
    if(ui->wifiConnect_pushButton->isChecked()){
        wifiSocket->connectToHost(ui->wifiIP_lineEdit->text(), ui->wifiPort_lineEdit->text().toInt());
    }else{
        wifiSocket->disconnectFromHost();
    }
}

void MainWindow::on_netSend_pushButton_clicked()
{
    if(ethSocket->state() == QTcpSocket::ConnectedState){
        ethSocket->write(QByteArray(ui->netSend_textEdit->toPlainText().toLocal8Bit()));
    }else{
        QMessageBox::critical(this, "警告", "ETH未连接，请先连接。");
    }
    if(wifiSocket->state() == QTcpSocket::ConnectedState){
        wifiSocket->write(QByteArray(ui->netSend_textEdit->toPlainText().toLocal8Bit()));
    }else{
        QMessageBox::critical(this, "警告", "wifi未连接，请先连接。");
    }
}

