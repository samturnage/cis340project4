

#include        <stdio.h>
#include	<signal.h>
#include	<errno.h>
#include	<strings.h>
#include 	<stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include        <arpa/inet.h>


int main( int argc, const char* argv[] )
{
    	 // const char* floppy = argv[1];
	 //printf( "\nFloppy %s opened...\n",floppy)
    int cont, socket_fd, fd, fsize, recvlen;
    char *hostname[];
    struct sockaddr_in s_in, from;
    
    struct {
     char cmd;
     char sequence;
     short argument;
     unsigned char buffer[512];
        
    } serverMsg;
    
    
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);  //create socket
    if(socket_fd < 0){
        perror("socket not created"); //socket error
        exit(1);
    }
    bzero((char *)&s_in, sizeof(s_in));
    
    s_in.sin_family = (short)AF_INET;            //define the family, address and port ...I think we can use 5000? it was between 5000- 6000 something
    s_in.sin_addr.s_addr = hton(INADDR_ANY);
    s_in.sin_port = htons ((u_short)0x5000);
    
    if(bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in)) < 0){  //bind socket
        perror("bind failed");
        exit(1);
    }
    
    if((fd = open(hostname, O_RDONLY)) < 0){ //open the imagefile (fmount)
            perror("file open failed");
        }
    
    for(;;){
        fsize = sizeof(from);
        recvlen = recvfrom(socket_fd, &serverMsg, sizeof(serverMsg), 0, (struct sockaddr *)&from,&fsize);  //receive data from UDP datagram
        if(recvlen < 0){
            perror("recvfrom fail");
        }
       
        while((cont = read(fd, buffer, sizeof(buffer)))>0) {  // while loop reading from the sectors possibly need to manipulate this to read from individual sectors
            sleep(1);
            sendto(socket_fd, &serverMsg, sizeof(serverMsg), 0, (struct sockaddr *)&from,&fsize);  //trying to send back to client 
            
        }
        /*
        loop waiting for a message
        	get some message
        	if the message is traverse etc
        		read from open floppy
        		send back to client
        
        */	
        
    }
        //fumount
        printf("request complete");
        close(socket_fd);
	  //fumount
	  return 0;
}
