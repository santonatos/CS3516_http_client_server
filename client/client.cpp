#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "include/client.hpp"


using namespace std;

void ShowError(char *errorMessage){

	printf("error %s\n",errorMessage);
}


bool isIPaddr(char* input){
	for(int i = 0;i < strlen(input);i++){
		if(! (input[i] == '.' || isdigit(input[i])) ){
			//printf("letter?\n");
			return false;
		}
	}
	return true;
}

char* page(char* input){
	bool start = false;
	char* page = (char*)malloc(10000 * sizeof(char));
	page[0] = '/';
	int k = 0;
	int i = 0;
	for(i = 0;i < strlen(input);i++){
		if(start){
			k++;
			page[k] = input[i];
		}else if(input[i] == '/'){
			start = true;
		}
	}
	page[i] = '\0';
	return page;
}

char* host(char* input){
	char* host = (char*)malloc(10000 * sizeof(char));
	int i = 0;
	for(i = 0;i < strlen(input);i++){
		host[i] = input[i];	
		if(input[i] == '/'){
			break;
		}
	}
	host[i] = '\0';
	return host;
}



