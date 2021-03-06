#ifndef __rs485_h__
#define __rs485_h__

#define RS485_PORT "/dev/ttyAMA0"

#define RS485_THREAD_IDLE_DELAY 1000
#define MAX_RS485_BUFFER        1000

enum {
    HARDWARE_RTS = 0,
    SOFTWARE_RTS,
    SOFTWARE_GPIO
};


void init_rs485(int SoftRTS, int InvertRTS, int PinNum);
void finish_rs485();
int rs485_write(char *buf, int len);

#endif
