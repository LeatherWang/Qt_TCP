#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tcpServer = new QTcpServer(this);
        if(!tcpServer->listen(QHostAddress::LocalHost,2001))
        {  //监听本地主机的6666端口，如果出错就输出错误信息，并关闭
            qDebug() << tcpServer->errorString();
            close();
        }
     connect(tcpServer,SIGNAL(newConnection()),this,SLOT(sendMessage()));
    //连接信号和相应槽函数
}

void MainWindow::sendMessage()
{
    QByteArray block; //用于暂存我们要发送的数据
    QDataStream out(&block,QIODevice::WriteOnly);
    //使用数据流写入数据
    out.setVersion(QDataStream::Qt_4_6);
    //设置数据流的版本，客户端和服务器端使用的版本要相同
    out<<(quint16) 0;
    out<<tr("hello Tcp!!!");
    out.device()->seek(0);
    out<<(quint16) (block.size() - sizeof(quint16));

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    //我们获取已经建立的连接的子套接字
    connect(clientConnection,SIGNAL(disconnected()),clientConnection,
            SLOT(deleteLater()));
    clientConnection->write(block);
    clientConnection->disconnectFromHost();
    ui->statusLabel->setText("send message successful!!!");
    //发送数据成功后，显示提示
}

MainWindow::~MainWindow()
{
    delete ui;
}
