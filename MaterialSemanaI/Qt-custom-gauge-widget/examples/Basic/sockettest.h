#ifndef SOCKETTEST_H
#define SOCKETTEST_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QAbstractSocket>


class SocketTest : public QObject
{
    Q_OBJECT
public:
    explicit SocketTest(QObject *parent = 0);
    void OpenSocket( u_int16_t port );
    
signals:
    void newData( float value, int unitSel );
public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket;
    
};

#endif // SOCKETTEST_H
