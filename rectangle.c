#include "rectangle.h"

Req *request(const char *dstip, const int dstport) {
    Req *req;

    req = malloc(reqsize);
    req->vn = 4;
    req->cd = 1;
    req->dstport = htons(dstport);  // Switch to network byte order
    req->dstip = inet_addr(dstip);
    strncpy(req->userid, USERNAME, 7);

    return req;
};

int main(int argc, char *argv[]) {
    char *host;
    int port;
    int s;  // Socket file descriptor return value
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buf[ressize];
    int success;
    char temp[512];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        return -1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        return -1;
    }

    // Preparing the socket structure for TCP
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    if (connect(s, (struct sockaddr *)&sock, sizeof(sock))) {
        perror("connect");
        return -1;
    }

    printf("Connected to proxy server\n");

    // Handle connection
    req = request(host, port);
    write(s, req, reqsize);

    memset(buf, 0, ressize);
    if (read(s, buf, ressize) < 1) {
        perror("read");
        free(req);
        close(s);
        return -1;
    }

    res = (Res *)buf;
    success = (res->cd == 90);
    if (!success) {
        fprintf(stderr,
                "Unable to traverse the proxy"
                "the proxy, error code: %d\n",
                res->cd);
        close(s);
        free(req);
        return -1;
    }

    printf("Successfully connected through the proxy to %s:%d\n", host, port);

    memset(temp, 0, 512);
    // Get the response and write it to temp and later to s
    snprintf(temp, 511,
             "HEAD / HTTP/1.0\r\n"
             "Host: www.networktechnology.org\r\n"
             "\r\n");
    write(s, temp, strlen(temp));
    memset(temp, 0, 512);
    read(s, temp, 511);
    printf("'%s'\n", temp);

    close(s);
    free(req);

    return 0;
}
