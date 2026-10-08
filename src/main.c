#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <unistd.h>

int main() {
    // sockaddr_in (in for info) is a more robust sockaddr struct
    struct sockaddr_in clientInfo = {0};
    int clientSize = 0;

    struct sockaddr_in serverInfo = {0};
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = 0;
    serverInfo.sin_port = htons(5555);

    // socket is just a file descriptor
    // Explanation taken from: https://www.reddit.com/r/c_language/comments/p8n4rg/c_languagewhy_do_different_sockets_use_the_same/
    // - File descript of a socket is not a file, it's a network socket, but both use the same "way" to ask the OS for resources of input and output.
    // - Some OSs call them handles, which seems to be a better name.
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        return -1;
    }

    printf("%d\n", fd);

    // Bind the socket file descriptor to the defined address and port, before this is just an fd
    if(bind(fd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
        perror("bind");
        close(fd);
        return -1;
    }

    // Marks the socket internally as a passive socket, which is a socket that will be able to receive connections using accept.
    // Explanation taken from: https://stackoverflow.com/questions/60181148/what-is-the-purpose-of-listen-in-socket-programming
    // - The listen function tells the TCP/IP stack to accept TCP connections on the port defined by the socket.
    // - So, the TCP handshake is done before accept.
    if(listen(fd, 0) == -1) {
        perror("listen");
        close(fd);
        return -1;
    }

    int cfd = accept(fd, (struct sockaddr *)&clientInfo, &clientSize);
    if(cfd == -1) {
        perror("accept");
        close(fd);
        return -1;
    }

    close(cfd);
}
