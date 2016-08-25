#include <sockettest.h>
#include <QHostAddress>

SocketTest::SocketTest(QObject *parent) :
    QObject(parent)
{
}

void SocketTest::OpenSocket( u_int16_t port )
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(connected()));

    qDebug() << "Connecting,..";

    socket->connectToHost( QHostAddress::LocalHost, port);

    if(!socket->waitForDisconnected(1000))
    {
        qDebug() << "Error: " << socket->errorString();
    }

}

void SocketTest::connected()
{
    qDebug() << "Connected!";

    socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
}

void SocketTest::disconnected()
{
    qDebug() << "Disconnected!";
}

void SocketTest::bytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes;
}

void SocketTest::readyRead()
{
    QByteArray *data = new QByteArray(  socket->readAll() );
    qDebug() << "Reading...";
    qDebug() << data->data();
    emit ( newData( (*data)[0] , (*data)[1] ) );
}

