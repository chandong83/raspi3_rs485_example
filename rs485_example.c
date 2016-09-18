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
        printf("[SWRTS]  0 : Hardware RTS, 1 : Software RTS, 2 : Software GPIO\n");
        printf("[OPTION] When SWRTS is 1, 0 : Not Invert, 1 : Invert\n");
        printf("[OPTION] When SWRTS is 2, PIN NUMBER for DE/RE\n");
        printf("\t INVERT Option is SoftwareRTS Mode Only\n");
        printf("\t when is Hardware RTS mode, you should set to 0\n");
        printf("EX>\n");
        printf("\trs485_exam 0 0 : Hardware RTS, Nothing\n");
        printf("\trs485_exam 1 0 : Software RTS, Not Invert\n");
        printf("\trs485_exam 1 1 : Software RTS, Invert\n");
        printf("\trs485_exam 2 17: Software GPIO, Pin 17\n");
        return -1;
    }
    int sw = atoi(argv[1]);
    int option = atoi(argv[2]);

    printf("%d %d\n",sw, option);
    switch(sw)
    {
        case 0:
            init_rs485(sw, 0, 0);
            break;
        case 1:
            init_rs485(sw, option, 0);
        break;
        case 2:
            init_rs485(sw, 0, option);
            break;
    }

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
