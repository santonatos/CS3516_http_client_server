//#define _GLIBCXX_USE_CXX11_ABI 0

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */ 
#include <arpa/inet.h>   /* for sockaddr_in and inet_addr() */ 
#include <stdlib.h>        /* for atoi() and exit() */ 
#include <string.h>       /* for memset() */ 
#include <unistd.h>      /* for close() */  
#include <netdb.h> /* struct hostent, gethostbyname */


#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/mman.h>
#include <netinet/in.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <cstdint>
#include <thread>

#include "include/server.hpp"

//using namespace std;
using std::cout;
using std::endl;
using std::string;

#define RCVBUFSIZE 10000   /* Size of receive buffer */ 
#define BUFSIZE 1024
#define MAXERRS 16
#define MAXPENDING 5    /* Maximum outstanding connection requests */ 

 
int main(int argc,char* argv[]){
	int servSock;                    /*Socket descriptor for server */ 
	//int clntSock;                    /* Socket descriptor for client */ 
	struct sockaddr_in servAddr; /* Local address */ 
	struct sockaddr_in clntAddr; /* Client address */ 
	unsigned short servPort;     /* Server port */ 
	unsigned int clntLen;            /* Length of client address data structure */

	if (argc != 2)     /* Test for correct number of arguments */
	{  
		fprintf(stderr, "Usage:  %s <Server Port>\n", argv[0]); 
		exit(1);
	} 
	servPort = atoi(argv[1]);  /* First arg:  local port */ 
      /* Create socket for incoming connections */ 
	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)  
		ShowError("socket() failed"); 

	/* Construct local address structure */
	memset(&servAddr, 0, sizeof(servAddr));       /* Zero out structure */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	/* Any incoming interface */
	servAddr.sin_port = htons(servPort);                 /* Local port */

	/* Bind to the local address */

	if ( bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0 ) {ShowError("bind() failed");}

	/* make the socket listen for incoming connections */ 
	if (listen(servSock, MAXPENDING) < 0) ShowError("listen() failed"); 

	for (;;) /* Run indefinetely */ 
	{ 
		std::thread::id main_id = std::this_thread::get_id();
		pthread_t* new_thread = (pthread_t*)malloc(sizeof(pthread_t)); //new thread at each loop iter
		int tc; 
		int clntSock;
		/* Set the size of the in-out parameter */ 
		cout <<  "\n\n\nwaiting for client connection..\n";
		clntLen = sizeof(clntAddr);        /* Wait for a client to connect */ 
		if ((clntSock = accept(servSock, (struct sockaddr*) & clntAddr, &clntLen)) < 0) ShowError("accept() failed");
		cout <<  "accepted connection at: " << clntSock << endl; 
		/* clntSock is connected to a client! */ 
		cout <<"About to handle client: " <<  inet_ntoa(clntAddr.sin_addr) << endl; 

		tc = pthread_create(new_thread, NULL, HandleTCPClient, (void *)(clntSock)); //call thread creator for mulithread
		//HandleTCPClient(*clntSock); 
	} 
     /* NOT REACHED */ 
	 
} 





