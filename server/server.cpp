#define _GLIBCXX_USE_CXX11_ABI 0

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

using std::cout;
using std::endl;
using std::string;


#define BUFSIZE 1024

void* HandleTCPClient(void* clntSocket1)
{
	std::thread::id this_id = std::this_thread::get_id();
	intptr_t clntSocket = (intptr_t)clntSocket1;
	char buf[BUFSIZE];     /* message buffer */
	int fd;                
	struct stat sbuf;      /* file status */



	cout << "thread id: " << this_id <<  "  " << "HANDLING NEW CLIENT CONNECTION " << endl;
	int n = read(clntSocket,buf,255);
	string string_buf(buf);
	if (n < 0) cout << "ERROR reading from socket" << endl;

	string input(string_buf);
	std::istringstream inputString(input);
	string smethod;
	string suri;
	string sversion;

	inputString >> smethod >> suri >> sversion;
	string sfilename; //= suri.substr(1);
	if (suri.substr(0,0) == string("\ ").substr(0,0) && suri.length() == 1 ){
		sfilename = string("TMDG.html");
	}else{
		sfilename = suri.substr(1);
	}

	cout << "\n\n parsed inputs: " << endl;
	cout << "input string: " << input << endl;
	cout << "smethod: " << smethod << endl;
	cout << "suri: " << suri << endl;
	cout << "sversion: " << sversion << endl;
	cout << "sfilename: " << sfilename << endl;

    if (stat(sfilename.c_str(), &sbuf) < 0) {
	string errno1("404"); 
	string smess1("Not Found");
	string sdesc1("Spiros couldn't find this file");
	string response_string(string("HTTP/1.1 ") + errno1 + string(" ") + smess1 + string("\r\n") + string("Content-length: ") + std::to_string((long long int)sdesc1.length())+ string("\r\n\r\n") + sdesc1  /*string("<hr><em>Spiros Server</em>\n"*)*/);
	send(clntSocket, response_string.c_str(), response_string.length(), 0);
	printf("\nclosed connection with 404 error\n");
	close(clntSocket);
	pthread_exit(NULL);
	return (void*)NULL;
    }

   if (smethod != string("GET")) {
	string errno1("501"); 
	string smess1("Not Implemented");
	string sdesc1("spiros does not implement this method");
	string response_string(string("HTTP/1.1 ") + errno1 + string(" ") + smess1 + string("\r\n") + string("Content-length: ") + std::to_string((long long int)sdesc1.length())+ string("\r\n\r\n") + sdesc1  /*string("<hr><em>Spiros Server</em>\n"*)*/);
	send(clntSocket, response_string.c_str(), response_string.length(), 0);
	printf("\nclosed connection with 505 error\n");
	close(clntSocket);
	pthread_exit(NULL);
	return (void*)NULL;
    }


      /* print response header */
	cout <<  "\nresponse headers to send: \n" << endl;
	string sdesc2("test body");
	/*string response_headers(string("HTTP/1.1 200 OK\r\n") + string("Content-length: ") + string("5") + string("\r\n") + string("Content-type: ") + string("text/html") + string("\r\n\r\n") + sdesc2/*+ string("<h1> HTML test </h1>")); 
	cout << response_headers << endl;*/
	std::ifstream stream(sfilename);
	std::string line; 
	std::string body("."); 		
	while (std::getline(stream, line)) {
		//cout << "line: " << line;
		body = body + line;
  	}

	string response_headers(string("HTTP/1.1 200 OK\r\n") + string("Content-length: ") + std::to_string((long long int)body.length() + 50) + string("\r\n") + string("Content-type: ") + string("text/html") + string("\r\n\r\n") + sdesc2/*+ string("<h1> HTML test </h1>")*/); 
	string entire_response = response_headers + body/* + string("</html>")*/; 
	send(clntSocket, entire_response.c_str(), entire_response.length(), 0);
	cout << "SENT headers and body" << endl;
	char bufend[BUFSIZE];     /* message buffer */
	bool stay = true;
	while(stay){//detect the ending of the html file
		int n = read(clntSocket,bufend,255);
		string string_bufend(bufend);
		if(string_bufend.find(string("</html>")) != std::string::npos ){
			cout << "HERE" << endl;
			stay = false;
		}
	}

	cout << "closing connection of thread id : " << this_id << "  .." << endl;
	close(clntSocket);
	pthread_exit(NULL);
}


void ShowError(char *errorMessage){

	printf("error %s\n",errorMessage);
}






