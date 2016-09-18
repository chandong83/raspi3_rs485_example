#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rs485.h"

int main(int argc, char *argv[])
{
    char buf[10];
    int i=0;

    if(argc < 3)
    {
        printf("rs485_exam [SWRTS] [INVERT]\n");
        printf("[SWRTS]  0 : Hardware RTS, 1 : Software RTS\n");
        printf("[INVERT] 0 : Not Invert, 1 : Invert\n");
        printf("\t INVERT Option is SoftwareRTS Mode Only\n");
        printf("\t when is Hardware RTS mode, you should set to 0\n");
        printf("EX>\n");
        printf("\trs485_exam 0 0 : Hardware RTS, Nothing\n");
        printf("\trs485_exam 1 0 : Software RTS, Not Invert\n");
        printf("\trs485_exam 1 1 : Software RTS, Invert\n");
        return -1;
    }
    int sw = atoi(argv[1]);
    int invert = atoi(argv[2]);

    printf("%d %d\n",sw, invert);
    init_rs485(sw, invert);

    //create the dummy data
    for(i=0;i<10;i++)
    {
        buf[i] = i;
    }

    rs485_write(buf, 10);
    usleep(50000);
    rs485_write(buf, 10);

    finish_rs485();

    return 1;
}
