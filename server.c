
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <netinet/in.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<fcntl.h>


#define BUFLEN 512  //Max length of buffer
#define PORT 5000   //The port on which to listen for incoming data


struct Packet
{
	short argument;
	unsigned char data[512];//array to hold data, can hold 1 sector etc
	char command[32]; //command that is being used
	 
};

void die(char *s)
{
    perror(s);
    exit(1);
}

int main()
{
    struct sockaddr_in si_me, si_other;
    int socket_fd, floppy_fd;
    unsigned int slen;
    struct Packet *message = malloc(sizeof(struct Packet));
    
    //create a UDP socket
    if ((socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        die("socket");
    }
    // zero out the structure
    memset(&si_me,0,sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    //bind socket to port
    if( bind(socket_fd ,(struct sockaddr *) &si_me, sizeof(si_me) ) < 0)
    {
        die("bind failed");
    }
    if((floppy_fd = open("./imagefile.img", O_RDONLY))<0){
        perror("no such file");
    }
    
    //keep listening for data
    while(1)
    {
        printf("\nWaiting for data...");
        fflush(stdout);
        slen = sizeof(si_other);
        //try to receive some data, this is a blocking call
        if ((recvfrom(socket_fd, message, sizeof(*message), 0,(struct sockaddr *)  &si_other, &slen)) < 0)
        {
            die("\nrecvfrom() failed");
        }
        //print details of the client/peer and the data received
        printf("\nHandling client %s", inet_ntoa(si_other.sin_addr));
        printf("\nIncoming Length: %u", slen);
        printf("\nCommand: %s" , message->command);
        printf("\nArgument: %u" , message->argument);
        printf("\nData: %s" , message->data);
        //now reply the client with the same data
         
        if(strcmp(message->command, "structure")==0){
            lseek(floppy_fd, 0, SEEK_SET );
            read(floppy_fd, message->data, sizeof(message->data));
            //printf("\nData: %*s",512 , flop);
            if (sendto(socket_fd, message, sizeof(*message), 0, (struct sockaddr*) &si_other, slen) == -1)
            {
            	die("\nsendto()");
            }
        }
        else if(strcmp(message->command, "showsector")==0){
            lseek(floppy_fd, message->argument*512, SEEK_SET );
            read(floppy_fd, message->data, sizeof(message->data));
            //printf("\nData: %*s",512 , flop);
            if (sendto(socket_fd, message, sizeof(*message), 0, (struct sockaddr*) &si_other, slen) == -1)
            {
            	die("\nsendto()");
            }
        }
	else if(strcmp(message->command, "traverse")==0){
	    
	    if(message->argument==0)//init, get ready for reading traverse data
	    {
	    	  char num_fat;
  		  uint16_t size_fat;
  		  uint16_t rootdirs;
  		  
	    	  lseek(floppy_fd, 16, SEEK_SET);
		  read(floppy_fd, &num_fat, 1);
		  read(floppy_fd, message->data, 2);
		  lseek(floppy_fd, 22, SEEK_SET);
		  read(floppy_fd, &size_fat, 2);
		  /* seek to the beginning of root directory */
		  lseek(floppy_fd, (1 + num_fat * size_fat) * 512, SEEK_SET);
		  if (sendto(socket_fd, message, sizeof(*message), 0, (struct sockaddr*) &si_other, slen) == -1)
	          {
	            	die("\nsendto()");
	          }
	    }
	    else if(message->argument>0)//return data for directories
	    {
	    	read(floppy_fd, message->data, (int)message->argument);
	    	if (sendto(socket_fd, message, sizeof(*message), 0, (struct sockaddr*) &si_other, slen) == -1)
	        {    	die("\nsendto()");}
	    }
	}
        
        
        
    }
    close(floppy_fd);
    close(socket_fd);
    return 0;
}



