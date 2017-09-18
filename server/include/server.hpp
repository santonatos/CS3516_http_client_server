#ifndef SERVER_HPP
#define SERVER_HPP

void ShowError(char *errorMessage);  /* Error handling function */ 
void* HandleTCPClient(void* clntSocket);   /* TCP client handling function */

#endif

 




