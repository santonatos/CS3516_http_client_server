README FILE:

author: spiros antonatos

HOW TO COMPILE(MAKE):
--------------------
 
>type 'make' while in the directory of the main folder (not inside client or server directories)
->that will procude 2 executables: http_client in the client folder, and http_server in the server folder


>when you want to clean the directories type 'make clean' while in the main directory (or make clean in the seperate ones)
>when you want to clean the client directory from the response txt files (remove the txt files) type 'make clean-txt' in the main directory- or the client directory



HOW TO RUN:
------------

>after you compile, run './http_server 55551' -in the server directory- that will get the demo server up; you can also pick any other port of your choosing. Then you can run a http request with the http_request program like that:

-> ./http_client -p localhost/TMDG.html 55551      (not index.html) or
-> ./http_client -p localhost 55551

Only the headers are printed on the console, the whole response prints on a txt file

For arbirary sites you can run (usually on port 80):

-> ./http_client -p www.cnn.com 80	or
-> ./http_client www.msn.com 80		or
-> ./http_client -p www.msn.com/el-gr/ 80




