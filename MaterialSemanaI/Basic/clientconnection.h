#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QLocalSocket>
#include <QAbstractSocket>
#include <raspi_ipc_defs.h>


#define ID_GAGE_SPEED   1
#define ID_GAGE_TACHO   2
#define ID_GAGE_FUEL    3


class ClientConnection : public QObject
{
    Q_OBJECT

public:
    explicit ClientConnection(QObject *parent = 0);
    void connectToServer(void );
    QLocalSocket::LocalSocketState state( void );
    
signals:
    void newData(qint8 id, qint16 value, qint8 unitSel );
public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QByteArray processData( QByteArray& data );
    static QString serverName;
    QLocalSocket *socket;

};

#endif // SOCKETTEST_H
