// Server

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include "dns.h"

#pragma comment(lib, "Ws2_32.lib")

// argv[1] port
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Pass port!\n");
        return 1;
    }

    int PORT = atoi(argv[1]);
    printf("Server is running in port %d\n", PORT);

    WSADATA wsaData;

    SOCKET ServerSocket = INVALID_SOCKET;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Initalize to default value to be safe.
    ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ServerSocket == INVALID_SOCKET) {
        printf("socket failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddr;

    // Bind the socket to any address and the specified port.
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr.s_addr);
    serverAddr.sin_port = htons(PORT);

    if (bind(ServerSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr))) {
        printf("bind failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    char receiveBuf[1025];
    int receiveBufLen = 1024;

    while (true) {
        // Keep a seperate address struct to store sender information. 
        struct sockaddr_in SenderAddr;
        int SenderAddrSize = sizeof(SenderAddr);

        while (true) {
            // Receive from client
            int bytes_received = recvfrom(ServerSocket, receiveBuf, receiveBufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);

            if (bytes_received == SOCKET_ERROR) {
                printf("recvfrom failed with error %d\n", WSAGetLastError());
                break;
            }

            if (bytes_received < 1025) {
                receiveBuf[bytes_received] = '\0';
            }

            printf("receive: %s\n", receiveBuf);

            // Send to client
            const char* sendBuf = get_result(receiveBuf);
            int sendResult = sendto(ServerSocket,sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&SenderAddr, SenderAddrSize);
            if (iResult == SOCKET_ERROR) {
                printf("Sending back response got an error: %d\n", WSAGetLastError());
            }
        }
    }
    
}