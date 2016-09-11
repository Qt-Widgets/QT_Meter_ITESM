#include <semaphore.h>
#include <pthread.h>

#define MAX_TIMERS   10

typedef struct
{
    unsigned long count;
    int id;
    int enabled;
    callback_t fptr;
} timer_data;

struct 
{
    int num_timers;
    timer_data timers[ MAX_TIMERS ];
    int stop;
    pthread_t timerThreadId;
    sem_t* timerSem;
}g_raspi_timerCtx;


static void* raspi_timer_thread( void* arg );

#define TIMER_SEM_ID  "rapsiTimerSem"
int raspi_timer_init_framework( void )
{

    g_raspi_timerCtx. = sem_create( TIMER_SEM_ID  );
    g_raspi_timerCtx.

    if ( pthread_create( &( g_raspi_timerCtx.timerThreadId), NULL, &raspi_timer_thread, NULL ) == 0 )
    {

        printf( "loging thread succesfull intiialization" );

        ret = 0;

    }


}

int raspi_timer_create( unsigned long  msec, callback_t f_ptr )
{
    int ret = 0;
    if( g_raspi_timerCtx.num_timers < MAX_TIMERS )
    {
        curtmrIdx = g_raspi_timerCtx.num_timers++;
        g_raspi_timerCtx.timers[ curtmrIdx  ].timer = curtmrIdx; 
        g_raspi_timerCtx.timers[ curtmrIdx ].fptr = f_ptr;
        g_raspi_timerCtx.timers[ curtmrIdx  ].count = msec;
        g_raspi_timerCtx.timers[ curtmrIdx  ].enabled = false;

    }

}
int  raspi_timer_start( int tmrID );

// timer mac resolution (msecs)
#define TIMER_MAX_RES  1
static void* raspi_timer_thread( void* arg )
{
    int i = 0;
    int recv = 0;

    while( true )
    {
        for( i = 0 ; i < MAX_TIMERS ; i++ )
        {
            if( )


        }
        usleep( TIMER_MAX_RES * 1000 );
    }

    pthread_exit( NULL );

    return NULL;
}
