#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>

#include "raspi_gui_ipc.h"
#include "raspi_ipc_defs.h"

#define IPC_GUI_MAX_CLIENTS   15

typedef struct 
{
    int serv_sock;
    struct sockaddr_un localaddr;

    // one to one association
    struct sockaddr_un remote_sock[ IPC_GUI_MAX_CLIENTS ];
    int client_sock[ IPC_GUI_MAX_CLIENTS ]; 
    int curClientIdx;
}serverContext;


typedef struct
{
    int cli_sock;
    struct sockaddr_un local_servaddr;  
}clientContext;

serverContext servCtx;
clientContext cliCtx;

int raspi_gui_server_init( void )
{
    int ret = -1;
    int len = 0;

    servCtx.curClientIdx = 0;
    servCtx.localaddr.sun_family = AF_UNIX;
   

    strncpy( servCtx.localaddr.sun_path, SOCKET_NAME_SERVER, sizeof(servCtx.localaddr.sun_path) - 1 );

    if( ( servCtx.serv_sock = socket( AF_UNIX, SOCK_STREAM,  0 ) ) != -1 )
    {

        unlink( servCtx.localaddr.sun_path );

        if( bind ( servCtx.serv_sock, (struct sockaddr*)&servCtx.localaddr, sizeof(struct sockaddr_un)) == -1 )
        {
            printf( "server bind error:\n %s\n", strerror( errno ) ); 
        }
        else 
        {
            listen( servCtx.serv_sock, MAX_SERVER_CONNECTIONS );
            ret = 0;
        }

    }
    else
    {
        printf( "server socket() call error:\n %s \n", strerror( errno ) );
    }
    return ret;
}

int raspi_gui_server_accept( void )
{
    int ret = -1;

    printf( "server waiting for client connection... \n" );
    if( ( ret = accept( servCtx.serv_sock, NULL, NULL ) ) != -1 )
    {
       printf( "succesfull client connection \n" );
       
       servCtx.client_sock[ servCtx.curClientIdx ] = ret;
       servCtx.curClientIdx++; 
       ret = 0;
    }

    return ret;
}


int raspi_gui_server_send( char* buffer, int size )
{
    int i = 0;
    int ret = 0;
    if( buffer != NULL && size > 0 )
    {
        // send to all clients
        for( i = 0 ; i < servCtx.curClientIdx ; i ++ )
        {
            if( ( ret = send( servCtx.client_sock[ i ], buffer, size, MSG_DONTWAIT ) ) == -1 )
            {
                perror( "server send error to client socket \n" );
            }
            //ret = write(  servCtx.client_sock[ i ], buffer, size );    
        }
    }
    return ret;
}

int raspi_gui_server_send_gauge( unsigned char id, unsigned short value, unsigned char unit  )
{
    int ret = -1;
    unsigned char send_buff[ IPC_SET_GAGE_FRAME_SZ ] = 
    {
        IPC_SET_GAGE_SOF,
        0x00, // ID
        0x00, // DATA H
        0x00, // DATA L
        0x00, // UNIT
    };

    send_buff[ IPC_ID_IDX ] = id;
    send_buff[ IPC_DATA_IDX_H ] = ( unsigned char ) ( ( value & 0xff00 ) >> 8 );
    send_buff[ IPC_DATA_IDX_L ] = ( unsigned char ) ( value & 0x00ff );
    send_buff[IPC_UNIT_IDX] = unit;

    ret = raspi_gui_server_send( send_buff, IPC_SET_GAGE_FRAME_SZ );

    return ret;
}

#define ON   1
#define OFF  0
int raspi_gui_server_send_ignition( unsigned char IG )
{
    int ret = -1;

    if( IG == ON )
    {
        ret = raspi_gui_server_send_gauge( ID_TURN_ON, 0, 0 );
    }
    else if( IG == OFF )
    {
        ret = raspi_gui_server_send_gauge( ID_TURN_OFF, 0, 0 );

    }

    return ret;
}



void raspi_gui_server_close( void )
{
    int i = 0;
    for( i = 0 ; i < servCtx.curClientIdx ; i++ )
    {
        close( servCtx.client_sock[ i ] );
    }

    close( servCtx.serv_sock );
}


// ****** client api, test purpose only ********

int raspi_gui_client_init( void )
{
    int ret = -1;
    memset( &cliCtx, 0x00, sizeof(cliCtx));

    cliCtx.local_servaddr.sun_family = AF_UNIX;

    strncpy( cliCtx.local_servaddr.sun_path, SOCKET_NAME_SERVER, sizeof(cliCtx.local_servaddr.sun_path) );

    ret = socket( AF_UNIX, SOCK_STREAM, 0 );


    if ( ( ret = socket( AF_UNIX, SOCK_STREAM, 0 ) ) == -1 ) 
    {
        printf( "%s function error\n", __FUNCTION__ );
    }
    else
    {
        cliCtx.cli_sock = ret; 
        printf( "client initialization success!!\n" );
    }
    return ret;
}

int raspi_gui_client_connect( void )
{
    int ret = -1;
    
    if ( ( ret = connect( cliCtx.cli_sock, (struct sockaddr*)&cliCtx.local_servaddr, sizeof( cliCtx.local_servaddr ) ) ) == -1 )
    {
        perror( "client connect error\n" );
    }
    
    return ret;
}

int raspi_gui_client_recv( char* buffer, int size )
{ 
    int ret = 0;

    //ret = read( cliCtx.cli_sock );
    if ( ( ret = recv( cliCtx.cli_sock, buffer, size, MSG_DONTWAIT ) )  == -1 )
    {
        perror( "client receive error\n" );
    }

    return ret;
}

void raspi_gui_client_close( void )
{
    close( cliCtx.cli_sock );

}

