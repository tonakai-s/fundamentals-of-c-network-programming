#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <ip of host>\n", argv[0]);
        return 0;
    }
    // sockaddr_in (in for info) is a more robust sockaddr struct
    struct sockaddr_in serverInfo = {0};
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_addr.s_addr = inet_addr(argv[1]);
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

    if(connect(fd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)) == -1) {
        perror("connect");
        close(fd);
        return 0;
    }

    close(fd);
}
