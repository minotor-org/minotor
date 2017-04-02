/*
dmxmain.h forkt from https://github.com/ohm2013loc/art/blob/master/dmxmain.c
*/

#ifndef dmxmain_H
#define dmxmain_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

extern struct addrinfo *dmx_dest;

void setup_socket(char *universe_address);
void send_dmx(struct addrinfo *dest, char *data, int length);
void delay_setup(void);
void frame_delay(unsigned int microseconds);


#endif // dmxmain_H

