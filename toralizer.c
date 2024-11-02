#include "toralizer.h"

ProxyRequest* create_request(struct sockaddr_in* server_address) {
    ProxyRequest* request = (ProxyRequest*)malloc(sizeof(ProxyRequest));
    request->vn = 4;
    request->cd = 1;
    request->dst_port = server_address->sin_port;
    request->dst_ip = server_address->sin_addr.s_addr;
    strncpy(request->user_id, USERNAME, 8);

    return request;
}


int connect(int original_sockfd, const struct sockaddr *original_address,
            socklen_t addrlen){
    char host[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &((struct sockaddr_in*)original_address)->sin_addr, host, INET_ADDRSTRLEN);
    int port = ntohs(((struct sockaddr_in*)original_address)->sin_port);

    int proxy_sockfd;
    struct sockaddr_in proxy_server_address;
    int (*original_connect)(int, const struct sockaddr*, socklen_t);

    original_connect = dlsym(RTLD_NEXT, "connect");

    // Create socket to connect to proxy server
    if ((proxy_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Proxy socket creation error\n");
        return -1;
    }

    proxy_server_address.sin_family = AF_INET;
    proxy_server_address.sin_port = htons(PROXY_PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, PROXY, &proxy_server_address.sin_addr) <= 0) {
        fprintf(stderr, "Invalid proxy address/ Proxy address not supported\n");
        return -1;
    }

    printf("Socket created for %s:%d\n", PROXY, PROXY_PORT);

    // Connect to the proxy server
    if (original_connect(proxy_sockfd, (struct sockaddr *)&proxy_server_address, sizeof(proxy_server_address)) < 0) {
        fprintf(stderr, "Connection Failed\n");
        return -1;
    }

    printf("Connected to proxy %s:%d\n", PROXY, PROXY_PORT);

    ProxyRequest* request = create_request((struct sockaddr_in*)original_address);

    write(proxy_sockfd, request, sizeof(ProxyRequest));

    ProxyResponse response;
    memset(&response, 0 , sizeof(ProxyResponse));
    if (read(proxy_sockfd, &response, sizeof(ProxyResponse)) < 0) {
        fprintf(stderr, "Read failed\n");
        free(request);
        close(proxy_sockfd);
        return -1;
    }

    char success = (response.cd == 90);
    if (!success) {
        fprintf(stderr, "Unable to connect through proxy. Error code: %d\n", response.cd);
        free(request);
        close(proxy_sockfd);
        return -1;
    } 
    
    printf("Connected through proxy to %s:%d\n", host, port);
    dup2(proxy_sockfd, original_sockfd);

    free(request);
    
    return 0;
}