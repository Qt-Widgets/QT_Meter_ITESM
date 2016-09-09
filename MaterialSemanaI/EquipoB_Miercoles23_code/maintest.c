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

	 int test
	 tachoProcess(data, &tacho);
		   /*  IGNF=0;
	         printf("hola");
			if( init()==SUCCESS){
				//

				id_Timer_Animate=raspi_timer_create(50, gaugeAnimate);
				raspi_timer_start(id_Timer_Animate);
				if( ( raspi_gui_server_accept()==SUCCESS) )
				{
						 if ( rapspi_can_msg_queue_start()==SUCCESS )
						 {
							    raspi_gui_server_send_ignition(1);
							    IGNF=1;
							 	usleep(10000);
							 	tacho.targetValue=120;
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
	printf("Queue init succes");
	if(raspi_gui_server_init()!=SUCCESS)
		return -1;
	printf("Server init succes");
	if(raspi_timer_init_framework()!=SUCCESS)
		return -1;
	printf("Timer init succes");

	return 0;
}



int gaugeAnimate(int id){
	if(IGNF){
		int error=tacho.targetValue-tacho.displayValue;
		printf("Error %i\n", error);
		double m=error*.01;
		unsigned short aux=(unsigned short)tacho.targetValue;
		printf("Manipulated Var %d\n", m);

		if(error!=0 && (error>100 || error<-100)){
			tacho.displayValue=tacho.displayValue+m;
		}
		else{
			tacho.displayValue=tacho.targetValue;
		}

       aux = 5000;
		raspi_gui_server_send_gauge(ID_GAGE_TACHO, aux ,0 );
	}



	return id;

}*/

