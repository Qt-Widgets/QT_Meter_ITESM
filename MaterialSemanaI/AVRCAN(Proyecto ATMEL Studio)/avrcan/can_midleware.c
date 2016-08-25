/****************************************************************** 
 * can_midleware.c
 * Created on: Aug 3, 2015
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
#ifndef can_midleware_C_
#define can_midleware_C_
#endif
/**
 * \addtogroup can_midleware_module
 * @{
 */

/******************************************************************/
/*    I N C L U D E   F I L E S                                   */
/******************************************************************/
#include <can_midleware.h>
#include <can_driver.h>
#include <cpu.h>
#include <string.h>
volatile can_Ir_lock mutex_reg;
volatile can_event	event_can;
volatile can_event	event_can_dummy;
volatile MBox_type mail_box_no;

volatile CanMW_MBox MboxControlReg[ NUM_MAILBOXES ];

#define Event_Set_ISR()			event_can=1

#define Set_Event_to_ISRAPP()		event_can_dummy=1
#define Clr_Event_to_ISRAPP()		event_can_dummy=0
#define Get_Event_to_ISRAPP()		(event_can_dummy)

#define CAN_MIDLEWARE_RX_MESSAGES_FILL_ON_INT	1
#define CAN_MIDLEWARE_SECURE_MAILBOX_WRITE		1

/*******************************************************************************************/
/*    M A C R O S                                                                          */
/*******************************************************************************************/

/**********************************************************************************//**
 *  \param[in] MBox_type MailBox number.
 *  \return void
 *  \brief CanMw_MailBox_Rx_Handler
 *  \author E.Sanchez
 ****************************************************************************************/
void CanMw_MailBox_Rx_Handler(MBox_type mail_box_no){
		can_dlc_type dlcw_reg=0;
		can_dlc_type j=0;
		uint8_t place=0;
		can_message_data candata=0;
		Set_MailBox(mail_box_no);
		dlcw_reg=can_get_dlc();
		MboxControlReg[mail_box_no].msg.dlc=dlcw_reg;
		MboxControlReg[mail_box_no].msg.message_id=(CAN_ID_type)Get_CAN_ID();
		MboxControlReg[mail_box_no].msg.stamp=(can_tstamp)Get_Can_Time_Stamp();
		for(j=0; j<dlcw_reg; j++){
			place=Can_Get_FifoPosition();
			candata=CANMSG;
			MboxControlReg[mail_box_no].msg.data[place]=candata;
		}
		can_clear_flags_registers();
		return;
}

/**********************************************************************************//**
 *  \param[void] None
 *  \return 0
 *  \brief  CANRX callback service
 *  \note First Implementation 5:43:59 PM Aug 10, 2015
 *  \warning Warning.
 *  \author E.Sanchez
 ****************************************************************************************/
void CanMW_ReceivedRxHandle(void){
	mail_box_no=can_highest_priority_mob();

	#if CAN_MIDLEWARE_RX_MESSAGES_FILL_ON_INT == 1
	  CanMw_MailBox_Rx_Handler(mail_box_no);
	  Event_Set_ISR();
	#else
	  Set_Event_to_ISRAPP();
	#endif
}
/**********************************************************************************//**
 *  \brief Dummy Function
 *  \warning Not Functional !!.
 *  \author E.Sanchez
 ****************************************************************************************/
void CanMw_Update_App(void){
   #if CAN_MIDLEWARE_RX_MESSAGES_FILL_ON_INT == 1

   #else
   if(Get_Event_to_ISRAPP()){
	   mail_box_no=can_highest_priority_mob();
    CanMw_MailBox_Rx_Handler(mail_box_no);
	
    Event_Set_ISR();
   }
   #endif
}
/**********************************************************************************//**
 *  \param[void] None.
 *  \return 0
 *  \brief Initialization CAN API
 *  \note First Implementation 5:45:55 PM Aug 10, 2015
 *  \author E.Sanchez
 ****************************************************************************************/
void CanMW_APP_Init(void){
	event_can=0;
	event_can_dummy=0;
}

/**********************************************************************************//**
 *  \param[in] mailbox_num Mailbox Number
 *  \param[in] operation_mode Can Mailbox Mode
 *  \param[in] data_l CAN Message Length
 *  \param[in] msg_id CAN Message ID
 *  \param[in] mbox_mask CAN Mailbox Mask
 *  \param[in] enable_ints Interruption Type
 *  \param[in] time Time Stamp
 *  \return 0
 *  \brief Module Description
 *  \see See more information Here !!
 *  \note First Implementation 5:47:16 PM Aug 10, 2015
 *  \warning Warning.
 *  \author E.Sanchez
 ****************************************************************************************/
void CanMW_InitMail_Box(MBox_type mailbox_num,CAN_Mode_type operation_mode,can_dlc_type data_l,CAN_ID_type msg_id,CAN_ID_type mbox_mask,can_int_ctrl enable_ints,Can_timmer_type time)
{
	MboxControlReg[mailbox_num].mailbox=mailbox_num;
	MboxControlReg[mailbox_num].mode=operation_mode;
	MboxControlReg[mailbox_num].msg.dlc=data_l;
	MboxControlReg[mailbox_num].msg.ttc_timmer=time;
	MboxControlReg[mailbox_num].msg.message_id=msg_id;
	MboxControlReg[mailbox_num].conf_mask=mbox_mask;
	MboxControlReg[mailbox_num].enable_int=enable_ints;
	//TODO Change this :P
	Can_Configure_MailBox(mailbox_num,operation_mode,data_l,msg_id,mbox_mask,enable_ints);
}
uint8_t iCounter=0;

void CanMw_Send_TX_Messages(void){
	
		//for(iCounter=0;iCounter<3;iCounter++){
			if(iCounter<3){
				iCounter++;
				}else{
				iCounter=0;
			}
			if(MboxControlReg[iCounter].mode == MAIL_BOX_ENABLE_TX ){
				Can_Tx_Msg(MboxControlReg[iCounter].mailbox,MboxControlReg[iCounter].msg.message_id,MboxControlReg[iCounter].msg.dlc,MboxControlReg[iCounter].msg.data);
			}
		//}
		}

void CanMw_Send_TX_Message( MBox_type mailbox_num )
{
    if ( mailbox_num < NUM_MAILBOXES )
    {
        if( MboxControlReg[ mailbox_num ].mode == MAIL_BOX_ENABLE_TX )
        {
            Can_Tx_Msg( MboxControlReg[ mailbox_num ].mailbox,
                        MboxControlReg[ mailbox_num ].msg.message_id,
                        MboxControlReg[ mailbox_num ].msg.dlc,
                        MboxControlReg[ mailbox_num ].msg.data );
        }
    }

    return;
}

void CanMW_ReSync_Driver(void){
	uint8_t iCounter;

	for(iCounter=0;iCounter<3;iCounter++){
		//
		if(MboxControlReg[iCounter].mode == MAIL_BOX_DISABLE || MboxControlReg[iCounter].mode == MAIL_BOX_ENABLE_TX){
					
		}else{
		Can_Configure_MailBox(
				MboxControlReg[iCounter].mailbox,
				MboxControlReg[iCounter].mode,
				MboxControlReg[iCounter].msg.dlc,
				MboxControlReg[iCounter].msg.message_id,
				MboxControlReg[iCounter].conf_mask,
				MboxControlReg[iCounter].enable_int
				);
		}
	}
}
void CanMW_ReConf_Driver(void){
	uint8_t iCounter;

	for(iCounter=0;iCounter<3;iCounter++){
		//
		if(MboxControlReg[iCounter].mode == MAIL_BOX_DISABLE){
			
			}else{
			Can_Configure_MailBox(
			MboxControlReg[iCounter].mailbox,
			MboxControlReg[iCounter].mode,
			MboxControlReg[iCounter].msg.dlc,
			MboxControlReg[iCounter].msg.message_id,
			MboxControlReg[iCounter].conf_mask,
			MboxControlReg[iCounter].enable_int
			);
		}
	}
}
void CanMW_set_mutex(can_Ir_lock state){
	mutex_reg=state;
}
can_Ir_lock CanMW_get_mutex(void){
	return mutex_reg;
}

can_event CanMW_Event_Get(void){
	return event_can;
}
void CanMW_Event_Clean(void){
	event_can=0;
}

void Get_Message_FromMbox(MBox_type mail,CanMw_Message * message){
	memcpy(message,(void*)&MboxControlReg[mail].msg,sizeof(CanMw_Message));
}

void Get_MboxConfig( MBox_type mail, CanMW_MBox * MboxConfig ){
    memcpy(MboxConfig,(void*)&MboxControlReg[mail],sizeof(CanMW_MBox));
}

void Write_Message_Mbox(MBox_type mail,CanMW_MBox  * mail_box){
	#if CAN_MIDLEWARE_SECURE_MAILBOX_WRITE == 1
	Can_Int_Transactions_Off();
	#endif
	memcpy((void*)&MboxControlReg[mail],(void*)mail_box,sizeof(CanMW_MBox));
	#if CAN_MIDLEWARE_SECURE_MAILBOX_WRITE == 1
	Can_Int_Transactions_On();
	#endif
}

/*******************************************************************************************/
/*    F U N C T I O N   P R O T O T Y P E S                                                */
/*******************************************************************************************/

/**
 * @}
 */
