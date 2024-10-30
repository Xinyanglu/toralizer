#include "toralizer.h"

ProxyRequest* create_request(const char* dst_ip, const int dst_port) {
    ProxyRequest* request = (ProxyRequest*)malloc(sizeof(ProxyRequest));
    request->vn = 4;
    request->cd = 1;
    request->dst_port = htons(dst_port);
    request->dst_ip = inet_addr(dst_ip);
    strncpy(request->user_id, USERNAME, 8);

    return request;
}


int main(int argc, char *argv[]) {
    char* host;
    int port;
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <host> <port> \n", argv[0]);
        return -1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    int proxy_sockfd;
    struct sockaddr_in server_addr;

    // Create socket to connect to proxy server
    if ((proxy_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Proxy socket creation error\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PROXY_PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, PROXY, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid proxy address/ Proxy address not supported\n");
        return -1;
    }

    printf("Socket created for %s:%d\n", PROXY, PROXY_PORT);

    // Connect to the proxy server
    if (connect(proxy_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Connection Failed\n");
        return -1;
    }

    printf("Connected to proxy %s:%d\n", PROXY, PROXY_PORT);

    ProxyRequest* request = create_request(host, port);


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
    } 
    
    printf("Connected through proxy %s:%d\n", host, port);

    char tmp[512] = "HEAD / HTTP/1.0\r\n\r\n";

    write(proxy_sockfd, tmp, 512);

    read(proxy_sockfd, tmp, 512);
    printf("'%s'\n", tmp);

    close(proxy_sockfd);
    free(request);
    
    return 0;
}