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

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_me, si_other;
    
    int s, i, n, fd, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];
    char *hostname;
    
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
    
    // zero out the structure
    bzero((char *) &si_me,sizeof(si_me));
    
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
    //if((fd = open("imagefile.img", O_RDONLY))<0){
        //perror("no such file");
    //}
    
    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
        
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
        if(strcmp(buf, "fmount")==0){
            
            fd = open(hostname, O_RDONLY);
        }
        if(strcmp(buf, "structure")==0){
            lseek(fd, 0, SEEK_SET );
            while((n = read(fd, buf, BUFLEN)) > 0){
                sendto(s, buf, recv_len, 0, (struct sockaddr*)&si_other, slen);
                die("sendto()");
            }
        }

    
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
        
        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }
    
    close(s);
    return 0;
}
