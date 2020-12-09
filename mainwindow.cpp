#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "qdatastream.h"
#include "QImageReader"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
}
void MainWindow::Init()
{
    /*
     * 1 创建套接字
     * 2 连接
         **/
    dataSize = 0;
   this->client_socket = new QTcpSocket(this);
    qtcpserver =new QTcpServer(this);
   newTcpConnect();
   connect(qtcpserver,SIGNAL(newConnection()),this,SLOT(acceptconnetion()));
   connect(client_socket,SIGNAL(readyRead()),this,SLOT(acceptData()));//readyRead()表示服务端发送数据过来即发动信号
   connect(client_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError()));
}
void MainWindow::acceptData()
{
    //QString strData = client_socket->readAll();//接受数据
    //ui->lineEdit->setText(strData);
    //客服端第一次先接收数据流中图片大小信息
    if(dataSize == 0)
    {
        QDataStream in(client_socket); // 将套接字中数据流read 到in,再到datasize；
        in.setVersion(QDataStream::Qt_5_6);
        if(client_socket->bytesAvailable() < sizeof(quint32))//可阅读的字节数小于4个字节
        {
            return;
        }
        in>>dataSize;
    }
    if(dataSize > client_socket->bytesAvailable()) return;
    displayVideo();

}
void MainWindow::displayVideo()
{
    QByteArray data = client_socket->read(dataSize);
    QBuffer buffer(&data);//为QByteArray提供一个读写设备的缓存区
    buffer.open(QIODevice::ReadOnly);

    //为从文件或者设备中读取一个图片设置的接口
    QImageReader reader(&buffer,"jpg");
    QImage iMage = reader.read();

    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap::fromImage(iMage,Qt::AutoColor));

    if(dataSize != 0)
    {
        ui->lineEdit_2->setText(QString::number(dataSize));
        ui->lineEdit_2->setAlignment(Qt::AlignCenter);
    }
    dataSize = 0;//将datasize图片大小信息重置0为下一接收做准备。

}
void MainWindow::displayError()
{
    qDebug()<<"error: "<<client_socket->errorString();
    client_socket->close();
}

void MainWindow::acceptconnetion()
{
    client_socket =qtcpserver->nextPendingConnection();//指导返回下一个链接的套接字
}

//采用程序启动就自访问服务端
void MainWindow::newTcpConnect()
{
    client_socket->abort();//终止当前连接，重新设置套接字
    client_socket->connectToHost("192.168.0.115",5555,QTcpSocket::ReadWrite);//本地的ip
}
MainWindow::~MainWindow()
{
    delete ui;
}
