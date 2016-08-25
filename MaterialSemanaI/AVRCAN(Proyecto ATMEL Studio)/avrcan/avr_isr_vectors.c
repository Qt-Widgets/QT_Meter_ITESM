/****************************************************************** 
* avr_isr_vectors.c
* Created on: Aug 6, 2015
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*© 2015
*****************************************************************/ 
#ifndef avr_isr_vectors_C_
 #define avr_isr_vectors_C_
#endif
/**
 * \addtogroup avr_isr_vectors_module
 * @{
 */

/******************************************************************/
/*    I N C L U D E   F I L E S                                   */
/******************************************************************/
#include <cpu.h>
#include <can_midleware.h>
#include <avr/interrupt.h>

/*******************************************************************************************/
/*    M A C R O S                                                                          */
/*******************************************************************************************/


/*******************************************************************************************/
/*    F U N C T I O N   P R O T O T Y P E S                                                */
/*******************************************************************************************/
#pragma vector=SIG_CAN_INTERRUPT1
ISR(SIG_CAN_INTERRUPT1)
{
	CanMW_set_mutex(MUTEX_LOCK_CHANNEL);
	CanMW_ReceivedRxHandle();
	CanMW_set_mutex(MUTEX_RELEASE_CHANNEL);
}

/**
 * @}
 */
