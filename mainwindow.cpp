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
     * 1 �����׽���
     * 2 ����
         **/
    dataSize = 0;
   this->client_socket = new QTcpSocket(this);
    qtcpserver =new QTcpServer(this);
   newTcpConnect();
   connect(qtcpserver,SIGNAL(newConnection()),this,SLOT(acceptconnetion()));
   connect(client_socket,SIGNAL(readyRead()),this,SLOT(acceptData()));//readyRead()��ʾ����˷������ݹ����������ź�
   connect(client_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError()));
}
void MainWindow::acceptData()
{
    //QString strData = client_socket->readAll();//��������
    //ui->lineEdit->setText(strData);
    //�ͷ��˵�һ���Ƚ�����������ͼƬ��С��Ϣ
    if(dataSize == 0)
    {
        QDataStream in(client_socket); // ���׽�����������read ��in,�ٵ�datasize��
        in.setVersion(QDataStream::Qt_5_6);
        if(client_socket->bytesAvailable() < sizeof(quint32))//���Ķ����ֽ���С��4���ֽ�
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
    QBuffer buffer(&data);//ΪQByteArray�ṩһ����д�豸�Ļ�����
    buffer.open(QIODevice::ReadOnly);

    //Ϊ���ļ������豸�ж�ȡһ��ͼƬ���õĽӿ�
    QImageReader reader(&buffer,"jpg");
    QImage iMage = reader.read();

    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap::fromImage(iMage,Qt::AutoColor));

    if(dataSize != 0)
    {
        ui->lineEdit_2->setText(QString::number(dataSize));
        ui->lineEdit_2->setAlignment(Qt::AlignCenter);
    }
    dataSize = 0;//��datasizeͼƬ��С��Ϣ����0Ϊ��һ������׼����

}
void MainWindow::displayError()
{
    qDebug()<<"error: "<<client_socket->errorString();
    client_socket->close();
}

void MainWindow::acceptconnetion()
{
    client_socket =qtcpserver->nextPendingConnection();//ָ��������һ�����ӵ��׽���
}

//���ó����������Է��ʷ����
void MainWindow::newTcpConnect()
{
    client_socket->abort();//��ֹ��ǰ���ӣ����������׽���
    client_socket->connectToHost("192.168.0.115",5555,QTcpSocket::ReadWrite);//���ص�ip
}
MainWindow::~MainWindow()
{
    delete ui;
}
