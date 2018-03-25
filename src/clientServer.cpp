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
struct sockaddr_in configureClient(char *ip, char *port, int &sock, struct sockaddr_in echoserver, const char *localIP);
void sendMessage(int sock, struct sockaddr_in echoserver, char *message);
void closeSock(int sock);

int main(int argc, char *argv[])
{

    if (argc != 5)
    {
    	printf("(Yao): incorrect number of argument. Number of argument is %d, not 4.\n", argc);
        cerr << "USAGE: clientServer localIP localPort remoteIP remotePort\n";
        exit(EXIT_FAILURE);
    }

    // ----------------- configure for the Chord server ------------

    char *localIP = argv[1]; // the server IP.
    char* localPortStr = argv[2]; // the port number in the form of c string.
    int localPortNum = atoi(localPortStr); // the port number in the form of integer.

	// start to configure the server.
//**	configureServer(localIP, localPortNum);

	char *remoteIP = argv[3]; // the server IP.
	char *remotePortStr = argv[4]; // the port number in the form of c string.
	int remotePortNum = atoi(remotePortStr); // the port number in the form of integer.

    // ----------------- configure for a Chord client ------------

//	char *port = argv[2]; // the port will be used for UDP messages
	                      // transmission.
//    char *message = argv[3]; // the UDP message the client will send to the server.
    int sock;

//    const char *localIP = "127.0.0.100"; // assign this ip to local client.
//    const char *localIP = serverIP; // assign this ip to local client.
//    char *serverIP = "127.0.0.2"; // the server IP address that the client will
//    	                          // connect to.
//    char *serverIP = argv[1]; // the server IP address that the client will
       	                          // connect to.

    struct sockaddr_in echoserver;  // structure for address of the server.
    struct sockaddr_in echoserverConf; // structure for address of the
                                       // server that is pass from
                                       // configuring the client.

    // ---------------- fork 2 processes ----------------

    pid_t pid;

    	// fork will make 2 processes
    	pid = fork();

    	if (pid == -1)
    	{
    		perror("fork");
    		exit(EXIT_FAILURE);
    	}

    	if (pid == 0)
    	{

    		// start to configure the server.
    	    configureServer(localIP, localPortNum);

//    		// Child process: fork() returned 0
//    		int j;
//    		for (j = 0; j < 10; j++)
//    		{
//    			cout << "child: " << j << endl;
//    			sleep(1);
//    		}
    	}
    	else
    	{

    		// start to configure the client socket.
    		echoserverConf = configureClient(remoteIP, remotePortStr, sock, echoserver, localIP);


//    		// Parent pocess: fork() returned a positive number
//    		int i;
//    		for (i = 0; i < 10; i++)
//    		{
//    			cout << "parent: " << i << endl;
//    			sleep(1);
//    		}
    	}



    // start to configure the client socket.
//    struct sockaddr_in echoserverConf = configureClient(remoteIP, remotePortStr, sock, echoserver, localIP);

    // -----------------------------------------------------

    char msg[100];

    while(true)
    {
	    printf("\n(Yao): any entered a message will be sent to the server: %s:%s\n", remoteIP, remotePortStr);
	    printf("\n(Yao): Enter a message here: ");

	    // Accept input. "+2" is for the \n and \0 characters.
	    fgets(msg, 100, stdin);

	    // send the message to the server.
	    sendMessage(sock, echoserverConf, msg);
    }

	// close the socket.
//    closeSock(sock);

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
        cerr << "(Yao): The server connected to client: (" << inet_ntoa(echoclient.sin_addr) << ")\n";

        buffer[received] = '\0';        // Assure null-terminated string
        cout << "(Yao): The server received message from client: (" << inet_ntoa(echoclient.sin_addr) << ") and the message: " << buffer << endl;

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


/**
 * Configure the client for the socket for UDP.
 * @para ip The ip address to which the message will be sent to.
 * @para port The port through which the message will be passed.
 * @para sock The socket descriptor.
 * @para echoserver The structure describing an Internet socket address.
 * @return sockaddr_in The structure describing an Internet socket address.
 *     I will be used for sending and receiving messages from the server.
 */
struct sockaddr_in configureClient(char *ip, char *port, int &sock, struct sockaddr_in echoserver, const char *localIP)
{
    // Create the UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }



    // Bind to a specific network interface (and optionally a specific local port)
    struct sockaddr_in localaddr;
    localaddr.sin_family = AF_INET;
    localaddr.sin_addr.s_addr = inet_addr(localIP);
    localaddr.sin_port = 0;  // Any local port will do
    bind(sock, (struct sockaddr *)&localaddr, sizeof(localaddr));

    // Construct the server sockaddr_in structure
    memset(&echoserver, 0, sizeof(echoserver));        /* Clear struct */
    echoserver.sin_family = AF_INET;                   /* Internet/IP */
    echoserver.sin_addr.s_addr = inet_addr( ip );   /* IP address */
    echoserver.sin_port = htons(atoi(port));        /* server port */

    return echoserver;
}

/**
 * The client will send a message to the server.
 * @para sock The socket descriptor.
 * @para echoserver The structure describing an Internet socket address.
 * @para message The message will be sent to the server.
 * @return No return value.
 */
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

/**
 * Close the client UDP socket.
 * @para sock The socket descriptor.
 * @return No return value.
 */
void closeSock(int sock)
{
    close(sock);
}
