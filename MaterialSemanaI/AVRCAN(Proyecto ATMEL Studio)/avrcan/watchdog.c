/****************************************************************** 
 * watchdog.c
 * Created on: Jul 13, 2015
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
#ifndef watchdog_C_
#define watchdog_C_
#endif
/**
 * \addtogroup watchdog_module
 * @{
 */


/******************************************************************/
/*    I N C L U D E   F I L E S                                   */
/******************************************************************/
#include <watchdog.h>
#include <avr\wdt.h>


/*******************************************************************************************/
/*    M A C R O S                                                                          */
/*******************************************************************************************/


/*******************************************************************************************/
/*    F U N C T I O N   P R O T O T Y P E S                                                */
/*******************************************************************************************/
//void wdt_off(void)
//{
//	asm("push r30");	// using GCC without optimizations results in
//	asm("ldi r30, 0x18");	// a code not meeting the requirements
//	asm("sts 0x60, r30");	// of the datasheet.
//	asm("ldi r30, 0x7");
//	asm("sts 0x60, r30");
//	asm("pop r30");
//	WDTCR = (1<<WDCE) | (1<<WDE);
//	WDTCR = 0x07;					//increase Watchdog time-out period (Bit0,1,2 = 1)
//}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 11:12:03 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void watchdog_reset(void){
	wdt_reset();
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *   \see See more information Watchdog Timer Control Register – WDTCR
 *  \note First Implementation 11:11:59 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void watchdog_off(void)
{
	wdt_disable();
	return;
}
/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Watchdog Timer Control Register – WDTCR
 *  \note First Implementation 11:11:56 AM Jul 13, 2015
 *  \warning Pending for Review
 *  \author Author
 ****************************************************************************************/
void watchdog_on(WATCHDOG_COMMON_TIMES param)
{
	/* Write logical one to WDCE and WDE */
	wdt_enable(param);
	return;
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Watchdog Timer Control Register – WDTCR
 *  \note First Implementation 11:11:50 AM Jul 13, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void watchdog_preescaler(uint8_t options){
	WDTCR |= (1<<WDCE) | options ;
}

//TODO: Review WatchDog sequences.

/**
 * @}
 */
