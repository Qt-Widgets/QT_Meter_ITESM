/*!
*  \file raspi_gui_ipc.h
*  \brief Inter process communication API. Methods implemented here
*         will enable IPC betwen application code and GUI application
*/

#ifndef RASPI_GUI_IPC_
#define RASPI_GUI_IPC_

#define E_NOCLIENT  -2

// use thesei gage ids when setting gage type 
//********************************************
#define ID_GAGE_SPEED   1
#define ID_GAGE_TACHO   2
#define ID_GAGE_FUEL    3

/*********************************************/
//special purpose ids
#define ID_TURN_ON      0
#define ID_TURN_OFF    125 
// Use these unit definitions when setting unit ype
// *****************************************
// speed units
#define KPH   0
#define MPH   1

// tacho units
#define RPM   0

// fuel units
#define LTS   0
#define GAL   1
//*******************************************

/*! \fn  int raspi_gui_server_init( void )
*  \brief perform inter process communication framework initialization.
*         must be called before  accepting client conections
*  \return 0 -> suzzes  -1 -> fail
*/
extern int raspi_gui_server_init( void );
/*!   \fn   int raspi_gui_server_accept( void )
*    \brief  start listening for incoming client connections. 
*            warning!! this is a blocking call. Block forever until
*            connection is  available
*     \return client socket descriptor
*/
extern int raspi_gui_server_accept( void );
/*!   \fn   int raspi_gui_server_send( char* buffer, int size )
*    \brief  send array over via server connection. 
*            This is low level call, maybe not needed.
*    \param  buffer  array to be sent
*    \param  size    number of bytes to be sent from buffer           
*    \return  number of bytes sent : -1 error
*/
extern int raspi_gui_server_send( char* buffer, int size );
/*!   \fn   int raspi_gui_server_send_gauge( unsigned char id, unsigned short value, unsigned char unit )
*    \brief  send new gauge value over to server. Connection needs to be setup first. 
*    \param  id gage id number, where new value should be reflected.
*    \param  value new gage value, range value depends on gage implementation (GUI side)
*    \param  unit   gage unit value.
*    \return  0 : success  -1 : fail
*/
extern int raspi_gui_server_send_gauge( unsigned char id, unsigned short value, unsigned char unit );
/*!   \fn   int raspi_gui_server_send_ignition( unsigned char id )
*    \brief  send ignition event to gage client 
*    \param  IG Ignition state 
*    \return  0 : success  -1 : fail
*/
extern int raspi_gui_server_send_ignition( unsigned char IG );

/*!   \fn   void raspi_gui_server_close( void )
*    \brief  close server connection  
*/
extern void raspi_gui_server_close( void );

// ****** client api, test purpose only ********


/*!   \fn  int raspi_gui_client_init( void )
*    \brief initialize client connection. This will leave ground ready for 
*           raspi_client_connect() call.
*    \return  0: success   -1: fail 
*/
extern int raspi_gui_client_init( void );

/*!   \fn int raspi_gui_client_connect( void )
*    \brief initialize client connection. This will leave ground ready for 
*           raspi_client_connect() call.
*    \return  0: success   -1: fail 
*/
extern int raspi_gui_client_connect( void );

/*!   \fn int raspi_gui_client_recv( char* buffer, int size )
*    \brief  receive  data from server, allocate into buffer
*    \param  buffer. bytes will be allocated here.
*    \param  size. maximum number of bytes to be allocated.
*    \return > 0 : Number of bytes received ; -1 : fail
*/
extern int raspi_gui_client_recv( char* buffer, int size );

/*!   \fn void raspi_gui_client_close( void )
*    \brief  close client connection
*/
extern void raspi_gui_client_close( void );

#endif
