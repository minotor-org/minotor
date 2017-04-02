/* dmxmain.c forkt from https://github.com/ohm2013loc/art/blob/master/dmxmain.c */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <artdmx.h>

#define OpOutput 0x5000

#define VERSION_HI 0
#define VERSION_LO 14

#define ARTNET_PORT "6454"


struct pkt_ArtOutput {
    char ID[8];
    unsigned short OpCode;
    unsigned char ProtVerHi;
    unsigned char ProtVerLo;
    unsigned char Sequence;
    unsigned char Physical;
    unsigned char SubUni;
    unsigned char Net;
    unsigned char LengthHi;
    unsigned char Length;
    unsigned char Data[512];
} __attribute__((packed));

#define DMXPKTLEN(channels) (channels+18)

int art_socket;

struct addrinfo *dmx_dest;

void setup_socket(char *universe_address)
{
    struct addrinfo hints, *res;
    struct sockaddr_in myaddr;
    struct timeval timeout;
    int rv;

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    int yes = 1;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    getaddrinfo("255.255.255.255", ARTNET_PORT, &hints, &res);

    art_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (art_socket == -1) {
        perror("socket");
        exit(1);
    }

#if 0
    if (bind(art_socket, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        exit(1);
    }
#endif

    if (setsockopt(art_socket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))
        == -1) {
        perror("setsockopt");
        exit(1);
    }

    if (setsockopt(art_socket,SOL_SOCKET,SO_BROADCAST,&yes,sizeof(int))
        == -1) {
        perror("setsockopt");
        exit(1);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo(universe_address, ARTNET_PORT, &hints, &dmx_dest))
            != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

}

void send_dmx(struct addrinfo *dest, char *data, int length)
{
    struct pkt_ArtOutput pkt;
    int i;
    int numbytes;
    static unsigned char dmx_sequence;

    strcpy(pkt.ID, "Art-Net");
    pkt.OpCode = OpOutput;
    pkt.ProtVerHi = VERSION_HI;
    pkt.ProtVerLo = VERSION_LO;
    pkt.Sequence = dmx_sequence++;
    if (dmx_sequence == 0)
        dmx_sequence++;
    pkt.Physical = 0;
    pkt.SubUni = 0;
    pkt.Net = 0;
    pkt.LengthHi = (length >> 8);
    pkt.Length = length & 0xff;
    memcpy(pkt.Data, data, length);

    if ((numbytes = sendto(art_socket, &pkt, DMXPKTLEN(length), 0,
             dest->ai_addr, dest->ai_addrlen)) == -1) {
        perror("sendto");
        exit(1);
    }
}

struct timeval delay_time;

void delay_setup(void)
{
    gettimeofday(&delay_time, NULL);
}

void frame_delay(unsigned int microseconds)
{
    struct timeval interval;
    struct timeval now;
    struct timespec int_ts;

    interval.tv_sec = microseconds / 1000000;
    interval.tv_usec = microseconds % 1000000;

    timeradd(&delay_time, &interval, &delay_time);

    gettimeofday(&now, NULL);

    do {
        timersub(&delay_time, &now, &interval);

        int_ts.tv_sec = interval.tv_sec;
        int_ts.tv_nsec = interval.tv_usec * 1000;

        nanosleep(&int_ts, NULL);

        gettimeofday(&now, NULL);
    } while (timercmp(&delay_time, &now, >));
}
