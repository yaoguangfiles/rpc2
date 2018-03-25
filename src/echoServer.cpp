/*
 * echoServer.cpp
 * 
 * UPD echo server
 * 
 * 	loops/waits for message received from client 
 * 	      send message back to client
 * 
 * 	command line arguments:
 * 		argv[1] port number to receive from
 * 
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

void configureServer(char *serverIP, int portNum);

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cerr << "USAGE: echoServer port\n";
        exit(EXIT_FAILURE);
    }

    char* portStr = argv[1]; // the port number in the form of c string.
    int portNum = atoi(portStr); // the port number in the form of integer.

    char *serverIP = "127.0.0.2"; // the server IP.

    // start to configure the server.
    configureServer(serverIP, portNum);

    return 0;
}

/**
 * Configure server for the socket for UDP. The server will wait for
 * clients for connection. It will wait for messages sent from clients.
 * Upon receiving messages from clients, the server will respond to them.
 * @para portNum The port number through which the message will be passed.
 * @return No return value.
 */
void configureServer(char *serverIP, int portNum)
{
    char buffer[256];
    unsigned int clientlen, serverlen;
    int received = 0;

    int sock;
    struct sockaddr_in echoserver;  // structure for address of server
    struct sockaddr_in echoclient;  // structure for address of client

    // Create the UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Construct the server sockaddr_in structure
    memset(&echoserver, 0, sizeof(echoserver)); /* Clear struct */
    echoserver.sin_family = AF_INET; /* Internet/IP */
//    echoserver.sin_addr.s_addr = INADDR_ANY; /* Any IP address */

    echoserver.sin_addr.s_addr = inet_addr( serverIP );   /* the server IP address */
    echoserver.sin_port = htons(portNum); /* server port */

    // Bind the socket
    serverlen = sizeof(echoserver);
    if (bind(sock, (struct sockaddr *) &echoserver, serverlen) < 0)
    {
        perror("Failed to bind server socket");
        exit(EXIT_FAILURE);
    }

    // Run until cancelled
    while (true)
    {
        // === begin: handle RPC here ===
        // Receive a message from the client
        clientlen = sizeof(echoclient);
        if ((received = recvfrom(sock, buffer, 256, 0,
                (struct sockaddr *) &echoclient, &clientlen)) < 0)
        {
            perror("Failed to receive message");
            exit(EXIT_FAILURE);
        }
        cerr << "Client connected: " << inet_ntoa(echoclient.sin_addr) << "\n";
        //Send the message back to client
        if (sendto(sock, buffer, received, 0, (struct sockaddr *) &echoclient,
                clientlen) != received)
        {
            perror("Mismatch in number of echo'd bytes");
            exit(EXIT_FAILURE);
        }
        // === end: handle RPC ===
    }

    close(sock);
}
