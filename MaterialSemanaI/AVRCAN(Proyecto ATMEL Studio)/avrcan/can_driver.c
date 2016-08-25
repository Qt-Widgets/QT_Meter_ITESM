/****************************************************************** 
 * can_driver.c
 * Created on: Jul 30, 2015
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
#ifndef can_driver_C_
#define can_driver_C_
#endif
/**
 * \addtogroup can_driver_module
 * @{
 */

/******************************************************************/
/*    I N C L U D E   F I L E S                                   */
/******************************************************************/

#include <cpu.h>
#include <can_driver.h>
#include <avr/io.h>
/*******************************************************************************************/
/*    M A C R O S                                                                          */
/*******************************************************************************************/
#define MASK_CAN2A_CANIDX2	 	 0x07
#define MASK_CAN2A_CANIDX1		0x7F8

#define CAN2A_CANIDX2_OFFSET	 0x05
#define CAN2A_CANIDX1_OFFSET	 0x03

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 3:03:17 PM Jul 30, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
CAN_ID_type Get_CAN_ID(void){
	CAN_ID_type idcan=0;
#if CAN_VERSION_SUPPORTED == CAN_VERSION_2A

	idcan=((CAN_ID_type)((CANIDT1 << CAN2A_CANIDX1_OFFSET) & MASK_CAN2A_CANIDX1)) + ((CAN_ID_type)( MASK_CAN2A_CANIDX2 &(CANIDT2 >> CAN2A_CANIDX2_OFFSET)));

#else
#error " CAN Identifier Tag Registers V2.0 part B is not supported"
#endif

	return idcan;
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 3:03:24 PM Jul 30, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
CAN_ID_type Get_Mask_ID(void){
	CAN_ID_type mask_idcan=0;
#if CAN_VERSION_SUPPORTED == CAN_VERSION_2A
	mask_idcan=((CAN_ID_type)((CANIDM1 << CAN2A_CANIDX1_OFFSET) & MASK_CAN2A_CANIDX1)) + ((CAN_ID_type)( MASK_CAN2A_CANIDX2 &(CANIDM2 >> CAN2A_CANIDX2_OFFSET)));
#else
#error " CAN Identifier Tag Registers V2.0 part B is not supported"
#endif

	return mask_idcan;
}

/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 3:03:28 PM Jul 30, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void Can_Set_MSG_ID(CAN_ID_type input_id){
	uint8_t idreg=0;
#if CAN_VERSION_SUPPORTED == CAN_VERSION_2A
	idreg= (uint8_t)(0xFF & (input_id >> 3));
	CANIDT1=idreg;
	idreg= (uint8_t)(0xE0 & (input_id << 5));
	CANIDT2=idreg;
#else
#error " CAN Identifier Tag Registers V2.0 part B is not supported"
#endif
	return;
}
/**********************************************************************************//**
 *  \param[in] param Description of the  parameter of the function.
 *  \param[out] param Description of the  parameter of the function.
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 3:03:32 PM Jul 30, 2015
 *  \warning Warning.
 *  \author Author
 ****************************************************************************************/
void Can_Set_IDMask(CAN_ID_type mask_id){
	uint8_t mask_reg=0;
#if CAN_VERSION_SUPPORTED == CAN_VERSION_2A
	mask_reg= (0xFF & (mask_id >> 3));
	mask_reg= (0xE0 & (mask_id << 5));

	mask_reg= (uint8_t)(0xFF & (mask_id >> 3));
	CANIDM1=mask_reg;
	mask_reg= (uint8_t)(0xE0 & (mask_id << 5));
	CANIDM2=mask_reg;
#else
#error " CAN Identifier Tag Registers V2.0 part B is not supported"
#endif
}




/**********************************************************************************//**
 *  \return error_tx_counter
 *  \brief Get Transmit Error Counter Register
 *  \see 19.10.14 CAN Transmit Error Counter Register
 *  \note First Implementation 3:43:12 PM Jul 30, 2015
 *  \author E.Sanchez
 ****************************************************************************************/
Can_err_type Get_CAN_Tx_error(void){
	uint8_t error_tx_counter=0;
	error_tx_counter=CANTEC;
	return error_tx_counter;
}

/**********************************************************************************//**
 *  \return error_rx_counter
 *  \brief Get Receive Error Counter
 *  \see 19.10.15 CAN Receive Error Counter Register
 *  \note First Implementation 3:44:07 PM Jul 30, 2015
 *  \author E.Sanchez
 ****************************************************************************************/
Can_err_type Get_CAN_Rx_error(void){
	uint8_t error_rx_counter=0;
	error_rx_counter=CANREC;
	return error_rx_counter;
}

/**********************************************************************************//**
 *  \return time_can
 *  \brief Read internal can timmer
 *  \note First Implementation 3:49:02 PM Jul 30, 2015
 *  \author E.Sanchez
 ****************************************************************************************/
Can_timmer_type Get_CAN_Timmer(void){
	Can_timmer_type time_can=0;
	time_can += (CANTIMH << 8 );
	time_can += (CANTIML);

	return time_can;
}


/**********************************************************************************//**
 *  \return can_ttc_timer_read
 *  \brief Read TTC Timer
 *  \note First Implementation 3:49:37 PM Jul 30, 2015
 *  \author E.Sanchez
 ****************************************************************************************/
Can_timmer_type Get_CAN_TTC_Timmer(void){
	Can_timmer_type can_ttc_timer_read;
	can_ttc_timer_read=0;
	can_ttc_timer_read += (CANTTCH << 8 );
	can_ttc_timer_read += (CANTTCL);
	return can_ttc_timer_read;
}


can_tstamp Get_Can_Time_Stamp(void){
	can_tstamp stamp=0;
	stamp += (CANSTMH << 8 );
	stamp += (CANSTMH);
	return stamp;
}
#define MAIL_BOX_MAX			15

void Can_Driver_Init(void){
	uint8_t mob=0;

	can_ctrl_reset_controller();
	can_ctrl_standby_controller();
	/* CAN General Control Register */
	for (mob = 0; mob < MAIL_BOX_MAX; mob++){
		// Set the MOb page number for each MOb 0 to 14 (15 MObs total)
		//CANPAGE = (mob << MOBNB0);
		Set_MailBox(mob);
		// Set each MOb configuration to disabled
		can_mail_box_disable();
		//CANCDMOB = 0;
		// Clear all the MOb interrupt/polling flags
		can_mob_clear_status_flags();
		//CANSTMOB = 0;
	}

	/*BAUDRATE CONFIG*/
	CANBT1 = 0x06;
	CANBT2 = 0x04;
	CANBT3 = 0x13;

	//TODO: Check !!
	//can_ctrl_enable_controller();
	can_ctrl_standby_controller();
}

void CAN_MOB_Interrupt_Control(uint8_t mob,uint8_t enable){

	if(mob > 7){
		CANIE1|=(enable << mob);
	}else{
		CANIE2|=(enable << mob);
	}
	return;
}

void CAN_MOB_SimpleMode_Set(CAN_Mode_type mode){
	can_config_mail_box_mode(mode);
}
uint8_t CAN_Get_Status_Interrupt(uint8_t mob){
	uint8_t status_interrupt;

	status_interrupt=0;

	if(mob > 7){
		status_interrupt=( CANIE2 & (1<<mob));
	}else{
		status_interrupt=( CANIE1 & (1<<mob));
	}
	return status_interrupt;
}

void Can_Service_Interrupt_control(void){
#if CONFIG_ALL_INTERRUPTS == 1
	ENABLE_ALL_INTERRUPTS();
#else
	DISABLE_ALL_INTERRUPTS();
#endif

#if CONFIG_BUS_OFF_INTERRUPT == 1
	ENABLE_BUS_OFF_INTERRUPT();
#else
	DISABLE_BUS_OFF_INTERRUPT();
#endif

#if CONFIG_CAN_RECEIVE_INTERRUPT == 1
	ENABLE_CAN_RECEIVE_INTERRUPT();
#else
	DISABLE_CAN_RECEIVE_INTERRUPT();
#endif

#if CONFIG_CAN_TX_INTERRUPT == 1
	ENABLE_CAN_TX_INTERRUPT();
#else
	DISABLE_CAN_TX_INTERRUPT();
#endif

#if CONFIG_MOB_ERRORS_INTERRUPT == 1
	ENABLE_MOB_ERRORS_INTERRUPT();
#else
	DISABLE_MOB_ERRORS_INTERRUPT();
#endif

#if CONFIG_FRAME_BUFFER_INTERRUPT == 1
	ENABLE_FRAME_BUFFER_INTERRUPT();
#else
	DISABLE_FRAME_BUFFER_INTERRUPT();
#endif

#if CONFIG_GENERAL_ERRORS_INTERRUPT == 1
	ENABLE_GENERAL_ERRORS_INTERRUPT();
#else
	DISABLE_GENERAL_ERRORS_INTERRUPT();
#endif

#if CONFIG_CAN_TIMER_OVERRUN_INTERRUPT == 1
	ENABLE_CAN_TIMER_OVERRUN_INTERRUPT();
#else
	DISABLE_CAN_TIMER_OVERRUN_INTERRUPT();

#endif
	return;
}

can_status Can_Tx_Msg(MBox_type mailbox ,CAN_ID_type msg_id ,can_dlc_type dlc,can_message_data * buffer){
	can_dlc_type icounter=0;
	can_status status=CAN_SUCESS;

	DISABLE_CAN_RECEIVE_INTERRUPT();
	if(dlc <= 8){
		Set_MailBox(mailbox);
		can_set_dlc(dlc);
		Can_Set_MSG_ID(msg_id);
		for(icounter=0; icounter<dlc; icounter++){
			CANMSG=buffer[icounter];
		}
	}else{
		status=CAN_ERROR;
	}
	ENABLE_CAN_RECEIVE_INTERRUPT();
	return status;
}
void Can_Configure_MailBox(MBox_type mailbox,CAN_Mode_type mode,can_dlc_type data_l,CAN_ID_type mbox_id,CAN_ID_type mbox_mask,can_int_ctrl enable_int){
	switch (mode) {


	case MAIL_BOX_ENABLE_RX:
		Set_MailBox(mailbox);
		CAN_MOB_SimpleMode_Set(mode);
		can_set_dlc(data_l);
		Can_Set_MSG_ID(mbox_id);
		Can_Set_IDMask(mbox_mask);
		CAN_MOB_Interrupt_Control(mailbox,enable_int);
		break;
	case MAIL_BOX_ENABLE_TX:
		Set_MailBox(mailbox);
		CAN_MOB_SimpleMode_Set(mode);
		can_set_dlc(data_l);
		Can_Set_MSG_ID(mbox_id);
		Can_Set_IDMask(mbox_mask);
		break;

	case MAIL_BOX_DISABLE:
	default:

		break;
	}


}
/*******************************************************************************************/
/*    F U N C T I O N   P R O T O T Y P E S                                                */
/*******************************************************************************************/

/**
 * @}
 */
