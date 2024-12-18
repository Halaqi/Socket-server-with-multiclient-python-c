#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 6000
#define BUFFER_SIZE 1024

// void error_exit(const char *message) {
//     perror(message);
//     exit(EXIT_FAILURE);
// }

int main() {
    WSADATA wsaData;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int bytes_sent, bytes_received;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Failed to initialize Winsock.\n");
        return 1;
    }

    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) {
        error_exit("Socket creation failed");
    }

    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        error_exit("Connection failed");
    }
    printf("Connected to the server. Type 'exit' to quit.\n");

    while (1) {
        printf("Enter the medicine you want to buy: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        // If user wants to exit, send 'exit' and break
        if (strcmp(buffer, "exit") == 0) {
            send(client_socket, buffer, strlen(buffer), 0);
            break;
        }

        // Send the request to the server
        bytes_sent = send(client_socket, buffer, strlen(buffer), 0);
        if (bytes_sent == SOCKET_ERROR) {
            error_exit("Failed to send data");
        }

        // Receive the server's response
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received == SOCKET_ERROR) {
            error_exit("Failed to receive data");
        }

        buffer[bytes_received] = '\0'; // Null-terminate the string
        printf("Server response: %s\n", buffer);
    }

    // Close the socket
    closesocket(client_socket);
    WSACleanup();
    return 0;
}
