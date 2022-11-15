#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    decoder.initDecoder();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::udpInit()
{
    quint16 local_port = 2333;
    udp_server = new QUdpSocket(this);

    int ret = udp_server->bind( QHostAddress("172.16.1.13"), local_port );
    //int ret = udp_server->bind( local_address, local_port );
    if( ret < 0 ){
        return false;
    }

    connect(udp_server, SIGNAL(readyRead()), this, SLOT(udpServerReceiveData()));
    return true;
}

void MainWindow::udpServerReceiveData()
{
    QByteArray recvBuff;
    while( udp_server->hasPendingDatagrams() ){
        //qDebug()<<"received length: "<<udp_cam_server->pendingDatagramSize()<<endl;

        // Allocate the memory for the coming image
        recvBuff.resize( udp_server->pendingDatagramSize() );

        // Received the Data sent from the client
        udp_server->readDatagram(recvBuff.data(), recvBuff.size(), &client_address, &client_port);

       if( recvBuff[0] == 'L' && recvBuff[1] == 'E' && recvBuff[2] == 'N' && recvBuff[3] == 'G' ){
            memcpy( &frame_size, &recvBuff.data()[4], 4 );
            qDebug()<<"frame size : "<<frame_size<<endl;

            datagram.resize(frame_size);
        }
        else {
            memcpy( &datagram.data()[recv_count * MAXCACHE], recvBuff.data(), recvBuff.size() );
            recv_count ++;
            qDebug()<<"recvCount : "<<recv_count<<endl;

            if( recv_count == frame_size / MAXCACHE + 1 ){
                recv_count = 0;
                decoder.decodeOneFrame((uint8_t*)datagram.data(), frame_size, image);

                ui->label->setPixmap(QPixmap::fromImage(this->image).scaled(ui->label->size()));
            }
        }
    }
}


void MainWindow::on_pushButtonStart_clicked()
{
    if( udpInit() ){
        qDebug()<<"Init UDP SERVER !";
    }
}

void MainWindow::on_pushButtonClose_clicked()
{
    this->udp_server->close();
}
