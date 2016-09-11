/*!  \file raspi_can_queue.h
*    \brief  Implementation of polling queue for can messages
*/

#ifndef RASPI_CAN_QUEUE_H_
#define  RASPI_CAN_QUEUE_H_

#include "raspi_candrv.h"

#define CAN_MSG_QUEUE_OK     0
#define CAN_MSG_QUEUE_ERR    -1

#define MAX_QUEUE_SIZE  50000
#define MAX_POLL_BUFF_SIZE 10

/*! \fn int raspi_can_msg_queue_init( void )
*  \brief  prepare internal queue for message reception
*        
*  \return  0 : sucess ; -1 : fail
*/
extern int raspi_can_msg_queue_init( void );

/*! \fn int rapspi_can_msg_queue_start( void )
*  \brief start i2c message polling, this is spawn a thread
*         that will take control with i2c hardware and will
*         start communication with i2c slave to get new can
*         mailbox data
*  \return  0 : sucess ; -1 : fail
*/
extern int rapspi_can_msg_queue_start( void );

/*! \fn void raspi_can_msg_queue_stop( void )
*  \brief   stop polling for i2c data from slave and
*           stop enqueuing of can messages
*  \return  0 : sucess ; -1 : fail
*/
extern void raspi_can_msg_queue_stop( void );

/*! \fn int raspi_can_msg_queue_size( void )
*  \brief  retreive queue size so far. Total queue size
*          will be that of MAX_QUEUE_SIZE definition 
*  \return  0 : sucess ; -1 : fail
*/
extern int raspi_can_msg_queue_size( void );

/*! \fn int raspi_can_msg_dequeue( can_msg* msg )
*  \brief  dequeue messages from can message queue. Queue will
*           be increased by raspberry i2c polling thread 
*           each time a succesfull can message is receieved
*            via I2C protocol
*  \param    msg. pointer to message structure to be populated.         
*  \return   1: success -1 : fail
*/
extern int raspi_can_msg_dequeue( can_msg* msg );


#endif
