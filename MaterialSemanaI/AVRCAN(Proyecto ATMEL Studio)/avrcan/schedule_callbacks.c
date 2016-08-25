/*---------------------------------------------------------------------------*/
/**************************************************************************//**
 *  \file      schedule_callbacks.c
 *  \addtogroup timers
 *  @{
 *  \author    Roberto Alejandro Flores Estrada
 *  \since     10/08/2015
 *
 *****************************************************************************/

#include <schedule_callbacks.h>
#include <can_midleware.h>
#include <can_driver.h>
#include <Indicator_Driver.h>
/*---------------------------------------------------------------------------*/

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox0( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox0( void )
{
    CanMw_Send_TX_Message( 0 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox1( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox1( void )
{
    CanMw_Send_TX_Message( 1 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox2( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox2( void )
{
    CanMw_Send_TX_Message( 2 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox3( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox3( void )
{
    CanMw_Send_TX_Message( 3 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox4( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox4( void )
{
    CanMw_Send_TX_Message( 4 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox5( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox5( void )
{
    CanMw_Send_TX_Message( 5 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox6( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox6( void )
{
    CanMw_Send_TX_Message( 6 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox7( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox7( void )
{
    CanMw_Send_TX_Message( 7 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox8( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox8( void )
{
    CanMw_Send_TX_Message( 8 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox9( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox9( void )
{
    CanMw_Send_TX_Message( 9 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox10( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox10( void )
{
    CanMw_Send_TX_Message( 10 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox11( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox11( void )
{
    CanMw_Send_TX_Message( 11 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox12( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox12( void )
{
    CanMw_Send_TX_Message( 12 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox13( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox13( void )
{
    CanMw_Send_TX_Message( 13 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox14( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox14( void )
{
    CanMw_Send_TX_Message( 14 );
    return;
}

/**************************************************************************//**
 * \fn      void schedule_callback_Mailbox15( void )
 * \brief
 * \return
 *****************************************************************************/
void schedule_callback_Mailbox15( void )
{
    CanMw_Send_TX_Message( 15 );
    return;
}
