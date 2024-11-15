#define xdc__strict

#include <xdc/runtime/System.h>
#include <Headers/F2837xD_device.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Messages.h"


void scib_xmit(int a)
{
    while (ScibRegs.SCICTL2.bit.TXRDY == 0) {};
    ScibRegs.SCITXBUF.all = a;
}

void scib_msg(char *msg)
{
    int i;
    int length = strlen(msg);  // Calculate length once
    for (i = 0; i < length; i++)
    {
        scib_xmit(msg[i]);
    }
}

void scic_xmit(int a)
{
    while (ScicRegs.SCICTL2.bit.TXRDY == 0) {};
    ScicRegs.SCITXBUF.all = a;
}

void scic_msg(char *msg)
{
    int i;
    int length = strlen(msg);
    for (i = 0; i < length; i++)
    {
        scic_xmit(msg[i]);
    }
}
// First value is for grabber(0 for off 1 for on, any other for moving arm), second for x, then z
void sendCMD(int xVal, bool dir)
{
    if (xVal >= 110 && xVal <= 230) {
        int index = xVal - 110;
        const char *msg;
        if (dir) {
            msg = messagesup[index];
        } else {
            msg = messagesdown[index];
        }
        scic_msg((char *)msg);  // Now msg is accessible here
    }
}
void suck(bool answer)
{
    if (answer)
    {
       scic_msg("M2231 V1\n");
    }
    else
    {
        scic_msg("M2231 V0\n");
    }
}
