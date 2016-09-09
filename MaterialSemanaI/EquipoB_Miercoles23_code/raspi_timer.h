#ifndef _RASPI_TIMER_H_
#define _RASPI_TIMER_H_
/*!
*  \file raspi_timer.h 
*  \brief  convenience functions for the purpose of making an
*          easy timer interface 
*        
*/


typedef int (* callback_t)( int id );


/*!   \fn  int raspi_timer_init_framework( void )
*    \brief initialize timer infrastructure for use with timers 
*    \return  0: success   -1: fail 
*/

extern int raspi_timer_init_framework( void );

/*!   \fn  int raspi_timer_create( int msec )
*    \brief create timer given a time and a callback 
*    \param   msec.  number of milliseconds for callback pointer to
*                    be triggered 
*    \return  0: success   -1: fail 
*/

extern int raspi_timer_create( unsigned long msec, callback_t fptr );

/*!   \fn int raspi_timer_start( int timerID )
*    \brief start timer associated with given id 
*    \param   tmrID timer ID
*    \return  0: success   -1: fail 
*/

extern int  raspi_timer_start( int tmrID );


/*!   \fn int raspi_timer_stop( int tmrID )
*    \brief  stop timer with given id
*    \param   tmmrID timer id.
*    \return  0: success   -1: fail 
*/
extern int  raspi_timer_stop( int tmrID );

/*!   \fn  raspi_timer_close_framework( void )
*    \brief  close timer framework and release resources 
*/
extern void raspi_timer_close_framework( void );

 #endif
