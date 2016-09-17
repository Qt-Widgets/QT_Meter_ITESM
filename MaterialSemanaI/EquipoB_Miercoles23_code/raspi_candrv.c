#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "raspi_i2c_defs.h" 
#include "raspi_candrv.h"
 

// I2c frame sizes
#define I2C_CMD_MLREQUEST_FRAME_SZ        2
#define I2C_CMD_MLRESPONSE_FRAME_SZ        14

#define I2C_CMD_MLRESPONSE_MIN_FRAME_SZ   5
#define MAX_I2C_DATA_SZ                   50


typedef struct
{
    char  command;
    char  mailbox;
    char  status;
    // contains ID, dlc and data
    char  data_sz;
    char  data[MAX_I2C_DATA_SZ];
    char  chksum;

}i2c_msg;


raspi_can_ctx g_ctx = 
{
    //int canFd;
    0,
    //int i2c_slave_addr;
    I2C_CAN_SLAVE_ADDR,
    //int status;
    RASPI_CAN_CLOSED
};

//add AVR mailboxes to poll here
#define I2C_AVR_ENABLED_MBX 0x05 
char avr_mlbx_poll_list[ I2C_AVR_ENABLED_MBX ] =
{
    I2C_AVR_MAILBOX_1,
    I2C_AVR_MAILBOX_2,
    I2C_AVR_MAILBOX_3,
    I2C_AVR_MAILBOX_4,
    I2C_AVR_MAILBOX_5
};

static int raspi_i2c_to_can(  i2c_msg* i2c, can_msg* can );

static int raspi_get_mailbox(raspi_can_ctx* ctx, char mailNo, i2c_msg* response );

static int raspi_i2cmsg_to_i2cPdu( i2c_msg* i2c,  int size, char* buffer );
static int raspi_i2cPdu_toi2cmsg( int size , char* buffer, i2c_msg* i2c );

static int raspi_i2c_recv( raspi_can_ctx* ctx, int sz, char* buff );
static int raspi_i2c_snd( raspi_can_ctx* ctx, int sz, char* buff ); 

int raspi_can_init( raspi_can_ctx* ctx )
{
    int ret =RASPI_CAN_ERROR;
    int fd = 0;

#ifndef USE_FAKE_INIT

    if( ( fd = open( RASPI_I2C_DEV, O_RDWR ) ) != -1 )
    {
        // set slave address
        ret = ioctl( fd, I2C_SLAVE, I2C_CAN_SLAVE_ADDR );

        if( ret != 0 )
        {
            perror( " raspi can ioctl error " );
            exit( 1 );
        }
        else
        {
            ctx->canFd = fd;
            ctx->i2c_slave_addr = I2C_CAN_SLAVE_ADDR;
            ctx->status = RASPI_CAN_OPEN;
            ret = RASPI_CAN_OK;
        }

    }
    else
    {
	perror( "raspi can open error :");
        // error
        ctx->canFd = 0;
        ctx->status = RASPI_CAN_ERROR;
    }
#else
    ctx->canFd = fd;
    ctx->i2c_slave_addr = I2C_CAN_SLAVE_ADDR;
    ctx->status = RASPI_CAN_OPEN;
    ret = RASPI_CAN_OK;

#endif

    return ret;
}


// poll pre-configured mailboxes
int raspi_can_poll( raspi_can_ctx* ctx, int size, can_msg* msg )
{
    i2c_msg i2c = {0};
    can_msg can = {0};
    int idx = 0;
    int ret = RASPI_CAN_ERROR;
    
    for( idx = 0 ; ( ( idx < I2C_AVR_ENABLED_MBX ) && ( idx < size ) ) ; idx++ )
    {
        memset( &i2c, 0x00, sizeof(i2c_msg));
        memset( &can, 0x00, sizeof(can_msg) );
        if( ( raspi_get_mailbox( ctx, avr_mlbx_poll_list[ idx ], &i2c ) == I2C_OK ) && 
            ( raspi_i2c_to_can( &i2c, &can ) == I2C_OK ) )
        {
            // add received can msg
            memcpy( msg, &can, sizeof( can_msg ) );
			msg++;
            //msg[ idx ] = can;
            ret = idx + 1;
        }
        else
        {
            printf( "can poll error, mailbox no: %d \n",  avr_mlbx_poll_list[ idx ] );
            ret = RASPI_CAN_ERROR;
            //break;
        }

    }

    return ret;
}

// not supported
//int raspi_can_send( raspi_can_ctx* ctx, can_msg* msg )
//{
//}

void raspi_can_close( raspi_can_ctx* ctx )
{
    if( ctx->canFd != 0 )
    {
        close( ctx->canFd );

        ctx->canFd = 0;
        ctx->status = RASPI_CAN_CLOSED;
    }
}

static int raspi_i2c_to_can( i2c_msg* i2c, can_msg* can )
{
    int idx = 0;
    int ret = I2C_ERROR;

    if( ( i2c != 0x00 ) &&
            ( can != 0x00 ) )
    {
        memset( can, 0x0, sizeof(can_msg) );
        switch ( i2c->command )
        {
            case I2C_CMD_MAILBOX_RESPONSE:
                can->msgID =  ( i2c->data[ 1 ] ) | ( ( i2c->data[ 0 ] ) << 8 ); 

                can->DLC = i2c->data[ 2 ];
				//max payload = msgid (2) + dlc(1) + data + period(1)
                if( i2c->data_sz < CAN_DATA_MAX_SZ + 4 )
                {
                    // ignore last bytes: Period( 1 byte )
                    for( idx = 0 ; idx < ( i2c->data_sz - 1 ) ; idx++ )
                    {
                        // skip MSG ID (2 bytes) + DLC (1 byte)
                        can->data[ idx ] = i2c->data[idx + 3 ];
                    }
					// last byte period
					can->period = i2c->data[ idx ];
                    ret = I2C_OK;
                }
                break;

            default:
                break;
        }
    }
    return ret; 
}

    // translate i2c msg structure to i2c tx buffer 
    //According to documented frame structure
    //       ___________________________________________________________________________________________
    //       |  Byte 0   |  Byte 1  | Byte 2  |  Byte 3 |  Byte 4      -    Byte ( n-1 )  |  Byte n     |
    //       |   I2C CMD |  ***  CAN ID ***   |    DLC  |  *******   CAN DATA  *********  |  I2C CHKSUM |
    //       ----------------------------------------------------------------------------------------
    //                   |                                                               |
    //                   |    ***********I2C DATA FIELD (Optional) *******************   |
    //                   -----------------------------------------------------------------

static int raspi_i2cmsg_to_i2cPdu( i2c_msg* i2c,  int size, char* buffer   )
{
    int idx = 0;
    int cksum = 0;
    int ret = I2C_ERROR;

    switch ( i2c->command )
    {
        case I2C_CMD_MAILBOX_REQUEST:
            // only using CMD + CHKSUM fields (collapse others)
            if( size >= I2C_CMD_MLREQUEST_FRAME_SZ )
            {
                buffer[ 0 ] = ( ( I2C_CMD_MAILBOX_REQUEST << 4 ) | ( i2c->mailbox & 0x0f )  );
                //chksm
                buffer[ 1 ] = buffer[ 0 ];
                // frame size
                ret = I2C_CMD_MLBREQ_FRAME_SZ;
            }

            break; 

        case I2C_CMD_MAILBOX_RESPONSE:
            if ( size >= ( I2C_CMD_MLRESPONSE_MIN_FRAME_SZ + i2c->data_sz) )
            {
                buffer[idx] = ( ( I2C_CMD_MAILBOX_REQUEST << 4 ) | ( i2c->mailbox & 0x0f ) );
                idx++;

                for( ; idx < ( i2c->data_sz ) ; idx++ )
                {
                    buffer[ idx ] = i2c->data[ idx ];
                    cksum += buffer[ idx + 1 ];
                }

                buffer[idx] = cksum; 
                // frame size
                ret = ++idx;
            } 

            break;

        default:
            break;

    }


    return ret;
}

// translate i2c rx buffer to i2c message structure
// translate i2c msg structure to i2c tx buffer 
    //According to documented frame structure
    //       ___________________________________________________________________________________________
    //       |  Byte 0   |  Byte 1  | Byte 2  |  Byte 3 |  Byte 4      -    Byte ( n-1 )  |  Byte n     |
    //       |   I2C CMD |  ***  CAN ID ***   |    DLC  |  *******   CAN DATA  *********  |  I2C CHKSUM |
    //       ----------------------------------------------------------------------------------------
    //                   |                                                               |
    //                   |    ***********I2C DATA FIELD (Optional) *******************   |
    //                   -----------------------------------------------------------------
#define I2C_FRAME_DATA_IDX   1
#define I2C_FRAME_DLC_IDX    3
static int raspi_i2cPdu_toi2cmsg( int size , char* buffer, i2c_msg* i2c  )
{
    int ret = I2C_ERROR;
    unsigned char cksum = 0;
    int idx = 0;

    if( ( buffer != 0x00 ) &&
        ( i2c != 0x00 ) )
    {

        memset( i2c, 0x0, sizeof(i2c_msg) );
        // get command
        i2c->command = (buffer[ 0 ] & 0xf0) >> 4;
        i2c->mailbox =  buffer[ 0 ] & 0x0F;
        
        switch( i2c->command )
        {
                // only valid request for now
            case I2C_CMD_MAILBOX_RESPONSE:

                // fill Data & calc checksum
                cksum = buffer[ 0 ];
                // total data size = total size - cmd - checksum
                for( idx = 0 ; idx < ( size - 2 ) ; idx++ )
                {
					// skip cmd ( 1 byte )
                    i2c->data[ idx ] = buffer[ idx + 1 ];
                    cksum+=buffer[ idx + 1 ];
                }

                idx++;

                // check for valid checksum
                if( buffer[ idx ] == cksum  )
                {
                    i2c->chksum = cksum;
                    i2c->status = I2C_STATUS_VALID;
                    //data size = MSG ID(2 bytes) + DLC(data sz) +  DATA + PERIOD
                    i2c->data_sz = buffer[ I2C_FRAME_DLC_IDX ] + 4;
                    ret = I2C_OK;
                }
                else
                {
                    // checksum not valid
                    i2c->status = I2C_STATUS_ERROR;
                }

            break;

            // for now only mlbx response needed
            default:
             
            break;


        }        
    }
 return ret;
}



static int raspi_get_mailbox(raspi_can_ctx* ctx, char mailNo, i2c_msg* response )
{
    char buffer[ MAX_I2C_BUFF ];
    i2c_msg i2c_cmd;
    int i2c_sz = 0; 
    int i2c_ret =  I2C_ERROR;
    // build mailbox req
    i2c_cmd.command = I2C_CMD_MAILBOX_REQUEST; 
    i2c_cmd.mailbox = mailNo;
    i2c_sz = raspi_i2cmsg_to_i2cPdu( &i2c_cmd ,  sizeof(buffer), buffer );

    //send
    if ( raspi_i2c_snd( ctx, i2c_sz, buffer ) == I2C_OK )
    {
        usleep( WAIT_RESPONSE_US_DELAY ); 

	    // get only response bytes
        i2c_sz = raspi_i2c_recv( ctx, I2C_CMD_MLRESPONSE_FRAME_SZ, buffer);
        i2c_ret =  raspi_i2cPdu_toi2cmsg( i2c_sz, buffer, &i2c_cmd); 

        //receive
        if( ( i2c_sz > 0) &&
          ( i2c_ret == I2C_OK ) ) // &&
        //  ( i2c_cmd.command == I2C_CMD_MAILBOX_RESPONSE ) &&
        //  ( i2c_cmd.status == I2C_STATUS_VALID  )) 
        {
            // copy to output
            memcpy( response, &i2c_cmd, sizeof(i2c_cmd) ); 
            i2c_ret = I2C_OK;
        }
    }

    return i2c_ret;
}

// low level i2c receive
static int raspi_i2c_recv( raspi_can_ctx* ctx, int sz, char* buff )
{
    int ret = I2C_ERROR;

    if( ( ctx != 0x00 ) &&
        ( ctx->status ==  RASPI_CAN_OPEN ) &&
        ( buff != 0x00 ) )
    {
#ifndef USE_FAKE_READ
        if ( ( ret = read( ctx->canFd, buff, sz ) ) == -1 )
        {
           perror( " raspi read error : ");
        }
#else

        ret = fake_read( ctx->canFd, buff, sz ); 
#endif
    }

    return ret;
}

// low  level i2c send
static int raspi_i2c_snd( raspi_can_ctx* ctx, int sz, char* buff )
{
    int ret = I2C_ERROR;
    int sendBytes = 0;

    if( ( ctx != 0x00 ) &&
            ( ctx->status ==  RASPI_CAN_OPEN ) &&
            ( buff != 0x00 ) )
    {   

#ifndef USE_FAKE_WRITE
        //transmit

        if( ( ( sendBytes = write( ctx->canFd, buff, sz ) ) > 0 ) &&
            ( sendBytes == sz ) )
        {
            ret = I2C_OK;
        }
        else if ( sendBytes == -1 )
        {
            perror( " raspi i2c error : ");
        }	
#else

        if( ( ( sendBytes = fake_write( ctx->canFd, buff, sz ) ) > 0 ) &&
            ( sendBytes == sz ) )
        {
            ret = I2C_OK;
        }

#endif 

    }
    return ret;
}

