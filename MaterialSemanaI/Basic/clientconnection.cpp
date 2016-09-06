#include <QApplication>
#include <clientconnection.h>
#include <QHostAddress>

#include <stdint.h>


QString ClientConnection::serverName;


ClientConnection::ClientConnection(QObject *parent) :
    QObject(parent)
{

    socket = new QLocalSocket( this );

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    //connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(connected()));

    serverName = SOCKET_NAME_SERVER;
}

void ClientConnection::connectToServer( void )
{

    qDebug() << "Connecting GUI client in 45 secs,..";

    //socket->connectToHost( QHostAddress::LocalHost, port);
    socket->connectToServer( serverName, QIODevice::ReadWrite );

    qDebug() << "Attempting local connection to: " << serverName;

    if( !socket->waitForConnected(3000) )
    {
        qDebug() << "Error: " << socket->errorString();
        qDebug() << "cannot connect, launch server first!";
    }

}

QLocalSocket::LocalSocketState ClientConnection::state( )
{
    return socket->state();
}

void ClientConnection::connected()
{
    qDebug() << "Qt App Gage: Succesfull client GUI connection!!";
    qDebug() << "QT Gage Appication Connected!";
}

void ClientConnection::disconnected()
{
    qDebug() << "QT Gage Application Disconnected!";
}

void ClientConnection::bytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes;
}


void ClientConnection::readyRead()
{
    uint16_t value = 0;
    uint8_t  unit = 0;
    uint8_t id = 0;
    QString strData;
    QByteArray data;
    QByteArray frame;

    data = socket->readAll();

    strData = QString::fromStdString( data.toHex().toStdString() );

    qDebug() << "Reading" << data.size() << +  " bytes...";
    qDebug() << strData;

    while ( ( frame = processData( data ) ).size() > 0 )
    {
        if( frame.size() == IPC_SET_GAGE_FRAME_SZ &&
                ( (unsigned char) frame.at(0) == IPC_SET_GAGE_SOF ) )
        {

            id = frame.at( IPC_ID_IDX );

            value = ( ( ( ( (uint16_t)frame.at( IPC_DATA_IDX_H ) ) << 8 ) & 0xFF00 ) )
                    | ( ( ( uint16_t ) frame.at( IPC_DATA_IDX_L) ) & 0x00FF);

            unit = (uint8_t ) frame.at( IPC_UNIT_IDX );

            qDebug() << "id: " << id;
            qDebug() << "gage value:" << value;
            qDebug() << "unit" << unit;

            emit ( newData( id, value , unit ) );

        }
        else
        {
            qDebug() << "Invalid Data, SOF / frame size!! ";
        }

    }
}

QByteArray ClientConnection::processData( QByteArray& data )
{
    int i = 0;
    QByteArray ret;

    // get number of complete frames

    while( i < data.size() )
    {
       if( ( ( unsigned char )data.at( i ) ) == IPC_SET_GAGE_SOF &&
             ( ( i + IPC_SET_GAGE_FRAME_SZ ) <= data.size() ) )
       {
           ret = data.mid( i, i + IPC_SET_GAGE_FRAME_SZ );
           data.remove( i, i + IPC_SET_GAGE_FRAME_SZ );
           break;
       }

       i++;

    }
    return ret;

}
