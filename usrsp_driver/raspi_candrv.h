/*! \file    raspi_candrv.h
*   \brief   I2C to CAN message user space driver API header file
*/


#ifndef RASPI_CANDRV_H_
#define RASPI_CANDRV_H_

// ************ ENABLE FOR DEBUGGING!!! ***************

//#define USE_FAKE_INIT
//#define USE_FAKE_READ
//#define USE_FAKE_WRITE

//*****************************************************

// can driver return codes
#define RASPI_CAN_CLOSED    1
#define RASPI_CAN_OPEN      0
#define RASPI_CAN_ERROR      -1

#define RASPI_CAN_OK        0
#define RASPI_CAN_EMPTY     1
#define RASPI_CAN_BUSY      2

/*****************************************/
// msg CAN IDs

#define MSG_CAN_ID_SPEED    0x0081
#define MSG_CAN_ID_TACHO    0x0093 
#define MSG_CAN_ID_FUEL     0x0160



#define MG_CAN_ID_SYS_UNIT  0x34A
#define MSG_CAN_ID_IGN_1    0x24B

//****************************************


#define CAN_DATA_MAX_SZ    10

typedef struct
{
    short int msgID;
    char  DLC;
    char  data[ CAN_DATA_MAX_SZ ];
    int   period;
}can_msg;


typedef struct
{
    int canFd;
    int i2c_slave_addr;
    int status;
}raspi_can_ctx;

/*! \fn  int raspi_can_init( raspi_can_ctx* ctx )
* \brief initialize raspberry i2c interface and prepare for receive msg
*        as I2c bus master receiver
* \param  ctx structure holds current i2c module low level configuration
* \return initialization result 0 -> good -1 -> fail
*/
extern int raspi_can_init( raspi_can_ctx* ctx );

/*! \fn int raspi_can_poll( raspi_can_ctx* ctx, int size, can_msg* msg )
*   \brief   poll pre-configured mailboxes this will translate into master reques
*            and then a slave response
*   \param   ctx structure holds current i2c module low level configuration
*   \param   size maximum number of messages to be retreived from can mailboxes
*   \param   msg pointer to can message buffer ( please be sure enough 
*            space for all messages will be available ) 
*   \return  number of received messages : -1 > error
*/
extern int raspi_can_poll( raspi_can_ctx* ctx, int size, can_msg* msg );

/*! \fn  void raspi_can_close( raspi_can_ctx* ctx )
*  \brief release resources allocated during i2c module initialization
*  \param ctx structure holds current i2c module configuration
*/
extern void raspi_can_close( raspi_can_ctx* ctx );

//TODO: currently not supported, mmaybe add later
//int raspi_can_send( raspi_can_ctx* ctx, can_msg* msg );

#endif
