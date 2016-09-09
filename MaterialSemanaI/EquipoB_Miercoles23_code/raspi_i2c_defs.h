
// raspi device name as loaded by i2c-dev module
#define I2C_DEV_NAME  "/dev/i2c-1"
#define I2C_CAN_SLAVE_ADDR 0x1C 


#define MAX_I2C_BUFF 50 
//microsec delay between request / response
#define WAIT_RESPONSE_US_DELAY  0


// i2c interface return codes
#define I2C_OK         0
#define I2C_BUSY       1 
#define I2C_ERROR     -1

#define RASPI_I2C_DEV  "/dev/i2c-1"

// I2C frames type

#define I2C_CMD_MAILBOX_REQUEST     0xA
#define I2C_CMD_MAILBOX_RESPONSE    0xB

#define I2C_CMD_MLBREQ_FRAME_SZ     2

// I2C frame status
#define I2C_STATUS_TIMEOUT     0xFE
#define I2C_STATUS_VALID       0xAA
#define I2C_STATUS_ERROR       0xFF

// AVR can 15 mailboxes max
// AVR mailboz addr
#define I2C_AVR_MAILBOX_0    0x0
#define I2C_AVR_MAILBOX_1    0x1 
#define I2C_AVR_MAILBOX_2    0x2
#define I2C_AVR_MAILBOX_3    0x3
#define I2C_AVR_MAILBOX_4    0x4
#define I2C_AVR_MAILBOX_5    0x5
#define I2C_AVR_MAILBOX_6    0x6
#define I2C_AVR_MAILBOX_7    0x7
#define I2C_AVR_MAILBOX_8    0x8
#define I2C_AVR_MAILBOX_9    0x9
#define I2C_AVR_MAILBOX_10   0xa
#define I2C_AVR_MAILBOX_11   0xb
#define I2C_AVR_MAILBOX_12   0xc
#define I2C_AVR_MAILBOX_13   0xd
#define I2C_AVR_MAILBOX_14   0xe


