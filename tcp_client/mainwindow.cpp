#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
             this,SLOT(displayError(QAbstractSocket::SocketError)));
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

void MainWindow::readMessage()
{
    //qDebug() <<"error!";
   // qDebug() <<tcpSocket->readAll();
//    QDataStream in(tcpSocket);
//    in.setVersion(QDataStream::Qt_4_6);
//    //设置数据流版本，这里要和服务器端相同
//    if(blockSize==0) //如果是刚开始接收数据
//    {
//        //判断接收的数据是否有两字节，也就是文件的大小信息
//        //如果有则保存到blockSize变量中，没有则返回，继续接收数据
//        if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
//        in >> blockSize;
//    }
//    if(tcpSocket->bytesAvailable() < blockSize) return;
//    //如果没有得到全部的数据，则返回，继续接收数据
//    in >> message;
//    //将接收到的数据存放到变量中
    ui->textBrowser->append(tcpSocket->readAll());
//    //显示接收到的数据
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
    }
    else
    {
        tcpSocket->write("p");
        tcpSocket->disconnectFromHost();
        openCloseFlag = false;
        ui->pushButton->setText("连接服务器");
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    tcpSocket->write(QByteArray (ui->send_lineEdit->text().toLatin1()));
}
