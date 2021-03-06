
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
    char currentclient[100];
    short hasclient = 0;
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
        printf("\n-----------------------");
        printf("\nHandling client %s", inet_ntoa(si_other.sin_addr));
        
        //printf("\nIncoming Length: %u", slen);
        printf("\nCommand: %s" , message->command);
        //printf("\nArgument: %u" , message->argument);
        //if(message->data!=NULL)printf("\nData: %s" , message->data);
        printf("\n");
        //now reply the client with the same data
        if(strcmp(message->command, "connect")==0){
            if(hasclient == 0)
            {
            	    strcpy(currentclient,inet_ntoa(si_other.sin_addr));
            	    printf("\nConnection made to client %s",currentclient);
            	    hasclient = 1;
            	    strcpy(message->command,"success");
            	    if (sendto(socket_fd, message, sizeof(*message), 0, (struct sockaddr*) &si_other, slen) == -1)
	            {die("\nsendto()");}
            }
            else
            {
            	    printf("Connection failed, already connected to: %s",currentclient);
            	    strcpy(message->command,"failed");
            	    if (sendto(socket_fd, message, sizeof(*message), 0, (struct sockaddr*) &si_other, slen) == -1)
	            {
	            	die("\nsendto()");
	            }
            }
        } 
        else if(strcmp(message->command, "disconnect")==0 && hasclient==1){
            if(strcmp(inet_ntoa(si_other.sin_addr), currentclient)==0)	
            {
            	printf("Disconnected from client: %s",currentclient);
            	memset(currentclient,0,strlen(currentclient));
            	hasclient = 0;
            }
            else
            {
            	printf("\nNon-connected client trying to disconnect");
            }
        } 
        else if(strcmp(message->command, "structure")==0 && hasclient==1 && strcmp(inet_ntoa(si_other.sin_addr), currentclient)==0){
            lseek(floppy_fd, 0, SEEK_SET );
            read(floppy_fd, message->data, sizeof(message->data));
            //printf("\nData: %*s",512 , flop);
            if (sendto(socket_fd, message, sizeof(*message), 0, (struct sockaddr*) &si_other, slen) == -1)
            {
            	die("\nsendto()");
            }
        }
        else if(strcmp(message->command, "showsector")==0 && hasclient==1 && strcmp(inet_ntoa(si_other.sin_addr), currentclient)==0){
            lseek(floppy_fd, message->argument*512, SEEK_SET );
            read(floppy_fd, message->data, sizeof(message->data));
            //printf("\nData: %*s",512 , flop);
            if (sendto(socket_fd, message, sizeof(*message), 0, (struct sockaddr*) &si_other, slen) == -1)
            {
            	die("\nsendto()");
            }
        }
	else if(strcmp(message->command, "traverse")==0 && hasclient==1 && strcmp(inet_ntoa(si_other.sin_addr), currentclient)==0){
	    
	    if(message->argument==0)//init, get ready for reading traverse data
	    {
	    	  char num_fat;
  		  uint16_t size_fat;
  		  
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
	else
	{
		printf("\n Request failed. Bad non-conected server or bad command");
	}
	printf("\n-----------------------");

    }
    close(floppy_fd);
    close(socket_fd);
    return 0;
}



