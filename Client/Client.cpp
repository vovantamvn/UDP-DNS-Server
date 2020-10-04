// Client

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "Ws2_32.lib")

// argv[1] ip
// argv[2] port
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Pass ip and port!\n");
        return 1;
    }

    int PORT = atoi(argv[2]);

    WSADATA wsaData;

    SOCKET SendSocket = INVALID_SOCKET;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Initalize to default value to be safe.
    SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (SendSocket == INVALID_SOCKET) {
        printf("socket failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in ServerAddr;
    int serverAddrSize = (int)sizeof(ServerAddr);
    
    ServerAddr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &ServerAddr.sin_addr.s_addr);
    ServerAddr.sin_port = htons(PORT);

    char receiveBuf[1025];
    int receiveBufLen = 1024;

    while (true) {

        char sendBuf[1024];
        printf("Enter the ip or domain: ");
        fgets(sendBuf, sizeof(sendBuf), stdin);

        // Check empty text
        if (sendBuf[0] == '\n') {
            puts("Close program!");
            break;
        }

        // Send an initial buffer
        iResult = sendto(SendSocket, sendBuf, strlen(sendBuf) - 1, 0, (SOCKADDR*)&ServerAddr, serverAddrSize);
        if (iResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(SendSocket);
            WSACleanup();
            return 1;
        }

        // Receive from client
        int bytes_received = recvfrom(SendSocket, receiveBuf, receiveBufLen, 0, (SOCKADDR*)&ServerAddr, &serverAddrSize);

        if (bytes_received < 1025) {
            receiveBuf[bytes_received] = '\0';
        }

        puts(receiveBuf);
    }

    // cleanup
    closesocket(SendSocket);
    WSACleanup();

}