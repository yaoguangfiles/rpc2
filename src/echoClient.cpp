/*
 * echoClient.cxx
 * 
 * UPD echo client
 * 
 * 	sends message to echo server
 * 	waits for message received from server 
 * 	      
 * 	command line arguments:
 * 		argv[1] IP number of server
 * 		argv[2] port number to send to
 * 		argv[3] message to send
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

struct sockaddr_in configureClient(char *ip, char *port, int &sock, struct sockaddr_in echoserver);
void sendMessage(int sock, struct sockaddr_in echoserver, char *message);
void closeSock(int sock);

int main(int argc, char *argv[])
{
	if (argc != 4) {
		cerr << "USAGE: echoClient server_ip port message\n";
		exit(EXIT_FAILURE);
	}
	
	struct sockaddr_in echoserver;  // structure for address of server

	char *ip = argv[1];
	char *port = argv[2];
    char *message = argv[3];
    int sock;

    struct sockaddr_in echoserverConf = configureClient(ip, port, sock, echoserver);

//    sendMessage(socket);
	sendMessage(sock, echoserverConf, message);

    closeSock(sock);

//	char buffer[256];
//	int echolen, received = 0;
//	unsigned int addrlen;
//
//	int sock;
//	struct sockaddr_in echoserver;  // structure for address of server
//
//	// Create the UDP socket
//	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
//		perror("Failed to create socket");
//		exit(EXIT_FAILURE);
//	}
//
//	// Construct the server sockaddr_in structure
//	memset(&echoserver, 0, sizeof(echoserver));        /* Clear struct */
//	echoserver.sin_family = AF_INET;                   /* Internet/IP */
//	echoserver.sin_addr.s_addr = inet_addr(argv[1]);   /* IP address */
//	echoserver.sin_port = htons(atoi(argv[2]));        /* server port */
//
//	// Send the message to the server
//	echolen = strlen(argv[3]);
//	if (sendto(sock, argv[3], strlen(argv[3]), 0, (struct sockaddr *) &echoserver, sizeof(echoserver)) != echolen) {
//		perror("Mismatch in number of sent bytes");
//		exit(EXIT_FAILURE);
//	}
//
//	// Receive the message back from the server
//	addrlen = sizeof(echoserver);
//	if ((received = recvfrom(sock, buffer, 256, 0, (struct sockaddr *) &echoserver, &addrlen)) != echolen) {
//		perror("Mismatch in number of received bytes");
//		exit(EXIT_FAILURE);
//	}
//
//	buffer[received] = '\0';        /* Assure null-terminated string */
//	cout << "Server (" << inet_ntoa(echoserver.sin_addr) << ") echoed: " << buffer << endl;
//
//	close(sock);
	
	return 0;
}


struct sockaddr_in configureClient(char *ip, char *port, int &sock, struct sockaddr_in echoserver)
{
//    char buffer[256];
//    int echolen, received = 0;
//    unsigned int addrlen;

//    int sock;
//    struct sockaddr_in echoserver;  // structure for address of server

    // Create the UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Construct the server sockaddr_in structure
    memset(&echoserver, 0, sizeof(echoserver));        /* Clear struct */
    echoserver.sin_family = AF_INET;                   /* Internet/IP */
    echoserver.sin_addr.s_addr = inet_addr( ip );   /* IP address */
    echoserver.sin_port = htons(atoi(port));        /* server port */

//    // Send the message to the server
//    echolen = strlen(argv[3]);
//    if (sendto(sock, argv[3], strlen(argv[3]), 0, (struct sockaddr *) &echoserver, sizeof(echoserver)) != echolen) {
//        perror("Mismatch in number of sent bytes");
//        exit(EXIT_FAILURE);
//    }
//
//    // Receive the message back from the server
//    addrlen = sizeof(echoserver);
//    if ((received = recvfrom(sock, buffer, 256, 0, (struct sockaddr *) &echoserver, &addrlen)) != echolen) {
//        perror("Mismatch in number of received bytes");
//        exit(EXIT_FAILURE);
//    }
//
//    buffer[received] = '\0';        /* Assure null-terminated string */
//    cout << "Server (" << inet_ntoa(echoserver.sin_addr) << ") echoed: " << buffer << endl;

//    close(sock);

    return echoserver;
}

void sendMessage(int sock, struct sockaddr_in echoserver, char *message)
{
    char buffer[256];
    int echolen, received = 0;
    unsigned int addrlen;

    // Send the message to the server
    echolen = strlen(message);
    if (sendto(sock, message, strlen(message), 0, (struct sockaddr *) &echoserver, sizeof(echoserver)) != echolen) {
        perror("Mismatch in number of sent bytes");
        exit(EXIT_FAILURE);
    }

    // Receive the message back from the server
    addrlen = sizeof(echoserver);
    if ((received = recvfrom(sock, buffer, 256, 0, (struct sockaddr *) &echoserver, &addrlen)) != echolen) {
        perror("Mismatch in number of received bytes");
        exit(EXIT_FAILURE);
    }

    buffer[received] = '\0';        /* Assure null-terminated string */
    cout << "Server (" << inet_ntoa(echoserver.sin_addr) << ") echoed: " << buffer << endl;
}

void closeSock(int sock)
{
    close(sock);
}
           
