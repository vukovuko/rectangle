#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PROXY "127.0.0.1"  // Localhost
#define PROXYPORT 9950     // Default port

// See https://www.openssh.com/txt/socks4.protocol

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

struct proxy_request {
    int8 vn;
    int8 cd;
    int16 dstport;
    int32 dstip;
    unsigned char userid[8];
};
typedef struct proxy_request Req;

struct proxy_response {
    int8 vn;
    int8 cd;
    int16 _;
    int32 __;
};
typedef struct proxy_response Res;
