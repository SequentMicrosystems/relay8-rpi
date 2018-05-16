#ifndef RELAY8_H_
#define RELAY8_H_

#define RETRY_TIMES	10
#define RELAY8_INPORT_REG_ADD	0x00
#define RELAY8_OUTPORT_REG_ADD	0x01
#define RELAY8_POLINV_REG_ADD	0x02
#define RELAY8_CFG_REG_ADD		0x03

#define	COUNT_KEY	0
#define YES		1
#define NO		2

#define CHANNEL_NR_MIN		1
#define RELAY_CH_NR_MAX		8

#define ERROR	-1
#define FAIL	0
#define OK		1

#define RELAY8_HW_I2C_BASE_ADD	0x20
typedef uint8_t u8;
typedef uint16_t u16;

typedef enum
{
	OFF = 0,
	ON
} OutStateEnumType;

void startThread(void);
int checkThreadResult(void);
int readReg16(int dev, int add);
int readReg24(int dev, int add);
int writeReg16(int dev, int add, int val);
int writeReg24(int dev, int add, int val);
void printbits(int v);
int writeReg8(int dev, int add, int val);
int readReg8(int dev, int add);
int doBoardInit(int hwAdd);
int getLedVal(int chip);
int setLedVal(int chip, int val);
void busyWait(int ms);
int boardCheck(int hwAdd);
#endif //RELAY8_H_