
#include "gauge.h"

char idT,idTOFF;
int tachoTimeOut(int id);
int tachoTimeOFF(int id);

void getData(can_msg* msg){
		memset( msg, 0x00, sizeof(can_msg));
		raspi_can_msg_dequeue(msg);
			dataProcessing(msg);
}

void dataProcessing(can_msg* msg){

	short int data;
	short int data2;
	data=0;



    if(msg->msgID!=MSG_CAN_ID_IGN_1 ){
    	if(IGNF){
    		if(msg->msgID==MSG_CAN_ID_TACHO){
    			if(msg->period!=0){
					data= (int)((msg->data[1]&0x7F)<<7) | (char)((msg->data[2]>>1)&0x7F);
					//tachoProcess(data, &tacho);
    			}
    			else{
    				idT=raspi_timer_create(2000, tachoTimeOut);
    				raspi_timer_start(idT);
    			}
    			printf("Gauge : Tacho Received\n");

    		}else{
    			//static int test=0;   //test
    			//tachoProcess(test++,&tacho);
    			printf("Gauge : %x\n",msg->msgID);
    		}
    	}
    	else{
    		// no hacer nada...la pantalla esta apagada
    	}
    }else{
    	data2= msg->data[1]&0x01;
    	//ignProcess(data2);
    }


}

void tachoProcess(int data, Gauge *tacho){
	if(data<10000){
		if(data>7000){
			//DATO PELIGRO Blinking
			if(data>8000){ //Exceso Rango Display Mandar 8000 Blinking
			tacho->targetValue=8000;
			}
		}else{
			tacho->targetValue=data;
		}
	}else {
		tacho->targetValue=0;//ERROR
	}
}

void ignProcess(int data){
	IGNF=data;
	raspi_gui_server_send_ignition(data);
	if(data==0){
		idTOFF=raspi_timer_create(5000, tachoTimeOFF);
		raspi_timer_start(idTOFF);
	}
}
int tachoTimeOut(int id){
	raspi_gui_server_send_ignition(0);
	raspi_timer_stop(idT);
}
int tachoTimeOFF(int id){
	raspi_gui_server_send_ignition(0);
	raspi_timer_stop(idTOFF);
}


/*int tachoTimeOut(int id){
	tacho->fastSpeed=1;
	tacho->targetValue=0;
return tacho->fastSpeed;
}*/

void gaugeInit(Gauge *g){
	g->displayValue=0;
	g->timeStamp=0;
	g->targetValue=0;
	g->errFlag=0;
	g->timeOutFlag=0;
	g->bFLag=0;
	g->bDisp=0;
	g->fastSpeed=0;
	g->msgId=0;
}


