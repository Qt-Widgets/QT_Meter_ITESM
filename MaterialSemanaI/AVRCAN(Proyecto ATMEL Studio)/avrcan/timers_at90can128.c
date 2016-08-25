/*---------------------------------------------------------------------------*/
/**************************************************************************//**
 *  \file      timers_at90can128.c
 * \addtogroup timers
 * @{
 *  \author    Roberto Alejandro Flores Estrada
 *  \since     31/07/2015
 *
 *****************************************************************************/

#include <timers_at90can128.h>

/**************************************************************************//**
 * \fn void tmr0At90can128Init( uint8_t clksrc, uint8_t wfg, uint8_t compm )
 *
 * \warning A prescaler reset affects all timers./n
 * 			Changing
 *****************************************************************************/
void tmr0At90can128Init( uint8_t clksrc, uint8_t wfg, uint8_t compm )
{
	clksrc &= TMRN_CLK_SRC_MASK;
	compm   = TMR0_COMP_SETUP;
	if ( wfg == TMR0_WFG_FPWM )
	{
		wfg = TMR0_FAST_PWM;
	}
	else if ( wfg == TMR0_WFG_CTC )
	{
		wfg = TMR0_CLR_TIME_COMP;
	}
	else if ( wfg == TMR0_WFG_PHC_PWM )
	{
		wfg = TMR0_PHASE_CHG_PWM;
	}
	else
	{
		wfg = TMR0_NORMAL_MODE;
	}
    TCCR0A  =  wfg | clksrc | compm;

    return;
}

/**************************************************************************//**
 * \fn      void tmr0At90can128EnableInterrupts( uint8_t val )
 * \param   val uint8_t Bit 0 -> TOIE0/n
 *                      Bit 1 -> OCIE0A
 * \brief   Enables/Disables Overflow and Comparator Interrupts for Timer 0
 * \warning
 * \return
 *****************************************************************************/
void tmr0At90can128EnableInterrupts( uint8_t val )
{
	TIMSK0 = val;

	return;
}
/**************************************************************************//**
 * \fn      uint8_t tmr0At90can128areInterruptsEnabled( void )
 * \return  uint8_t Bit 0 -> TOIE0/n
 *                  Bit 1 -> OCIE0A
 *****************************************************************************/
uint8_t tmr0At90can128areInterruptsEnabled( void )
{
    uint8_t ret = TIMSK0;

    return ret;
}

/**************************************************************************//**
 * \fn     uint8_t tmr0At90can128GetCount( void )
 * \param
 * \brief
 * \return uint8_t
 *****************************************************************************/
uint8_t tmr0At90can128GetCount( void )
{
	return TCNT0;
}

/**************************************************************************//**
 * \fn      void tmr0At90can128SetCount( uint8_t val )
 * \param   val uint8_t New count value
 * \brief   Updates the current count
 * \warning Changing it while the timer is on might cause to be triggered
 * 			immediately.
 * \return
 *****************************************************************************/
void tmr0At90can128SetCount( uint8_t val )
{
	TCNT0 = val;

	return;
}

/**************************************************************************//**
 * \fn     uint8_t tmr0At90can128GetTop( void )
 * \param
 * \brief
 * \return uint8_t
 *****************************************************************************/
uint8_t tmr0At90can128GetTop( void )
{
	uint8_t ret = OCR0A;

	return ret;
}

/**************************************************************************//**
 * \fn    void tmr0At90can128SetTop( uint8_t val )
 * \param val uint8_t New Top
 * \brief
 * \return
 *****************************************************************************/
void tmr0At90can128SetTop( uint8_t val )
{
	OCR0A = val;

	return;
}

/**************************************************************************//**
 * \fn    void tmr0At90can128SetupOC0A( uint8_t dir )
 * \param
 * \brief
 * \return
 *****************************************************************************/
void tmr0At90can128SetupOC0A( uint8_t dir )
{
	if ( !dir )
	{
        INPUT_OC0A( );
	}
	else
	{
		OUTPUT_OC0A( );
	}
}

/**************************************************************************//**
 * \fn    void tmr0At90can128SetupT0( uint8_t dir )
 * \param dir uint8_t External clock pin T0 direction set
 * \brief
 * \note  T0 can be defined as output, so that the software
 *        may take control of the timer events.
 * \return
 *****************************************************************************/
void tmr0At90can128SetupT0( uint8_t dir )
{
	if ( !dir )
	{
        INPUT_T0( );
	}
	else
	{
		OUTPUT_T0( );
	}
}

/**************************************************************************//**
 * \fn      uint8_t tmr0At90can128isOCF0AOn( void )
 * \brief   Return the value of OCF0A flag in TIF0.
 * \note    OCF0A in TIFR0 is cleared when its ISR is handled or by writing a
 *          logic one on it. It is set when a compare match occurs between
 *		    TCNT0 and 0CR0A.
 * \warning I-bit in SREG shall be on in order to execute the Compare match
 *          interrupt. This function is recommended when handling interrupts
 *          by polling
 * \return  uint8_t
 *****************************************************************************/
uint8_t tmr0At90can128isOCF0AOn( void )
{
    uint8_t ret = FALSE;

    ret = TIFR0 & ( TRUE << ( OCF0A ) );

    return ret;
}

/**************************************************************************//**
 * \fn      void tmr0At90can128ClearOCF0A( void )
 * \brief
 * \return
 *****************************************************************************/
void tmr0At90can128ClearOCF0A( void )
{
	TIFR0 |= ( TRUE << ( OCF0A ) );

	return;
}

/**************************************************************************//**
 * \fn      ISR( TIMER0_COMP_vect )
 * \brief   This interruption routine is called when OCR0A matches with
 *          TCNT0. The interruption flag is cleared when the routine
 *          is called.
 * \note    TIMER0_COMP_vect  is supported by At90CAN128, tmr0 has only 1
 *          channel, when porting to another MCU please check if more
 *          channels are available, thus a different vector name should
 *          be referenced instead.
 * \return
 *****************************************************************************/
ISR( TIMER0_COMP_vect )
{

}

/**************************************************************************//**
 * \fn      ISR( TIMER0_OVF_vect )
 * \brief   This isr is called when TCNT0 passes from MAX to MIN.
 * \note
 * \return
 *****************************************************************************/
ISR( TIMER0_OVF_vect )
{

}

/**
 * @}
 */
/*---------------------------------------------------------------------------*/
