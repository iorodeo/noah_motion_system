#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#if defined(OS_LINUX) || defined(OS_MACOSX)
#include <sys/ioctl.h>
#include <termios.h>
#elif defined(OS_WINDOWS)
#include <conio.h>
#endif

#include "hid.h"


const int timeout = 12;


int main()
{
    int i, r, num;
    char buf[64];


    // C-based example is 16C0:0480:FFAB:0200
    r = rawhid_open(1, 0x16C0, 0x0480, 0xFFAB, 0x0200);
    if (r <= 0) {
        // Arduino-based example is 16C0:0486:FFAB:0200
        r = rawhid_open(1, 0x16C0, 0x0486, 0xFFAB, 0x0200);
        if (r <= 0) {
            printf("no rawhid device found\n");
            return -1;
        }
    }
    printf("found rawhid device\n");

    while (1) {
        // check if any Raw HID packet has arrived
        num = rawhid_recv(0, buf, 64, timeout);
        if (num < 0) {
            printf("\nerror reading, device went offline\n");
            rawhid_close(0);
            return 0;
        }
        if (num > 0) {

            //printf("\nrecv %d bytes:\n", num);
            //for (i=0; i<num; i++) {
            //    printf("%02X ", buf[i] & 255);
            //    if (i % 16 == 15 && i < num-1) printf("\n");
            //}
            //printf("\n");

            unsigned long val = 0;
            for (i=0; i<4; i++)
            {
                val |= ((buf[i]&255) << 8*i);
            }
            printf("%lu\n", val);

            for (i=0;i<64;i++)
            {
                buf[i] = i;
            }
            rawhid_send(0, buf, 64, timeout);
        }
        else
        {
            printf("no data\n");
        }

    }
}




