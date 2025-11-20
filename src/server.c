#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
// Placeholder for non-Windows networking includes
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // For inet_ntoa
#endif

#define PORT 8080
#define MAX_CLIENTS 30
#define BUFFER_SIZE 1024

int main() {
    // --------------------------------------------------------------------
    // STEP 1: INITIALIZE WINSOCK (Windows-specific)
    // --------------------------------------------------------------------
    #if defined(_WIN32)
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }
    #endif

    SOCKET listen_socket;
    SOCKET client_sockets[MAX_CLIENTS] = {0}; // Array to hold client sockets

    // --------------------------------------------------------------------
    // STEP 2: CREATE A LISTENING SOCKET
    // --------------------------------------------------------------------
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == INVALID_SOCKET) {
        printf("Socket creation failed.\n");
        return 1;
    }

    // --------------------------------------------------------------------
    // STEP 3: BIND THE SOCKET
    // --------------------------------------------------------------------
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed.\n");
        closesocket(listen_socket);
        return 1;
    }

    // --------------------------------------------------------------------
    // STEP 4: LISTEN FOR CONNECTIONS
    // --------------------------------------------------------------------
    if (listen(listen_socket, 3) < 0) { // Listen with a backlog of 3
        perror("Listen failed");
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    // --------------------------------------------------------------------
    // STEP 5: MAIN SERVER LOOP (USING SELECT)
    // --------------------------------------------------------------------
    fd_set read_fds; // Set of file descriptors to monitor for reading
    int max_sd;      // Maximum socket descriptor number

    while (1) { // Infinite loop to keep the server running
        // 1. Clear the socket set
        FD_ZERO(&read_fds);

        // 2. Add the listening socket to the set
        FD_SET(listen_socket, &read_fds);
        max_sd = listen_socket;

        // 3. Add all active client sockets to the set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            SOCKET sd = client_sockets[i];
            if (sd > 0) {
                FD_SET(sd, &read_fds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        // 4. Wait for activity on any of the sockets using select()
        // select() is a blocking call. It will wait until at least one socket is ready.
        int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            printf("select() error.\n");
        }

        // 5. Check for incoming connections on the listening socket
        if (FD_ISSET(listen_socket, &read_fds)) {
            SOCKET new_socket;
            struct sockaddr_in client_addr;
            int addrlen = sizeof(client_addr);
            
            new_socket = accept(listen_socket, (struct sockaddr*)&client_addr, &addrlen);
            if (new_socket < 0) {
                perror("Accept failed");
                continue; // Continue to next iteration
            }

            printf("New connection: socket fd is %d, ip is: %s, port: %d\n",
                   new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Add the new socket to the array of client sockets
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
            }
        }

        // 6. Check for activity on existing client sockets
        for (int i = 0; i < MAX_CLIENTS; i++) {
            SOCKET sd = client_sockets[i];
            if (FD_ISSET(sd, &read_fds)) {
                char buffer[BUFFER_SIZE] = {0};
                int valread = recv(sd, buffer, BUFFER_SIZE, 0);

                if (valread == 0) {
                    // Client disconnected
                    struct sockaddr_in client_addr;
                    int addrlen = sizeof(client_addr);
                    getpeername(sd, (struct sockaddr*)&client_addr, &addrlen);
                    printf("Client disconnected: ip %s, port %d\n",
                           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    
                    closesocket(sd);
                    client_sockets[i] = 0; // Mark as empty
                } else if (valread > 0) {
                    // Data received from a client
                    buffer[valread] = '\0'; // Null-terminate the string
                    printf("Received from client %d: %s\n", i, buffer);
                    
                    // For now, we just print it. Later we will parse commands.
                }
            }
        }
    }

    // The code below will not be reached in this simple version
    // but is good practice for a server that can be shut down gracefully.
    closesocket(listen_socket);
    #if defined(_WIN32)
    WSACleanup();
    #endif

    return 0;
}
