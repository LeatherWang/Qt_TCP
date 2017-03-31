#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setStyleSheet("background-color:rgb(145,200,200);");
    ui->pushButton_2->setStyleSheet("background-color:rgb(180,218,218);");
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(displayError(QAbstractSocket::SocketError)));

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-100, 0);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newConnect()
{
    blockSize = 0; //初始化其为0
    tcpSocket->abort(); //取消已有的连接
    //tcpSocket->connectToHost(ui->hostLineEdit->text(), ui->portLineEdit->text().toInt());
    tcpSocket->connectToHost("192.168.10.1",
                             8082);
    //连接到主机，这里从界面获取主机地址和端口号
}

#define BSSID_raw 3
char BSSID[BSSID_raw][18]{"20:0c:c8:4a:df:fa","fc:d7:33:4a:5c:c8","10:08:b1:d9:93:eb"};//SSID:Robot-AN-101,Robot-IS-108,WW
float RSSIValue[BSSID_raw]={0.0};
QByteArray byteArray;
void MainWindow::readMessage()
{
    if(tcpSocket->bytesAvailable()>0)
    {
        const QByteArray data = tcpSocket->readAll();
        ui->textBrowser->append(data);

        for(quint8 i=0; i<BSSID_raw; i++)
        {
            if(data.indexOf(BSSID[i]) < 0)//判断此wifi是否在范围内
            {
                RSSIValue[i] = -99.0;
            }
            else
            {
                byteArray = data.mid(data.indexOf(BSSID[i]) , data.size()-data.indexOf(BSSID[i]));
                RSSIValue[i] = byteArray.mid(byteArray.indexOf("signal:") + 8,
                                             byteArray.indexOf("SSID:") - byteArray.indexOf("signal:") - 14).toFloat();
            }
        }
        qDebug()<<RSSIValue[0]<<RSSIValue[1]<<RSSIValue[2];

        QTime time(QTime::currentTime());//获取当前时间
        double key = time.hour()*3600 + time.minute()*60 + time.second() + time.msec()*0.001;

        ui->customPlot->graph(0)->addData(key, RSSIValue[0]);
        ui->customPlot->graph(1)->addData(key, RSSIValue[1]);

        // make key axis range scroll with the data (at a constant range size of 8):
        ui->customPlot->xAxis->setRange(key, 40, Qt::AlignRight);
        ui->customPlot->replot();
    }
}

void MainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug() <<"error!"<< tcpSocket->errorString(); //输出错误信息
    ui->messageLabel->setText("<font color=red>error!!</font>"+tcpSocket->errorString());
}

bool openCloseFlag = false;
void MainWindow::on_pushButton_clicked() //连接按钮
{
    if(openCloseFlag == false)
    {
        newConnect(); //请求连接
        openCloseFlag = true;
        ui->pushButton->setText("断开服务器");
        ui->pushButton->setStyleSheet("background-color:rgb(255,128,128);");
    }
    else
    {
        tcpSocket->write("p");
        tcpSocket->disconnectFromHost();
        openCloseFlag = false;
        ui->pushButton->setText("连接服务器");
        ui->pushButton->setStyleSheet("background-color:rgb(145,200,200);");
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    tcpSocket->write(ui->send_lineEdit->text().toLatin1());
}
