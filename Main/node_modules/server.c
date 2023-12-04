#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to server address
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    printf("UDP Echo Server listening on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // Receive data from client
        ssize_t recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
        if (recv_len == -1) {
            perror("recvfrom");
            continue;
        }

        printf("Received: %s", buffer);

        // Send back the received data to the client
        if (sendto(sockfd, buffer, recv_len, 0, (struct sockaddr*)&client_addr, client_len) == -1) {
            perror("sendto");
            continue;
        }
    }

    close(sockfd);

    return 0;
}
