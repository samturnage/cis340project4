
%%README Assignment 4 CIS 340



Turnage, Samuel	    s.turnage@vikes.csuohio.edu      
Aring, Danielle	    D.ARING@vikes.csuohio.edu    
 
_________________________
Percentage Contribution
_________________________
Turnage, Samuel 50%
Aring, Danielle 50%
_________________________
   Project Description
_________________________


Develop a single threaded UDP Floppy disk server daemon that allows the user to access a remotely mounted floppy disk. The client creates a shell 

environment that executes commands, the server provides the client with the raw data from the floppy image to perform said commands.

_________________________
   Included Files
_________________________

makefile
client.c
server.c
_________________________
        Design 
_________________________

The server.c file creates a socket that utilizes the SOCK_DGRAM, the socket is bound, and the floppy imagefile.img is permanently opened on the server 

side. The socket recieves data from the client we used the following stuct to create the packet that is sent and recieved from both the client and server 

side : 

      struct Packet 
 { 
 	short argument;  //extra arg of "-l" or "10"
 	unsigned char data[512];//array to hold data, can hold 1 sector etc 
 	char command[32]; //command that is being used 
 	  
 }; 
  
while the socket on the server side is listening for the data, we created a bunch of strcmp statements for each individual command the client would send 

to the server, and then used the linux system calls lseek() and read() to scan to the appropriate area on the floppy_fd to send the client the right data 

sectors. We used a strcmp for connect and disconnect so that when the client requests for data or a disconnection via the commands and the fmount and 

fumount, the server only handles the client for as long as it is connected to the host, and checks to make sure that the clients handle is consistent with 

its port number and inet address. 

client.c file creates the necessary functions to run structure, traverse, traverse -l, help and showsector. The same data struct above is declared on the 

client side as well as it recieves this packet from the server. The fmount function passes in the hostname, creates the hostent structure, and uses the 

gethostbyname() function to truly mount the IP server address. Fumount disconnects the server.  For the showsector and traverse commands we had to pass in 

the argument from the above struct to allow for the function to be executed. In our main we created a shell that checks the array for the command that is 

entered into stdin and performs the appropriate action. For every command entered the program checks if the user has mounted the host. If not the action 

is not performed. If the user enters a host that is invalid the program will not execute.
_________________________
 Compiling Instructions
_________________________

to compile: 

make 

to execute: 

for client : ./client
for server : ./server

_________________________
    Sample Test Run
_________________________
client :                                                                                             
arthur:~% cd /home/student/daaring/proj4                                                            
arthur:~/proj4% make
gcc  -c -o server.o server.c -Wall -g -I.
gcc  -o server server.o -Wall -g -I.
gcc  -c -o client.o client.c -Wall -g -I.
gcc  -o client client.o -Wall -g -I.
arthur:~/proj4% ./client

Client-Server floppy reader
===========================
NOTE: when using fmount, enter any hostname, this needs to be fixed later
Enter a command:
~flop:fmount 127.0.0.1
Connection successful
Floppy ready to be used
~flop:help

Client Floppy Image Reader
==================================
 This program pulls info from a floppy mounted on a server and interprets it
 ==================================
 
Commands: 

 help - Displays useable commands 
 fmount [hostname] - Mounts the desired server 
 fumount [hostname] - Unmounts the desired server 
 showsector [sectornum] - Pulls desired sector info from server and displays it 
 traverse [-l] - Display the root directory contents (with the option to show more info) 
 structure - pulls structural info from server and displays it 
 quit - Quit out of the floppy shell 


~flop:structure

flop structure
                number of Fat:                          2
                number of sectors used by FAT:          9
                number of sectors per cluster:          1
                number of ROOT Enteries:                224
                number of bytes per sector:             512
                ----------------        ------------------
                        0                       BOOT
                        1--9                    FAT1
                        10--18                  FAT2
                        19--32                  ROOT DIRECTORY

~flop:showsector 10

Sector : 10
           0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
   0      f0  ff  ff   0  40   0  ff  6f   0  ff  ff  ff   9  f0  ff  ff
  10      cf   0  ff  ff  ff  ff   f   1  11  f0  ff  ff  ff  ff  15  f0
  20      ff  17  f0  ff  ff  ff  ff  ff  ff  ff  1d  f0  ff  1f   0   2
  30      ff  ff  ff   0   0   0   0   0   0   0   0   0   0   0   0   0
  40       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  50       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  60       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  70       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  80       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  90       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  a0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  b0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  c0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  d0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  e0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
  f0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 100       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 110       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 120       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 130       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 140       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 150       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 160       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 170       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 180       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 190       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 1a0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 1b0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 1c0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 1d0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 1e0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
 1f0       0   0   0   0   0   0   0   0   0   0   0   0   0   0   0   0

~flop:traverse

roots:224
/A.CLA         
/ALOOP1.CLA    
/ALOOP1.JAV    
/ALOOP2.CLA    
/ALOOP2.JAV    
/ALOOP3.CLA    
/ALOOP3.JAV    
/B.CLA         
/CONDIT.CLA    
/CONDIT.JAV    
/D.CLA         
/DEMO.CLA      
/DEMO.JAV      
/DIR1                           <DIR>
/POLYTEST.CLA  
/POLYTEST.JAV  
/SOME.CLA      

~flop:traverse -l
        *****************************
        ** FILE ATTRIBUTE NOTATION **
        **                         **
        ** R ------ READ ONLY FILE **
        ** S ------ SYSTEM FILE    **
        ** H ------ HIDDEN FILE    **
        ** A ------ ARCHIVE FILE   **
        *****************************

roots:224
-A--    10/27/2014 13:13:10            670      /A.CLA
-A--    10/27/2014 13:13:10            763      /ALOOP1.CLA
-A--    10/27/2014 13:13:10            333      /ALOOP1.JAV
-A--    10/27/2014 13:13:10            786      /ALOOP2.CLA
-A--    10/27/2014 13:13:10            404      /ALOOP2.JAV
-A--    10/27/2014 13:13:10            786      /ALOOP3.CLA
-A--    10/27/2014 13:13:10            408      /ALOOP3.JAV
-A--    10/27/2014 13:13:10            358      /B.CLA
-A--    10/27/2014 13:13:10           1067      /CONDIT.CLA
-A--    10/27/2014 13:13:10            444      /CONDIT.JAV
-A--    10/27/2014 13:13:10            508      /D.CLA
-A--    10/27/2014 13:13:10            967      /DEMO.CLA
-A--    10/27/2014 13:13:10            648      /DEMO.JAV
----    10/27/2014 13:13:10     <DIR>           /DIR1
-A--    10/27/2014 13:13:10            630      /POLYTEST.CLA
-A--    10/27/2014 13:13:10           1063      /POLYTEST.JAV
-A--    10/27/2014 13:13:10            226      /SOME.CLA


~flop:fumount

Disconnected from host

arthur:~/proj4% ./client

Client-Server floppy reader
===========================
NOTE: when using fmount, enter any hostname, this needs to be fixed later
Enter a command:
~flop:fmount 127.0.0.1
Connection successful
Floppy ready to be used
~flop:quit

Disconnected from hostarthur:~/proj4% 



server:

arthur:~% cd /home/student/daaring/proj4
arthur:~/proj4% ./server

Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: connect

Connection made to client 127.0.0.1
-----------------------
Waiting for data...help^H^H^H
-----------------------
Handling client 127.0.0.1
Command: structure

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: showsector

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: traverse

-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: disconnect
Disconnected from client: 127.0.0.1
-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: connect

Connection made to client 127.0.0.1
-----------------------
Waiting for data...
-----------------------
Handling client 127.0.0.1
Command: disconnect
Disconnected from client: 127.0.0.1
-----------------------
Waiting for data...

_________________________
    Unfinished Work 
_________________________

none.



