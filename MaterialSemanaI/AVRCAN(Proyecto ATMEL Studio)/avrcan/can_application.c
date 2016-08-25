/****************************************************************** 
* can_application.c
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
#ifndef can_application_C_
 #define can_application_C_
#endif
/**
 * \addtogroup can_application_module
 * @{
 */

/******************************************************************/
/*    I N C L U D E   F I L E S                                   */
/******************************************************************/
#include <can_midleware.h>
#include <cpu.h>
#include <usart.h>
#include <task_scheduler.h>
#include <watchdog.h>
#include <hardware_init.h>
#include <can_driver.h>
#include <avr/interrupt.h>
#include <print_functions.h>
#include <can_message_configs.h>
#include <Indicator_Driver.h>

/*******************************************************************************************/
/*    M A C R O S                                                                          */
/*******************************************************************************************/
//#define MailBox_0		RX_ACC_CAN , MAIL_BOX_ENABLE_RX , 8 , (CAN_ID_type)0x0294,  (CAN_ID_type)0x07FF ,INTERRUPT_CAN_LEVEL_1 , 10
//#define MailBox_1		RX_PRN_CAN , MAIL_BOX_ENABLE_RX , 6 , (CAN_ID_type)0x0188,  (CAN_ID_type)0x07FF ,INTERRUPT_CAN_LEVEL_1 , 10
//#define MailBox_2		TX_CUSTOM ,  MAIL_BOX_ENABLE_RX   , 8 , (CAN_ID_type)0x0101,  (CAN_ID_type)0x0000 ,INTERRUPT_CAN_LEVEL_1 , 10

/*******************************************************************************************/
/*    F U N C T I O N   P R O T O T Y P E S                                                */
/*******************************************************************************************/
volatile uint8_t can_initialization=0;
void Can_AppLED(void);

void Can_App_Init(void){
	Can_Driver_Init();
	CanMW_APP_Init();

	CanMW_InitMail_Box(MailBox_0);
	CanMW_InitMail_Box(MailBox_1);
	CanMW_InitMail_Box(MailBox_2);

	schedulerSetupItem( 0, TRUE, SCH_100MS_SETUP );
	schedulerSetupItem( 1, TRUE, SCH_150MS_SETUP );
	schedulerSetupItem( 2, TRUE, SCH_220MS_SETUP );
	//CanMw_Send_TX_Messages();

	ENABLE_CAN_RECEIVE_INTERRUPT();
	can_initialization=1;
}
void CAN_App_50ms_Task(void){


}
void Can_App_IDLE_Hook(void){
	if(CanMW_Event_Get()){
		CanMW_ReSync_Driver();

		CanMW_Event_Clean();
		ENABLE_CAN_RECEIVE_INTERRUPT();
		ENABLE_ALL_INTERRUPTS();
		sei();
	}
	if(can_initialization==0){
		CanMW_ReConf_Driver();
		can_initialization=1;
	}
}
void Can_App_200ms_Task(void){
	/* Transmission */

}
void ErrorTask(void){
	Can_err_type status_error=0;

	status_error=Get_CAN_Tx_error();
	if(status_error){
		Led_Err(LED_INDICATOR_ON);
	}else{
		Led_Err(LED_INDICATOR_OFF);
	}
	
}
void Can_AppLED(void){
	CanMw_Message Mbread;

	Get_Message_FromMbox(RX_ACC_CAN,&Mbread);
	if(Mbread.data[0]==0x20){
		Led_Tx(LED_INDICATOR_ON);
	}else{
		Led_Tx(LED_INDICATOR_OFF);
	}
}
/**
 * @}
 */
