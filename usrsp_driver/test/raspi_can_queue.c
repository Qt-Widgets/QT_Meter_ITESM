//#include <semaphore.h>
#include <unistd.h>
//#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include "raspi_candrv.h"
#include "raspi_can_queue.h"


// poll each 30000 usec 
#define CAN_TH_POLL_RATE 15000 
// just a linear buffer for now

#define true 1
#define false 0

double counter;
int step_no;


typedef struct 
{
    can_msg queue[MAX_QUEUE_SIZE];
    //sem_t* queueSem;
    int msgQueueIdx;
   
    raspi_can_ctx ctx;
    //pthread_t pollThread;
    int sigStop;
}msgQueue_t;

msgQueue_t msgQueue;

static void* raspi_can_poll_thr( void* arg );
static int raspi_can_msg_enqueue( can_msg* msg );

#include "test_case_01.h"

//methods  for using with internal msg queue
#define CAN_QUEUE_SEM_NAME "canQueueSem"
int raspi_can_msg_queue_init( void )
{
    int ret = 0;
    counter = COUNTER_CALL_MAX;
    step_no = 0;
    return ret;
}



int raspi_can_msg_queue_size( void )
{
    int ret = 0;

    return ret;
}

// message receive thread
int rapspi_can_msg_queue_start( void )
{
    int ret = 0;
    counter = COUNTER_CALL_MAX;
    step_no = 0;
    can_msg msg;
    while (1){
    	raspi_can_msg_dequeue(&msg);
    }
    return ret;
}

void raspi_can_msg_queue_stop( void )
{

}

int raspi_can_msg_dequeue( can_msg* msg )
{
    int ret = 0;
    if (counter > 0){
    	counter--;
    }else{
    	counter = COUNTER_CALL_MAX;

        if ((step_no+1) <= (MAX_STEPS)){
        	 memcpy( msg, &step_data[step_no], sizeof(can_msg) );
        	step_no++;
        }
    	printf("paso: %d , DLC: %d, cANID %02x \n", step_no, msg->DLC, msg->msgID);

  	}


    return ret;
}

static int raspi_can_msg_enqueue( can_msg* msg )
{
    int ret = 1;
    return ret;
}

static void* raspi_can_poll_thr( void* arg )
{

    return NULL;
}
