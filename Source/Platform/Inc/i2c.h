#ifndef _IS_INCLUDED_I2C_H
#define _IS_INCLUDED_I2C_H
#include "type.h"

/*--- Return codes ---*/
#define OK            1
#define DATA          2
#define RTR           3

#define ERROR        -1
#define FULL         -2
#define EMPTY        -3
#define BUSY         -4


#define I2C_ACK0     0   
#define I2C_ACK1     1
#define I2C_READ     2



void i2c_init(U8 i2c_num);
 void i2c_reset(void); 
 S8 i2c_start(U8 i2c_num);
 S8 i2c_repeat_start(U8 i2c_num);
 S8 i2c_stop(U8 i2c_num);
 S8 i2c_master_putchar(U8 i2c_num, U8 Data );
 S8 i2c_master_getchar(U8 i2c_num, U8 Mode, U8 *pData );
 S8 i2c_master_write(U8 i2c_num, U8 *pTxData, U16 Length );
 S8 i2c_master_read(U8 i2c_num, U8 *pRxData, U16 Length);
 U8 i2c_check_status(U8 i2c_num);

#define I2C_PINSEL0 0x30C00000
#define I2C_SLAVEADR_SEND 0xf0
#define I2C_CLEAR_ALL_FLAGS 0x6C

enum I2C_NUM_E
{
  I2C_0,
  I2C_1
};

#endif 


