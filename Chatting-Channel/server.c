#define _WIN32_WINNT 0x501
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>     // Memory Management
#include <stdio.h>      // Standard I/O

// Need to link with Ws2_32.lib
// #pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_PORT "27015"    // Server Port Number
#define DEFAULT_BUFLEN 512      // Default Buffer Length for Communications

#define MAX_CONNECTIONS 10      // Maximum Number of Client Connections Allowed

// Data for Receiving Connections
typedef struct _ReceiveConnectionsData ReceiveConnectionsData;
struct _ReceiveConnectionsData {
    SOCKET *ClientSockets;
    SOCKET *Socket;
};

// Server - Receive Message from Client
DWORD WINAPI ReceiveMessages(LPVOID lpParam) {
    ReceiveConnectionsData *data = (ReceiveConnectionsData*) lpParam;

    char buffer[DEFAULT_BUFLEN];       // Receiving Buffer
    int bufferLen = DEFAULT_BUFLEN;    // Receiving Buffer Length

    int result = -1;                   // Number of Bytes Received 

    // Receive Messages from a Client
    do {
        // Clear Buffer
        memset(buffer, 0, DEFAULT_BUFLEN * sizeof(buffer[0]));

        // Receive Message from Client
        result = recv(*(data->Socket), buffer, bufferLen, 0); 
        printf("Bytes received: %d\n", result);
        // Check if Valid Message
        if ( result > 0 ) {
            // Print Message
            printf("Message Received: ");
            printf(buffer);
            printf("\n");

            // Echo to All Clients
            for ( int i = 0; i < MAX_CONNECTIONS; i++ ) {
                // If Client is Valid Socket
                if ( data->ClientSockets[i] == INVALID_SOCKET )
                    continue;

                // Send to Client
                int sendResult = send(data->ClientSockets[i], buffer, result, 0);

                // If Sending Failed, Close the Connection
                if ( sendResult == SOCKET_ERROR ) {
                    printf("Error Sending \n");
                    
                    closesocket(data->ClientSockets[i]);
                    data->ClientSockets[i] = INVALID_SOCKET;
                }

            }

        } 
        else {
            shutdown(*(data->Socket), SD_RECEIVE);
        }

    } while ( result > 0 );
    
    shutdown(*(data->Socket), SD_SEND);
    closesocket(*(data->Socket));

    *(data->Socket) = INVALID_SOCKET;

    printf("Client left. \n");
}

int __cdecl main(void) {
    int iResult; // Stores the results of winsock functions

    /* --- Initialize Winsock Library --- */ 
    WSADATA wsaData; // To be filled with information about the windows socket implementation

    if ( WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    
    /* --- Instantiate a Socket for the Server --- */
    // Determine Settings for the Server's Socket
    struct addrinfo hints;              // Holds Information About the Server's Socket
    
    ZeroMemory(&hints, sizeof(hints));  // Sets all bits to 0

    hints.ai_family = AF_INET;          // Specifies the Address Family (IPv4)
    hints.ai_socktype = SOCK_STREAM;    // Specify the Socket Type (Stream Socket)
    hints.ai_protocol = IPPROTO_TCP;    // Specifies the Protocol (TCP)
    hints.ai_flags = AI_PASSIVE;        // Intends to use the address in the bind() function (to create a server)

    // Resolve Server Address and Port
    struct addrinfo *result = NULL;     // Stores Address Information (?) 
    
    if ( getaddrinfo(NULL, DEFAULT_PORT, &hints, &result) != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        
        WSACleanup(); // Clean-Up Winsock Library on Failure

        return 1;
    }

    // Create a Socket to Listen for Client Connections
    SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        printf("Socket Creation Failed With Error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Binds the Socket to an IP Address and Port, Allowing it to Accept Client Connections
    if ( bind( ListenSocket, result->ai_addr, (int) result->ai_addrlen) == SOCKET_ERROR ) {
        printf("bind failed with error: %d\n", WSAGetLastError());

        freeaddrinfo(result);       // Free Address Information on Failure
        closesocket(ListenSocket);  // Close Created Socket on Failure
        WSACleanup();               // Clean-Up Winsock Library on Failure
        
        return 1;
    }
    freeaddrinfo(result); // Free Address Information - Socket has been bound

    // Listen for Client Connections
    if ( listen(ListenSocket, MAX_CONNECTIONS) == SOCKET_ERROR ) {        // 2nd Parameter - Number of Pending Connections Allowed
        printf("listen failed with error: %d\n", WSAGetLastError());

        closesocket(ListenSocket);
        WSACleanup();
        
        return 1;
    }

    // Connection Threads
    HANDLE threads[MAX_CONNECTIONS];
    
    // Stores Connection Data
    ReceiveConnectionsData connectionsData[MAX_CONNECTIONS];

    // Stores Client Sockets
    SOCKET clients[MAX_CONNECTIONS]; // All Client Sockets
    for ( int i = 0; i < MAX_CONNECTIONS; i++ ) { // Initialize All to Invalid
        clients[i] = INVALID_SOCKET;
    }

    // Begin Accepting Connections
    printf("Ready to Accept Connections! \n");
    int terminate = 1;
    
    while ( terminate != 0 ) {

        for ( int i = 0; i < MAX_CONNECTIONS; i++ ) {
            if ( clients[i] != INVALID_SOCKET ) 
                continue;
            
            clients[i] = accept(ListenSocket, NULL, NULL); // Accept a Client Socket
            if ( clients[i] != INVALID_SOCKET ) {
                printf("New client joined! \n");

                // Create New Thread to Process
                connectionsData[i].ClientSockets = clients;
                connectionsData[i].Socket = &(clients[i]);
                threads[i] = CreateThread(NULL, 0, ReceiveMessages, &(connectionsData[i]), 0, NULL);

                i = 0;
            } 
        }
    
        
        printf("Max Reached. Continue Accepting Connections? (YES/NO) \n");
        char userInput[10];
        scanf("%s", userInput);
        if ( strcmp(userInput, "NO") ) {
            terminate = 0; 
        }

    }
    printf("No More Connections Will Be Accepted. \n");
    closesocket(ListenSocket);

    WaitForMultipleObjects(MAX_CONNECTIONS, threads, TRUE, INFINITE);

    // Clean Up
    WSACleanup();

    return 0;
}