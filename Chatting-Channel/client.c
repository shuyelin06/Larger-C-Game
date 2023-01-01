#define _WIN32_WINNT 0x501
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h> // Memory Management

#include <stdio.h> // I/O

#include <string.h> // Strings

// gcc client.c -o client -lws2_32 -lmswsock -ladvapi32
// gcc client.c -o client -lws2_32
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib

// #pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
// #pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

// Connection Data for Both Threads to Use
#define CONNECT 0
#define DISCONNECT 1

typedef struct _ConnectionData ConnectionData;
struct _ConnectionData {
    SOCKET *server;
    
    int connect;
};

// Client - Send Message to Server
DWORD WINAPI Send(LPVOID lpParam) {
    ConnectionData *data = (ConnectionData*) lpParam;   // Server Socket and Data
    char message[DEFAULT_BUFLEN];       // Message to Send to Server

    printf("Sending Channel Initialized \n");

    do {
        // Clear Message
        memset(message, 0, DEFAULT_BUFLEN * sizeof(message[0]));
    
        // Obtain User Input
        scanf("%s", message);
        if ( strcmp(message, "EXIT") == 0 ) {
            data->connect = DISCONNECT;
            break;
        }

        // Send to Server
        int status = send(*(data->server), message, (int) strlen(message), 0);
        
        // Check for Failures
        if ( status == SOCKET_ERROR || status == 0 ) {
            data->connect == DISCONNECT;
        }

        
    } while ( data->connect == CONNECT );

    shutdown(*(data->server), SD_SEND);
    printf("Sending Channel Terminated \n");

    return 0;
}

// Client - Receive Message from Server
DWORD WINAPI Receive( LPVOID lpParam ) {
    ConnectionData *data = (ConnectionData*) lpParam;   // Server Socket and Data

    char buffer[DEFAULT_BUFLEN];        // Buffer to Send
    int bufferLen = DEFAULT_BUFLEN;     // Buffer Length
    
    printf("Receiving Channel Initialized \n");

    do {
        // Clear Buffer
        memset(buffer, 0, DEFAULT_BUFLEN * sizeof(buffer[0]));

        // Receive Message from Server
        int result = recv(*(data->server), buffer, bufferLen, 0);
        if ( result == 0 ) {
            data->connect = DISCONNECT;
            break;
        }

        printf("> ");
        printf(buffer);
        printf("\n");
    } while ( data->connect == CONNECT );

    shutdown(*(data->server), SD_RECEIVE);
    printf("Receiving Channel Terminated \n");
}

int __cdecl main(int argc, char **argv) 
{   
    /* --- Validate Program Parameters --- */
    if (argc != 2) {
        printf("usage: %s server-name \n", argv[0]);
        
        return 1;
    }

    int iResult;

    /* --- Initialize Winsock Library --- */ 
    WSADATA wsaData; // To be filled with information about the windows socket implementation

    if ( WSAStartup(MAKEWORD(2,2), &wsaData) != 0 ) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    /* --- Instantiate a Socket for the Client --- */
    // Determine Settings for the Client Socket
    struct addrinfo hints;                  // Holds Information About the Client's Socket

    ZeroMemory( &hints, sizeof(hints) );    // Sets all bits to 0

    hints.ai_family = AF_UNSPEC;            // Specifies the Address Family (IPv4)
    hints.ai_socktype = SOCK_STREAM;        // Specify the Socket Type (Stream Socket)
    hints.ai_protocol = IPPROTO_TCP;        // Specifies the Protocol (TCP)

    // Resolve the server address and port
    struct addrinfo *result = NULL;     // Stores Address Information For Server

    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        
        WSACleanup();
        
        return 1;
    }

    // Create a Socket to Connect to the Server
    SOCKET ConnectSocket = INVALID_SOCKET;

    struct addrinfo *ptr = NULL;
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    /* Communicate With the Server With Threads */
    // Create ConnectionData Struct for Threads to Use
    ConnectionData data = {&ConnectSocket, CONNECT};

    printf("Creating Threads! \n");

    HANDLE threads[2];
    threads[0] = CreateThread(NULL, 0, Send, &data, 0, NULL);
    threads[1] = CreateThread(NULL, 0, Receive, &data, 0, NULL);
    
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    // Clean-Up
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}