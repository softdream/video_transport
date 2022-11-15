#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QNetworkInterface>

#include "decoder.h"

#define MAXCACHE 64512

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool udpInit();

private slots:
    void udpServerReceiveData();

    void on_pushButtonStart_clicked();

    void on_pushButtonClose_clicked();

private:
    QHostAddress local_address;

    QUdpSocket *udp_server;
    QHostAddress client_address;
    quint16 client_port = 0;

    int frame_size = 0;
    QByteArray datagram;
    int recv_count = 0;

    Decoder decoder;
    QImage image;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
