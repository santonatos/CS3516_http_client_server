#ifndef CLIENT_HPP
#define CLIENT_HPP



void ShowError(char *errorMessage);  /* Error handling function */ 
char* page(char* input); //split the string
char* host(char* input); //
bool isIPaddr(char* input); //check if it is IP address or not

#endif
