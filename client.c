#include<stdio.h> //printf
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "127.0.0.1"  
#define BUFLEN 512  //Max length of buffer
#define PORT 5000   //The port on which to send data

struct packet
{
	char *command; //command that is being used
	short argument;
	char data[512]; //array to hold data, can hold 1 sector etc
};

struct sockaddr_in address;          //gethostbyname is obsolete if you google it you will find this so I used local host function so why it did not work
int socket_fd;





void die(char *s)
{
    perror(s);
    exit(1);
}

int fmount(char *hostname)
{
    //////////
    hostname = SERVER;
    /////////
    printf("\nConnecting to host [%s]",hostname);
    
    if ( (socket_fd = socket(AF_UNIX, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
    
    bzero((char *) &address, sizeof(address));
    address.sin_family = AF_UNIX;
    address.sin_port = htons(PORT);
    
    //translate the string IP into an IP adress data type
    if (inet_aton(hostname , &address.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        printf("\nConnection failed");
    	return 0;
    }
    /*
    //int host_fd;
    mode_t mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
    host_fd = open(hostname,O_RDONLY,mode);
    perror("error open  ");
    return fd;
	*/
    
    printf("\nConnection successful");
    return 1;
}
//detach from the server
void fumount() //int fd   
{
    close(socket_fd);
    printf("\nDisconnected from host");
}

void help()
{
    printf("\nClient Floppy Image Reader\n");
    printf("==================================\n ");
    printf("This program pulls info from a floppy mounted on a server and interprets it\n ");
    printf("==================================\n ");
    printf("\nCommands: \n\n ");
    printf("help - Displays useable commands \n ");
    printf("fmount [hostname] - Mounts the desired server \n ");
    printf("fumount [hostname] - Unmounts the desired server \n ");
    printf("showsector [sectornum] - Pulls desired sector info from server and displays it \n ");
    printf("traverse [-l] - Display the root directory contents (with the option to show more info) \n ");
    printf("structure - pulls structural info from server and displays it \n ");
    printf("quit - Quit out of the floppy shell \n\n");
}


/////////////////////////////////////////////////////////////
void structure()   //int fdif we can pass in the fd it will be so much easier
{
    unsigned char buff[512];
    //char *message = "structure";
    struct packet message;
    message.command = "structure";
    message.argument = 3;
    if (sendto(socket_fd, &message, sizeof(message) , 0 , (struct sockaddr *) &address, sizeof(address))==-1)
    {
            die("sendto()");
    }
    
    //lseek(fd,0,SEEK_SET);		this will be in the server
    //int n =read(fd,buff,512);
    
    //buff = getfromserver
    //buff = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
    
       //perror("error read  ");
    
    printf("\nflop structure\n");
    printf("\t\tnumber of Fat:\t\t\t\t%d\n",buff[16]);
    printf("\t\tnumber of sectors used by FAT:\t\t%d\n",buff[22]);
    printf("\t\tnumber of sectors per cluster:\t\t%d\n",buff[13]);
    printf("\t\tnumber of ROOT Enteries:\t\t%d\n",buff[17]|buff[18]<<8);
    printf("\t\tnumber of bytes per sector:\t\t%d\n",buff[11]|buff[12]<<8);
    
    printf("\t\t---Sector #---\t\t---Sector Types---\n");
    //int x,y,z;
    
    
}
/////////////////////////////////////////////////////////////
void traverse(char *flag)
{
}
/////////////////////////////////////////////////////////////
void showsector(int sectorNum)  //, int fdsame here to pass in the fd handler will be easier!
{
    /*
    unsigned char buff[512];
    int counter=0,i,j,z;
    unsigned int title=0x00;
    unsigned int col=0x00;
    
    //lseek(fd,sectorNum*512,SEEK_SET);
    //read(fd,buff,512);
    perror("error read");
    
    printf("flop:showsector %d\n",sectorNum);
    for(z=0; z<16; z++){   //format column
        printf("\t%x", col);
        col=col+0x01;}
    printf("\n");
    for(i=0; i<32;i++){   //format row
        printf("%x\t", title);
        title=title+0x10;
        for(j=0; j<16; j++){
            printf("%x\t", buff[counter]);
            counter++;
        }
        printf("\n");
    }
    */
    
}

int main(void)
{
    
    char *arguments[10];
    char input[100];
    
    short connected = 0;//variable so we know if we are connected to server or not
    
    printf("\nClient-Server floppy reader\n");
    printf("Enter a command:\n\n");
    while(1)
    {
        printf("\n~flop:");
        fgets(input, sizeof(input), stdin); //get message size ie data sectors from server
        
        int m = 0;
        arguments[0] = strtok(input, "\n ");
        m = 0;
	while(arguments[m] != NULL)
	{
		m++;
		arguments[m] = strtok(NULL, "\n ");
	}
        
        printf("\nargument 0 :%s",arguments[0]);
        printf("\nargument 1 :%s\n",arguments[1]);
        
        if(strcmp(arguments[0], "quit")==0)
        {
            break;
        }
        else if(strcmp(arguments[0], "help")==0)
        {
            help();
        }
        else if (strcmp(arguments[0], "fmount")==0)
        {
            if(arguments[1] == NULL)
            {
                printf("\nEnter hostname!\n");
            }
            else 
            {
                connected = fmount(arguments[1]);
            }
        }
        else if(strcmp(arguments[0], "fumount")==0)
        {
        	if(connected==0)
	        {
	            printf("\nYou must first connect to a host using fmount!\n");
	        }
	        else
	        {
	        	fumount();	
	        }
        }
        else if(strcmp(arguments[0], "showsector")==0)
        {
            if(arguments[1]==NULL)
            {
                printf("enter a number\n");
            }
            else
            {
            	if(connected==0)
	        {
	            	printf("\nYou must first connect to a host using fmount!\n");
	        }
	        else
	        {
	        	showsector(atoi(arguments[1]));	
	        }
            }
        }
        else if(strcmp(arguments[0], "structure")==0)
        {
        	if(connected==0)
	        {
	            printf("\nYou must first connect to a host using fmount!\n");
	        }
	        else
	        {
	        	structure();	
	        }
            //fd = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
            //printf("%d\n", fd);
        }
        else if(strcmp(arguments[0], "traverse")==0)
        {
        	if(connected==0)
	        {
	            printf("\nYou must first connect to a host using fmount!\n");
	        }
	        else
	        {
	        	traverse(arguments[1]);	
	        }
            
        }
        else
        {
            printf("\nPlease enter a valid command");
            printf("\nType 'help' for a list of commands\n");
        }
    }
    
    //close(s);
    return 0;
}
