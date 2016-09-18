#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <termio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include "rs485.h"

int rs485_fd = -1;
int rs485_finish = 0;
struct termios oldtty;
int rs485flag = TIOCM_RTS;
int bSoftRTS=0;
int bInvertRTS=0;

int rs485_open(int SoftRTS, int InvertRTS)
{

    char *portname = RS485_PORT;
    bSoftRTS = SoftRTS;
    bInvertRTS = InvertRTS;

    rs485_fd = open (portname, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (rs485_fd < 0)
    {
        printf("failed to open port %s\n", RS485_PORT);
        return -1;
    }
    tcgetattr(rs485_fd,&oldtty);

    struct termios tty;

    bzero(&tty, sizeof(tty));

    cfsetospeed (&tty, B9600);
    cfsetispeed (&tty, B9600);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8 | CLOCAL | CREAD;     // 8-bit chars
    if(bSoftRTS)
        tty.c_cflag |= HUPCL;
    else
        tty.c_cflag |= CRTSCTS | HUPCL;


    tty.c_iflag = IGNPAR;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 0;            // 0.5 seconds read timeout

    if (tcsetattr (rs485_fd, TCSANOW, &tty) != 0)
    {
      return -1;
    }


    tcflush(rs485_fd,TCIOFLUSH);

    if(bSoftRTS)
    {
        if(bInvertRTS)
            ioctl(rs485_fd, TIOCMBIS, &rs485flag);
        else
            ioctl(rs485_fd, TIOCMBIC, &rs485flag);
    }


    return 1;
}

void rs485_close()
{
    tcsetattr (rs485_fd, TCSANOW, &oldtty);
    close(rs485_fd);
}

int rs485_write(char *buf, int len)
{
    int nRet;
    int txempty;
    if(rs485_fd == -1){
        printf("%s port is not opened\n", RS485_PORT);
        return -1;
    }
    if(bSoftRTS)
    {
        if(bInvertRTS)
            ioctl(rs485_fd, TIOCMBIC, &rs485flag);
        else
            ioctl(rs485_fd, TIOCMBIS, &rs485flag);
    }
    nRet = write(rs485_fd, buf, len);

    if(bSoftRTS)
    {
        while(1)
        {
            ioctl(rs485_fd, TIOCSERGETLSR, &txempty);
            if(txempty) break;
        }

        if(bInvertRTS)
            ioctl(rs485_fd, TIOCMBIS, &rs485flag);
        else
            ioctl(rs485_fd, TIOCMBIC, &rs485flag);
    }

    return nRet;
}

void rs485_receive()
{
    int nRet;
    int nRead;
    int i;
    char cBuf[MAX_RS485_BUFFER];

    while(!rs485_finish) {
        nRead = 0;
        nRet = ioctl(rs485_fd, FIONREAD, &nRead);
        if(!nRead)
        {
            usleep(RS485_THREAD_IDLE_DELAY);
            continue;
        }

        nRet = read(rs485_fd, cBuf, nRead);

        for(i=0;i<nRet;i++)
        {
            printf("%c",cBuf[i]);
        }
        usleep(RS485_THREAD_IDLE_DELAY);
    }
    rs485_close();
}

void init_rs485(int SoftRTS, int InvertRTS)
{
  pthread_t tRS485Id;
  rs485_finish = 0;
  if(rs485_open(SoftRTS, InvertRTS) < 0)
  {
      return ;
  }
  pthread_create(&tRS485Id, NULL,(void *)&rs485_receive, NULL);
}


void finish_rs485()
{
  rs485_finish=1;
}
