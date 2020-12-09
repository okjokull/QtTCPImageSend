#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtNetwork/QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Init();
    void newTcpConnect();
    void displayVideo();

private slots:
    void acceptData();
    void displayError();
    void acceptconnetion();


private:
    Ui::MainWindow *ui;
    QTcpSocket *client_socket;
    QTcpServer *qtcpserver;
    int dataSize;
};

#endif // MAINWINDOW_H
