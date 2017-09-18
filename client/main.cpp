#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */ 
#include <arpa/inet.h>   /* for sockaddr_in and inet_addr() */ 
#include <stdlib.h>        /* for atoi() and exit() */ 
#include <string.h>       /* for memset() */ 
#include <unistd.h>      /* for close() */  
#include <netdb.h> /* struct hostent, gethostbyname */
#include <string>
#include <sys/time.h>

#include "include/client.hpp"


using namespace std;
#define RCVBUFSIZE 10000   /* Size of receive buffer */ 




int main(int argc,char* argv[]){

	int sock;   /* Socket descr */ 
	int received, bytes,total;
	struct sockaddr_in servAddr;  /* server address */ 
	unsigned short servPort;         /* server port */ 
	char *servIP;                                   /* Server IP address */
	char *servURL;                                     
	char *getRequest;                            /* Request string to send toserver */ 
	std::string getRequestString;

	char* buffer = (char*) malloc(RCVBUFSIZE*sizeof(char)); 
	unsigned int getRequestLen;           /* Length of string to send */ 
	int bytesRcvd, totalBytesRcvd;      /* Bytes read in single recv() and total bytes read */
    	struct hostent *server;
	bool print;  //vars for arg inputs
	bool useIP;
    	struct timeval t1, t2; //time variables
    	double elapsedTime;


	srand(time(NULL));
 
	if ((argc < 3) || (argc > 4))    /* Test for correct number of arguments */ 
	{ 
		cout << argc ;
		printf(" here1\n");
        	fprintf(stderr, "Usage: %s [-options] server_url port_number\n", argv[0]); 
        	fprintf(stderr, "options include: -p, to print the RTT time of the server-client exchange"); 
        	exit(1); 
        } 

	if(strcmp(argv[1],"-p") == 0 && argc == 4){
		print = true;
	}else if(strcmp(argv[1],"-p") == 0 && argc != 4){
		printf("here2\n");
        	fprintf(stderr, "Usage: %s [-options] server_url port_number\n", argv[0]); 
        	exit(1); 
	}else{
		print = false;
	}

	if(print){
		servURL = argv[2];             /* First arg: server IP address */ 
		servPort = atoi(argv[3]);         /* Second arg: port */ 
	}else{
		servURL = argv[1];             /* First arg: server IP address (dotted quad) */ 
		servPort = atoi(argv[2]);         /* Second arg: port */ 
	}

	if(isIPaddr(servURL)){
		useIP = true;
	}else{
		useIP = false;
	}


	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)  ShowError("socket() failed"); 

	std::string page1 (page(servURL));
	std::string host1 (host(servURL));
	string header2 = string("GET ") + page1 + string(" HTTP/1.1\r\nHost: ") + host1 + string("\r\n\r\n");
	const char* header5 = header2.c_str();

	/* lookup the ip address */
	if(!useIP){
		server = gethostbyname(host1.c_str()/*"www.cnn.com"host*/);
		if (server == NULL) fprintf(stderr, "ERROR, no such host");
	}


	/* Construct the server address structure */ 
	memset(&servAddr, 0, sizeof(servAddr));     /* Zero out structure */      
	servAddr.sin_family      = AF_INET;             /* Internet address family */ 
	servAddr.sin_port        = htons(servPort);     /* Server port */ 
	if(useIP){
		printf("using IP @%s@\n",servURL);
		servAddr.sin_addr.s_addr = inet_addr(servURL);
	}else{
    		memcpy(&servAddr.sin_addr.s_addr,server->h_addr,server->h_length); 
	}

	//START TIMER
    	gettimeofday(&t1, NULL);
	if (connect(sock, (struct sockaddr *) & servAddr, sizeof(servAddr)) < 0)   ShowError("connect() failed");
	getRequestLen = header2.length();          /* Determine input length */
	cout << "sending request: " << header5 << endl;
	if (send (sock, header5, getRequestLen, 0) != getRequestLen) ShowError("send() sent a different number of bytes than expected");
	totalBytesRcvd = 0; 
	bytesRcvd = 5;
	printf("Received - START OF RECEPTION -printing only the headers, the entire response is stored in a txt file: \n\n\n");
	string r_filename = "response" + to_string((long long int)rand()%100000) + ".txt";
	bool stay = true;
	bool first = true;
	FILE *file = fopen(r_filename.c_str(), "a+");
	while (stay) 
	{ 		
		if ((bytesRcvd = recv(sock, buffer, RCVBUFSIZE - 1, 0)) <= 0) {stay = false;ShowError("recv() failed or connection closed prematurely");}  
		totalBytesRcvd += bytesRcvd;   // Keep track of total bytes  
		buffer[bytesRcvd+1] = '\0';  // null Terminate the string!   
		string cback(buffer);


		string delim = string("\r\n\r\n");
		size_t pos = cback.find(delim);
		string headers;
		string body;
		if(first){
			headers = cback.substr(0,pos + delim.length() + 1);
			body = cback.substr(pos + delim.length() + 2, cback.length() - pos);
			cout <<  headers << endl;
			first = false;
		}

		int results = fputs(buffer, file);
		if (results == EOF) {
    			cout << "failed to write to file\n";
		}

		//LOOK FOR THE HTML ENDING
		if(cback.find(string("</html>")) != std::string::npos){
			send (sock, string("\n</html>\n").c_str(), string("\n</html>\n").length(), 0); 
			stay = false;
		}
	}
    	gettimeofday(&t2, NULL); //stop timer
	printf("\nEND of reception\n");
	printf("\nresponse stored in %s \n",r_filename.c_str());
	fclose(file);
	if(print){
    		elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
   		elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    		cout << "RTT: "<< elapsedTime << " ms.\n";

	}

	printf("\n");    /* Print a final linefeed */ 
	close(sock); 
	exit(0); 
} 



