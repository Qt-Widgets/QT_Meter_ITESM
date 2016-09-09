#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include "raspi_timer.h"

#define MAX_TIMERS   10

#define true 1
#define false 0

typedef struct
{
    unsigned long set;
    unsigned long count;
    int id;
    int enabled;
    callback_t fptr;
} timer_data;

struct gtimer
{
    int enable;
    int num_timers;
    timer_data timers[ MAX_TIMERS ];
    int stop;
    pthread_t timerThreadId;
    sem_t* timerSem;
}g_raspi_timerCtx;


static void* raspi_timer_thread( void* arg );

#define TIMER_SEM_NAME  "rapsiTimerSem"
int raspi_timer_init_framework( void )
{
    int ret = -1;

    sem_unlink( TIMER_SEM_NAME );
    g_raspi_timerCtx.timerSem = sem_open( TIMER_SEM_NAME, O_CREAT, O_RDWR, 1 );
    g_raspi_timerCtx.num_timers = 0;
    g_raspi_timerCtx.enable = true;

    if ( pthread_create( &g_raspi_timerCtx.timerThreadId, NULL, &raspi_timer_thread, NULL ) == 0 )
    {
        //printf( "timer thread succesfull intiialization" );
        ret = 0;
    }
   return ret;
}

void raspi_timer_close_framework( void )
{
    sem_wait(  g_raspi_timerCtx.timerSem );
    g_raspi_timerCtx.enable = false;
    pthread_join( g_raspi_timerCtx.timerThreadId, NULL );
}


int raspi_timer_create( unsigned long  msec, callback_t f_ptr )
{
    int ret = 0;
    int curtmrIdx = 0;

    sem_wait( g_raspi_timerCtx.timerSem );

    if( g_raspi_timerCtx.num_timers < MAX_TIMERS )
    {
        curtmrIdx = g_raspi_timerCtx.num_timers++;
        g_raspi_timerCtx.timers[ curtmrIdx  ].id = curtmrIdx; 
        g_raspi_timerCtx.timers[ curtmrIdx ].fptr = f_ptr;
        g_raspi_timerCtx.timers[ curtmrIdx  ].count = msec;
        g_raspi_timerCtx.timers[ curtmrIdx  ].set = msec;
        g_raspi_timerCtx.timers[ curtmrIdx  ].enabled = false;

        ret = curtmrIdx;
    }

    sem_post( g_raspi_timerCtx.timerSem );

    return ret;
}

int  raspi_timer_start( int tmrID )
{
    int ret = 0;

    sem_wait(  g_raspi_timerCtx.timerSem  );
    g_raspi_timerCtx.timers[ tmrID ].enabled = true;
    sem_post( g_raspi_timerCtx.timerSem );

    return ret;
}

int  raspi_timer_stop( int tmrID )
{
    int ret = 0;

    sem_wait(  g_raspi_timerCtx.timerSem  );
    g_raspi_timerCtx.timers[ tmrID ].enabled = false;
    sem_post( g_raspi_timerCtx.timerSem );

    return ret;
}


// timer mac resolution (msecs)
#define TIMER_MAX_RES  1
static void* raspi_timer_thread( void* arg )
{
    int i = 0;

    while( g_raspi_timerCtx.enable == true )
    {
        sem_wait( g_raspi_timerCtx.timerSem );
        for( i = 0 ; i < g_raspi_timerCtx.num_timers; i++ )
        {
            timer_data* data = NULL;
            //memcpy( &data, &( g_raspi_timerCtx.timers[ i ] ), sizeof( timer_data ) );
            data = &( g_raspi_timerCtx.timers[ i ] );

            if( data->enabled == true )
            {
                data->count--;
               
                if( data->count == 0x00 )
                {
                    data->count = data->set;
                    data->fptr( data->id );
                }
            }

        }

        sem_post( g_raspi_timerCtx.timerSem );
        // timer resolution 1 msec
        usleep( TIMER_MAX_RES * 1000 );
    }

    pthread_exit( NULL );

    return NULL;
}

