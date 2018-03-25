/*
 * echoServer.cxx
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

void configureServer(char *argv[]);

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cerr << "USAGE: echoServer port\n";
        exit(EXIT_FAILURE);
    }


    configureServer(argv);

//    char buffer[256];
//    unsigned int clientlen, serverlen;
//    int received = 0;
//
//    int sock;
//    struct sockaddr_in echoserver;  // structure for address of server
//    struct sockaddr_in echoclient;  // structure for address of client
//
//    // Create the UDP socket
//    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
//    {
//        perror("Failed to create socket");
//        exit(EXIT_FAILURE);
//    }
//
//    // Construct the server sockaddr_in structure
//    memset(&echoserver, 0, sizeof(echoserver)); /* Clear struct */
//    echoserver.sin_family = AF_INET; /* Internet/IP */
//    echoserver.sin_addr.s_addr = INADDR_ANY; /* Any IP address */
//    echoserver.sin_port = htons(atoi(argv[1])); /* server port */
//
//    // Bind the socket
//    serverlen = sizeof(echoserver);
//    if (bind(sock, (struct sockaddr *) &echoserver, serverlen) < 0)
//    {
//        perror("Failed to bind server socket");
//        exit(EXIT_FAILURE);
//    }
//
//    // Run until cancelled
//    while (true)
//    {
//        // Receive a message from the client
//        clientlen = sizeof(echoclient);
//        if ((received = recvfrom(sock, buffer, 256, 0,
//                (struct sockaddr *) &echoclient, &clientlen)) < 0)
//        {
//            perror("Failed to receive message");
//            exit(EXIT_FAILURE);
//        }
//        cerr << "Client connected: " << inet_ntoa(echoclient.sin_addr) << "\n";
//        //Send the message back to client
//        if (sendto(sock, buffer, received, 0, (struct sockaddr *) &echoclient,
//                clientlen) != received)
//        {
//            perror("Mismatch in number of echo'd bytes");
//            exit(EXIT_FAILURE);
//        }
//    }
//
//    close(sock);

    return 0;
}

void configureServer(char *argv[])
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
    echoserver.sin_addr.s_addr = INADDR_ANY; /* Any IP address */
    echoserver.sin_port = htons(atoi(argv[1])); /* server port */

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
