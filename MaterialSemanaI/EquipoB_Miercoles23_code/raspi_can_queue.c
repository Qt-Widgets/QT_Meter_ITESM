#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
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

typedef struct 
{
    can_msg queue[MAX_QUEUE_SIZE];
    sem_t* queueSem;
    int msgQueueIdx;
   
    raspi_can_ctx ctx;
    pthread_t pollThread; 
    int sigStop;
}msgQueue_t;

msgQueue_t msgQueue;

static void* raspi_can_poll_thr( void* arg );
static int raspi_can_msg_enqueue( can_msg* msg );

//methods  for using with internal msg queue
#define CAN_QUEUE_SEM_NAME "canQueueSem"
int raspi_can_msg_queue_init( void )
{
    int ret = -1;
    
    if( raspi_can_init( &msgQueue.ctx  ) == RASPI_CAN_OK )
    {
        sem_unlink( CAN_QUEUE_SEM_NAME );
        msgQueue.queueSem = sem_open( CAN_QUEUE_SEM_NAME, O_CREAT, O_RDWR, 1 );
        msgQueue.msgQueueIdx = 0;
        memset(&msgQueue.queue[0], 0x00, ( sizeof(can_msg) * MAX_QUEUE_SIZE ) );
        msgQueue.sigStop = 0;
        ret = 0;
    }
    else
    {
        printf("unable to init can queue");
    }
    
    return ret;
}



int raspi_can_msg_queue_size( void )
{
    int ret = 0;

    sem_wait( msgQueue.queueSem );
    ret = msgQueue.msgQueueIdx;
    sem_post( msgQueue.queueSem );
    return ret;
}

// message receive thread
int rapspi_can_msg_queue_start( void )
{
    int ret = -1;
    if ( pthread_create( &(msgQueue.pollThread), NULL, &raspi_can_poll_thr, NULL ) == 0 )
    {
        ret = 0;
        printf( "loging thread succesfull intiialization" );
    }

    return ret;
}

void raspi_can_msg_queue_stop( void )
{

    sem_wait( msgQueue.queueSem );    
    msgQueue.sigStop = true;
    // signal polling thread close
    if( pthread_join( msgQueue.pollThread, NULL ) == 0)
    {
        raspi_can_close( &msgQueue.ctx );
        msgQueue.msgQueueIdx = 0;
        sem_close( msgQueue.queueSem );
    }

    sem_post( msgQueue.queueSem );
}

int raspi_can_msg_dequeue( can_msg* msg )
{
    int ret = 1;

    if ( msgQueue.msgQueueIdx > 0 &&
        msg != NULL ) 
    {
        sem_wait( msgQueue.queueSem );
        msgQueue.msgQueueIdx--;
        memcpy( msg, &msgQueue.queue[ msgQueue.msgQueueIdx ], sizeof(can_msg) );
        sem_post( msgQueue.queueSem );

        ret = 0;
    }

    return ret;
}

static int raspi_can_msg_enqueue( can_msg* msg )
{
    int ret = 1;
    if( msgQueue.msgQueueIdx < MAX_QUEUE_SIZE )
    {
        sem_wait( msgQueue.queueSem );
        memcpy( &msgQueue.queue[ msgQueue.msgQueueIdx ], msg, sizeof(can_msg) );
        msgQueue.msgQueueIdx++;
        sem_post( msgQueue.queueSem );
        ret = 0;
    }
    return ret;
}

static void* raspi_can_poll_thr( void* arg )
{
    int i = 0;
    int recv = 0;
    can_msg msg[ MAX_POLL_BUFF_SIZE ];

    while(  msgQueue.sigStop == false )
    {
        // entire buffer available for filling
        recv =  raspi_can_poll( &msgQueue.ctx, MAX_POLL_BUFF_SIZE, msg ); 

        if ( recv > 0 )
        {
            //printf( "receiving %d can messages", recv );
            for(  i = 0 ;  i < recv ; i++ )
            {
                raspi_can_msg_enqueue( &msg[i] );
            }
        }

        usleep( CAN_TH_POLL_RATE );
    }

    pthread_exit( NULL );

    return NULL;
}
