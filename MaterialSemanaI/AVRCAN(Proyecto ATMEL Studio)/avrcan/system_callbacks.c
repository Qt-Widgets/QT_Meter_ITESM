/*---------------------------------------------------------------------------*/
/**************************************************************************//**
 * \file       system_callbacks.c
 * \addtogroup callbacks
 * @{
 * \since      13/08/2015
 * \version    0.1
 * \author     Roberto Alejandro Flores Estrada
 *****************************************************************************/


#include <system_callbacks.h>
#include <string.h>

static uint8_t dataBuffer[ DATA_LEN ];
static uint8_t targetMailbox = NUM_MAILBOXES;

typedef enum
{
    MSG_CMD = 0, /*< */
    MSG_IDH,     /*< */
    MSG_IDL,     /*< */
    MSG_DLC,     /*< */
    MSG_DT7,     /*< */
    MSG_DT6,     /*< */
    MSG_DT5,     /*< */
    MSG_DT4,     /*< */
    MSG_DT3,     /*< */
    MSG_DT2,     /*< */
    MSG_DT1,     /*< */
    MSG_DT0,     /*< */
    MSG_PRD,     /*< */
    MSG_CHK,     /*< */
    MSG_TOTAL_LENGTH
} messageFields;

uint8_t cbCalculateChecksum( uint8_t* data, uint32_t len );
uint8_t cbValidateChecksum( uint8_t *data, uint8_t chksm, uint32_t len );
uint32_t cbBuildDataFrame(uint8_t* data, CanMw_Message *msg,
                          CanMW_MBox *mailbox );
/*---------------------------------------------------------------------------*/


/**************************************************************************//**
 * \fn          void cbSetRXBuffer( uint8_t* data, uint32_t *len)
 * \param[out]  data   Establishes the data buffer that will be used in either
 *                     UART or TWI interface.
 * \param[out]  len    Data buffer length.
 * \brief
 * \note        This function is called at the beginning of the reception.
 * \return
 *****************************************************************************/
void cbSetRXBuffer( uint8_t** data, uint32_t *len)
{
    memset( dataBuffer, 0, sizeof( dataBuffer ) );
    *len = sizeof( dataBuffer );
    *data = dataBuffer;
    return;
}

/**************************************************************************//**
 * \fn          void cbSetTXBuffer( uint8_t* data, uint32_t *len)
 * \param[out]  data   Establishes the data buffer that will be used in either
 *                     UART or TWI interface.
 * \param[out]  len    Data buffer length.
 * \brief
 * \note        This function is called at the beginning of the transmission.
 * \return
 *****************************************************************************/
void cbSetTXBuffer( uint8_t** data, uint32_t *len)
{
    memset( dataBuffer, 0, sizeof( dataBuffer ) );
    *len = sizeof( dataBuffer );
    *data = dataBuffer;
    return;
}

/**************************************************************************//**
 * \fn          void cbDecodeReceivedData( uint8_t *data, uint32_t len )
 * \param[in]   data   Data to be processed.
 * \param[in]   len    Data length.
 * \brief
 * \note        This function is called at the end of the reception.
 *              Checksum is assumed to be the last byte received.
 * \return
 *****************************************************************************/
void cbDecodeReceivedData( uint8_t *data, uint32_t len )
{
    uint8_t chksmCorrect = FALSE;
    uint8_t cmd = 0;
    uint8_t idx = 0;
    uint32_t chkidx = 0;
    // Data pointer must not be NULL and len must be higher than zero.
    if ( ( data ) && ( len ) )
    {
        chkidx = len - 1;
        chksmCorrect = cbValidateChecksum( data, data[ chkidx ], chkidx );
        if ( chksmCorrect )
        {
            cmd = GET_HIGH_NIBBLE( data[ idx ] );
        }
        if ( cmd == CAN_READ )
        {
            targetMailbox = GET_LOW_NIBBLE( data[ idx ] );
        }
        else if ( cmd == CAN_WRITE )
        {
            cbCommitData( data, len);
        }
        else
        {
            //No operation
        }
    }
    return;
}

/**************************************************************************//**
 * \fn          uint8_t cbFetchData( uint8_t* data )
 * \param[out]  data array of data bytes
 * \brief       This function is called at every read request from a master to
 *              this device.
 * \return      uint32_t returns data length in bytes
 *****************************************************************************/
uint32_t cbFetchData( uint8_t* data )
{
    uint32_t ret        = 0;
    CanMw_Message msg;
    CanMW_MBox mailbox;
    uint32_t len = NUM_CONTROL_BYTES;
    if ( targetMailbox < NUM_MAILBOXES )
    {
        Get_Message_FromMbox( targetMailbox, &msg );
        Get_MboxConfig( targetMailbox, &mailbox );
        len += msg.dlc;
        if ( len  < DATA_LEN )
        {
            ret = cbBuildDataFrame( data, &msg, &mailbox );
        }
    }
    return ret;
}

/**************************************************************************//**
 * \fn          uint32_t cbBuildDataFrame(uint8_t* data, CanMw_Message *msg,
                          CanMW_MBox *mailbox )
 * \brief
 * \return
 *****************************************************************************/
uint32_t cbBuildDataFrame(uint8_t* data, CanMw_Message *msg,
                          CanMW_MBox *mailbox )
{
    messageFields ret     = 0;
    uint8_t checksum = 0;

    if ( data && msg && mailbox )
    {
        data[ MSG_CMD ] = CAT_NIBBLES( CAN_WRITE, targetMailbox );
        data[ MSG_IDH ] = GET_HIGH_BYTE( msg->message_id );
        data[ MSG_IDL ] = GET_LOW_BYTE( msg->message_id );
        data[ MSG_DLC ] = msg->dlc;
        memcpy( ( data + MSG_DT7 ), msg->data, ( sizeof( msg->data ) / sizeof( can_message_data ) ) );
        data[ MSG_PRD ] = GET_LOW_BYTE( msg->ttc_timmer );
        checksum = cbCalculateChecksum( data, MSG_CHK );
        data[ MSG_CHK ] = checksum;
        ret = MSG_TOTAL_LENGTH;
        targetMailbox = NUM_MAILBOXES;
    }

    return ret;
}


/**************************************************************************//**
 * \fn         void cbCommitData( uint8_t* data, uint32_t len )
 * \param[in]
 * \brief
 * \return
 *****************************************************************************/
void cbCommitData( uint8_t* data, uint32_t len )
{
    CanMW_MBox mbcfg;
    uint8_t Mailbox  = GET_LOW_NIBBLE( data[ MSG_CMD ] );
    uint32_t chkidx  = 0;

    if ( ( Mailbox < NUM_MAILBOXES ) && len )
    {
        Get_MboxConfig(Mailbox, &mbcfg);
        chkidx = len - 1;
        if ( cbValidateChecksum( data, data[ chkidx ], chkidx ) )
        {
            mbcfg.msg.message_id = CAT_BYTES( data [ MSG_IDH ], data [ MSG_IDL ] );
            mbcfg.msg.dlc = data[ MSG_DLC ];
            mbcfg.msg.ttc_timmer = data[ MSG_PRD ];
            memcpy( &mbcfg.msg.data, &(data[ MSG_DT7 ]), ( MSG_DT0 - MSG_DLC ) );

        }
        Write_Message_Mbox( Mailbox, &mbcfg );
    }

    return;
}

/**************************************************************************//**
 * \fn         uint8_t cbStartOfFrame( uint8_t data )
 * \param[in]
 * \brief
 * \return
 *****************************************************************************/
uint8_t cbStartOfFrame( volatile uint8_t data )
{
    uint8_t ret = FALSE;
    uint8_t cmd = GET_HIGH_NIBBLE( data );
    if ( ( cmd == CAN_READ )
        || ( cmd == CAN_WRITE ) )
    {
        ret = TRUE;
    }
    return ret;
}

/**************************************************************************//**
 * \fn         void cbSetupReply( uint8_t* in, uint8_t* out, uint32_t *len )
 * \param[in]
 * \brief
 * \return
 *****************************************************************************/
void cbSetupReply( uint8_t* in, uint8_t* out, uint32_t *len )
{
    *len = 0;
    uint8_t mailbox = 0;
    uint8_t checksum = 0;
    if ( in && out )
    {
        mailbox = GET_LOW_NIBBLE( *in );
        out[ ( *len )++ ] = CAT_NIBBLES( CAN_WRITE_REPLY, mailbox );
        out[ ( *len )++ ] = TRUE;
        checksum = cbCalculateChecksum( out, *len );
        out[ ( *len )++ ] = checksum;
    }

}

/**************************************************************************//**
 * \fn          uint8_t cbValidateChecksum( uint8_t *data, uint8_t chksm,
 *                                       uint32_t len )
 * \param[in]   data   Data to be validated.
 * \param[in]   chksm  Received checksum.
 * \param[in]   len    Data length.
 * \brief
 * \note
 * \return
 *****************************************************************************/
uint8_t cbValidateChecksum( uint8_t *data, uint8_t chksm, uint32_t len )
{
    uint32_t i  = 0;
    uint8_t sum = 0;
    uint8_t ret = FALSE;
    if ( data )
    {
        for ( ; i < len; i++)
        {
            sum += data[ i ];
        }
        ret = ( uint8_t ) ( sum == chksm );
    }
    return ret;
}

/**************************************************************************//**
 * \fn         uint8_t cbCalculateChecksum( uint8_t* data, uint32_t len )
 * \param[in]
 * \brief
 * \return
 *****************************************************************************/
uint8_t cbCalculateChecksum( uint8_t* data, uint32_t len )
{
    uint8_t ret = 0;
    uint32_t i  = 0;
    if ( data )
    {
        for ( ; i < len; i++ )
        {
            ret += data[i];
        }
    }
    return ret;
}
