#include <stdio.h>
#include <stdlib.h>
#include "raspi_candrv.h"
#include "raspi_gui_ipc.h"
#include "raspi_ipc_defs.h"
#include "raspi_timer.h"
#include "gauge.h"
#include "raspi_can_queue.h"

#define SUCCESS 0

int gaugeAnimate(int id);
char init();

int keep_going = 1;
can_msg msg;
char id_Timer_Animate;



int main(void) {
	gaugeInit(&tacho);
	     IGNF=0;
         printf("hola/n");
		if( init()==SUCCESS){
			//

			id_Timer_Animate=raspi_timer_create(50, gaugeAnimate);

			if( ( raspi_gui_server_accept()==SUCCESS) )
			{
					 if ( rapspi_can_msg_queue_start()==SUCCESS )
					 {
						    raspi_gui_server_send_ignition(1);
						    IGNF=1;
						    raspi_gui_server_send_gauge(ID_GAGE_TACHO, 8000 ,0 );
						    tacho.displayValue=8000;
						 	usleep(3000000);
						 	tacho.targetValue=100;
						 	raspi_timer_start(id_Timer_Animate);
						 	dataSampling();

					 }
					 else
					 {
						 printf( " unable to start msg queue \n");

					 }
			}
		}

	return EXIT_SUCCESS;
}

void dataSampling(){
  int queueSize = 0;
	while(keep_going){
		queueSize=raspi_can_msg_queue_size();
		int i;
		for(i=0; i<queueSize && queueSize>0; i++){//TEST
			getData(&msg);
		}

		usleep(10000);
	}
}




char init(){
	if(raspi_can_msg_queue_init()!=SUCCESS)
			return -1;
	printf("Queue init succes/n");
	if(raspi_gui_server_init()!=SUCCESS)
		return -1;
	printf("Server init succes/n");
	if(raspi_timer_init_framework()!=SUCCESS)
		return -1;
	printf("Timer init succes/n");

	return 0;
}



int gaugeAnimate(int id){
	if(IGNF){
		int error=tacho.targetValue-tacho.displayValue;
		//printf("Error %i\n", error);
		double m;
		if(abs(error)>3000)
			m=error*.08;
		else if (abs(error)>1000)
			m=error*.1;
		else
			m=error*.3;
		unsigned short aux=(unsigned short)tacho.targetValue;
		//printf("Manipulated Var %d\n", m);

		if(error!=0 && (error>50 || error<-50)){
			tacho.displayValue=tacho.displayValue+m;
		}
		else{
			tacho.displayValue=tacho.targetValue;
		}

       //aux = 5000;
		raspi_gui_server_send_gauge(ID_GAGE_TACHO, tacho.displayValue ,0 );
	}


	/*if(tacho.fastSpeed){
		//animate();
		raspi_gui_server_send_gauge(MSG_CAN_ID_TACHO, tacho.displayValue ,0 );
	}
	else if(count){
		count=0;
		//animate()
		raspi_gui_server_send_gauge(MSG_CAN_ID_TACHO, tacho.displayValue ,0 );
	}
	else
		count=count+1;//*/

	return id;

}

