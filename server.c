#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define BUFLEN 512  //Max length of buffer
#define PORT 5000   //The port on which to listen for incoming data

struct packet
{
	char *command; //command that is being used
	short argument;
	char data[512]; //array to hold data, can hold 1 sector etc
};


void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_un si_me, si_other;
    
    int socket_fd, floppy_fd;
    //int slen = sizeof(si_other) , recv_len;
    //char buf[BUFLEN];
    //char *hostname;
    
    //create a UDP socket
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
    
    // zero out the structure
    bzero((char *) &si_me,sizeof(si_me));
    
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //bind socket to port
    if( bind(socket_fd , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
    //if((fd = open("imagefile.img", O_RDONLY))<0){
        //perror("no such file");
    //}
    
    //keep listening for data
    while(1)
    {
        printf("\nWaiting for data...");
        fflush(stdout);
        
        struct packet message;
        //try to receive some data, this is a blocking call
        if ((recvfrom(socket_fd, message, sizeof(struct packet), 0, (struct sockaddr*) &si_other, sizeof(si_other))) == -1)
        {
            die("\nrecvfrom()");
        }
        /*
        if(strcmp(buf, "fmount")==0){
            
            floppy_fd = open(hostname, O_RDONLY);
        }
        if(strcmp(buf, "structure")==0){
            lseek(fd, 0, SEEK_SET );
            while((n = read(fd, buf, BUFLEN)) > 0){
                sendto(s, buf, recv_len, 0, (struct sockaddr*)&si_other, slen);
                die("sendto()");
            }
        }
        */
    
        //print details of the client/peer and the data received
        printf("\nReceived packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Command: %s\n" , message.command);
        printf("Argument: %u\n" , message.argument);
        printf("Data: %s\n" , message.data);
        //now reply the client with the same data
        /*
        if (sendto(socket_fd, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("\nsendto()");
        }
        */
    }
    
    close(socket_fd);
    return 0;
}
