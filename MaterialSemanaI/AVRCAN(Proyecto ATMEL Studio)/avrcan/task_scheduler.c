/*---------------------------------------------------------------------------*/
/**************************************************************************//**
 *  \file      task_scheduler.c
 *  \addtogroup timers
 *  @{
 *  \author    Roberto Alejandro Flores Estrada
 *  \since     06/08/2015
 *
 *****************************************************************************/

#include <task_scheduler.h>
#include <can_application.h>
#include <schedule_callbacks.h>

void schedulerEmptyfunction( void );

waveform periods[] =
{
        {  0,  0 },
        {  1,  1 },
        {  1,  2 },
        {  2,  2 },
        {  2,  3 },
        {  3,  3 },
        {  3,  4 },
        {  4,  4 },
        {  4,  5 },
        {  5,  5 },
        {  5,  6 },
        {  6,  6 },
        {  6,  7 },
        {  7,  7 },
        {  7,  8 },
        {  8,  8 },
        {  8,  9 },
        {  9,  9 },
        {  9, 10 },
        { 10, 10 },
        { 10, 11 },
        { 11, 11 },
        { 11, 12 },
        { 12, 12 },
        { 12, 13 },
        { 13, 13 },
        { 13, 14 },
        { 14, 14 },
        { 14, 15 },
        { 15, 15 },
        { 15, 16 },
        { 16, 16 },
        { 16, 17 },
        { 17, 17 },
        { 17, 18 },
        { 18, 18 },
        { 18, 19 },
        { 19, 19 },
        { 19, 20 },
        { 20, 20 },
        { 20, 21 },
        { 21, 21 },
        { 21, 22 },
        { 22, 22 },
        { 22, 23 },
        { 23, 23 },
        { 23, 24 },
        { 24, 24 },
        { 24, 25 },
        { 25, 25 },
        { 25, 26 },
        { 26, 26 },
        { 26, 27 },
        { 27, 27 },
        { 27, 28 },
        { 28, 28 },
        { 28, 29 },
        { 29, 29 },
        { 29, 30 },
        { 30, 30 },
        { 30, 31 },
        { 31, 31 },
        { 31, 32 },
        { 32, 32 },
        { 32, 33 },
        { 33, 33 },
        { 33, 34 },
        { 34, 34 },
        { 34, 35 },
        { 35, 35 },
        { 35, 36 },
        { 36, 36 },
        { 36, 37 },
        { 37, 37 },
        { 37, 38 },
        { 38, 38 },
        { 38, 39 },
        { 39, 39 },
        { 39, 40 },
        { 40, 40 }
};


schedulerItem schedule[] =
{
//   active    waveform   state      count             function
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox0  },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox1  },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox2  },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox3  },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox4  },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox5  },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox6  },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox7  },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox8  },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox9  },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox10 },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox11 },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox12 },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox13 },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox14 },
    { FALSE, &periods[0], FALSE, SCH_COUNT_INIT, schedule_callback_Mailbox15 },
};

#define SCHEDULE_NUM_ITEMS   ( sizeof( schedule ) / sizeof( schedulerItem ) )
/*---------------------------------------------------------------------------*/

/**************************************************************************//**
 * \fn
 * \brief
 * \return
 *****************************************************************************/
void schedulerSetupItem( uint8_t index, uint8_t active, sch_time_list time )
{
    schedule[ index ].active = active;
    schedule[ index ].dty_cycles = &periods[ time];
    return;
}


/**************************************************************************//**
 * \fn      void schedulerInit( void )
 * \brief   Setups TMR0 as a 20ms period timer.
 * \return
 *****************************************************************************/
void schedulerInit( void )
{
    tmr0At90can128Init( SCHEDULER_PRESCALER_OPT,
                        SCHEDULER_TMR_WFG,
                        SCHEDULER_TMR_MODE );
    tmr0At90can128SetTop( ( uint8_t ) SCHEDULER_COMP_REG );
    tmr0At90can128SetCount( SCH_COUNT_INIT );
    tmr0At90can128SetupOC0A( TRUE );
    return;
}

/**************************************************************************//**
 * \fn      void msgSchedulerEnableInterrupt( void )
 * \return
 *****************************************************************************/
void schedulerEnableInterrupt( void )
{
    tmr0At90can128EnableInterrupts( SCHEDULER_INTERRUPT_EN );
    return;
}

/**************************************************************************//**
 * \fn      void schedulerEventHandler( void )
 * \return
 *****************************************************************************/
void schedulerEventHandler( void )
{
    uint8_t index = 0;              // Schedule index used to reference
                                    // its items.
    uint8_t timeElapsed = FALSE;    // States if the schedule item's state
                                    // should change.
    uint8_t count = SCH_COUNT_INIT; // An item's current count.
    uint8_t state = SCH_LOW_STATE;  // An item's current state;
    // This function shall be called only if the interrupt is disabled but
    // the flag must be active.
	if ( !( tmr0At90can128areInterruptsEnabled( ) & SCHEDULER_INTERRUPT_EN )
			&& ( tmr0At90can128isOCF0AOn( ) ) )
    {
        tmr0At90can128ClearOCF0A( ); //turn off the interrupt flag.
        for ( ; index < SCHEDULE_NUM_ITEMS; index++ )
        {
            if ( !schedule[ index ].active )
            {
                continue;
            }
            count = schedule[ index ].count;
            state = schedule[ index ].state;
            switch ( state )
            {
                case SCH_HIGH_STATE:
                    timeElapsed =
                            ( schedule[ index ].dty_cycles->high_cycles
                              <= count );
                    break;
                case SCH_LOW_STATE:
                    timeElapsed =
                            ( schedule[ index ].dty_cycles->low_cycles
                              <= count );
                    break;
                default:
                    break;
            }
            if ( timeElapsed )
            {
                count = SCH_COUNT_INIT;
                state = !state;
                if ( state )
                {
                    // At every positive edge transition the item's
                    // function is called.
                    ( schedule[ index ].function )(  );
                }
                schedule[ index ].state = state;
            }
            count++;
            schedule[ index ].count = count;
        }
    }
	return;
}

/**************************************************************************//**
 * \fn      void schedulerEventHandler( void )
 * \brief   An empty function which is used as a dummy callback for those
 *          time events that has not associated functions.
 * \return
 *****************************************************************************/
void schedulerEmptyfunction( void )
{
    /* NO OPERATION */
	return;
}

/**
 * @}
 */
/*---------------------------------------------------------------------------*/
