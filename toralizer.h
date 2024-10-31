#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <dlfcn.h>

#define PROXY "127.0.0.1"
#define PROXY_PORT 9050
#define USERNAME "toruser"

typedef unsigned char int8;
typedef unsigned short int16;
typedef unsigned int int32;

typedef struct {
    int8 vn;
    int8 cd;
    int16 dst_port;
    int32 dst_ip;
    int8 user_id[8];
}ProxyRequest;


typedef struct{
    int8 vn;
    int8 cd;
    int16 dst_port; //ignored
    int32 dst_ip; // ignored
}ProxyResponse;

ProxyRequest* create_request(struct sockaddr_in*);
int connect(int, const struct sockaddr*,
            socklen_t);