
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

/* the properties of a root directory entry */
#define	SLOT_EMPTY	0x00
#define	SLOT_DELETED	0xe5
#define	ATTR_VOLUME  	0x08		
#define	ATTR_DIRECTORY	0x10
#define	ATTR_READONLY	0x01		
#define	ATTR_HIDDEN	0x02		
#define	ATTR_SYSTEM	0x04		
#define	ATTR_ARCHIVE	0x20

/* define a directory entry in the Root directory */
struct direntry {
  char name[8];
  char ext[3];
  char attribute;
  char resv[10];	
  uint8_t timeUpdated[2];  /* updated time */
  uint8_t dateUpdated[2];  /* updated date */
  char startCluster[2];
  uint8_t size[4];
};
/*data packets sent between server and client */
struct Packet
{
	short argument;
	unsigned char data[512];
	char command[32]; //command that is being used
	 //array to hold data, can hold 1 sector etc
};

//addresses for network communication
struct sockaddr_in address;    
int socket_fd;
unsigned int addrlen;

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
    /*
    struct hostent *entry;
    entry = gethostbyname(hostname);
    if(!entry)
    {
    	die("could not find hostname");
    }
    printf("\nIP address: %s",inet_ntoa((struct inaddr **)entry->h_addr_list));
    */
    
    printf("\nConnecting to host [%s]",hostname);
    
    if ( (socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
    
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    
    //translate the string IP into an IP adress data type
    if (inet_aton(hostname , &address.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        printf("\nConnection failed");
    	return 0;
    }
    addrlen = sizeof(address);
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
    //unsigned char buff[512];
    //char *message = "structure";
    
    struct Packet *message = malloc(sizeof(struct Packet));
    strcat(message->command, "structure");
    if (sendto(socket_fd, (struct Packet*)message, sizeof(*message) , 0 , (struct sockaddr *)&address, sizeof(address))==-1)
    {
            die("Error sending to server");
    }
    if ((recvfrom(socket_fd, (struct Packet*)message, sizeof(*message), 0,(struct sockaddr *) &address,&addrlen )) < 0)
    {
            die("\nrecvfrom() failed");
    }
    
    
    printf("\nflop structure\n");
    printf("\t\tnumber of Fat:\t\t\t\t%d\n",message->data[16]);
    printf("\t\tnumber of sectors used by FAT:\t\t%d\n",message->data[22]);
    printf("\t\tnumber of sectors per cluster:\t\t%d\n",message->data[13]);
    printf("\t\tnumber of ROOT Enteries:\t\t%d\n",message->data[17]|message->data[18]<<8);
    printf("\t\tnumber of bytes per sector:\t\t%d\n",message->data[11]|message->data[12]<<8);
    
    int x,z;
    x=(message->data[22] * message->data[16]);
    z=x+(((message->data[17]| message->data[18] << 8) * 32)/ (message->data[11] | message->data[12]<<8));
    printf("\t\t----------------	------------------\n");
    printf("\t\t\t0\t\t\tBOOT\n");
    printf("\t\t\t1--%d\t\t\tFAT1\n",message->data[22]);
    printf("\t\t\t%d--%d\t\t\tFAT2\n", message->data[22] + 1,x);
    printf("\t\t\t%d--%d\t\t\tROOT DIRECTORY\n", x + 1, z);
    
}
/////////////////////////////////////////////////////////////
static void displayEntry(struct direntry *de, int flag) {
  /* construct the full name for the file or directory */
  char fullName[14];
  int i, k;

  memset(fullName, 0, sizeof(fullName));

  fullName[0] = '/';
  k = 1;
  for (i = 0; i < 8; i++) {
    if (de->name[i] != ' ') {
      fullName[k++] = de->name[i];
    } else {
      break;
    }
  }
  for (i = 0; i < 3; i++) {
    if (de->ext[i] != ' ') {
      if (i == 0) {
        fullName[k++] = '.';
      }
      fullName[k++] = de->ext[i];
    } else {
      break;
    }
  }

  if (flag) { /* detailed format */
    
    char attr[5];
    int hour, minute, second;
    int year, month, day;
    uint16_t temp;
    uint32_t size;

    /* construct the attribute column */
    attr[0] = (de->attribute & ATTR_READONLY) ? 'R' : '-';
    attr[1] = (de->attribute & ATTR_ARCHIVE)  ? 'A' : '-';
    attr[2] = (de->attribute & ATTR_SYSTEM)   ? 'S' : '-';
    attr[3] = (de->attribute & ATTR_HIDDEN)   ? 'H' : '-';
    attr[4] = '\0';

    /* construct the time part */
    temp = (de->timeUpdated[1] << 8) | de->timeUpdated[0];
    second = ((temp) & 0x1f) * 2;
    minute = (temp >> 5) & 0x3f;
    hour = (temp >> 11) & 0x1f;

    /* construct the date part */
    temp = (de->dateUpdated[1] << 8) | de->dateUpdated[0];
    day = (temp) & 0x1f;
    month = (temp >> 5) & 0xf;
    year = ((temp >> 9) & 0x7f) + 1980;

    /* construct the file size */
    size = (de->size[3] << 24) | (de->size[2] << 16) |
           (de->size[1] << 8)  | (de->size[0]);

    if ((de->attribute & ATTR_DIRECTORY) != 0) {
      printf("%s\t%02d/%02d/%04d %02d:%02d:%02d\t<DIR>\t\t%s\n", 
        attr, month, day, year, hour, minute, second, fullName);
    } else {
      printf("%s\t%02d/%02d/%04d %02d:%02d:%02d\t%10d\t%s\n", 
        attr, month, day, year, hour, minute, second, size, fullName);
    }
  } else { /* short format */
    printf("%-15s", fullName);
    if ((de->attribute & ATTR_DIRECTORY) != 0) { 
      /* this is a directory */
      printf("\t\t\t<DIR>");
    }
    printf("\n");
  }

}
/////////////////////////////////////////////////////////////
void traverse(char *argument)
{
  int flag = 0;
  if(argument != NULL && strcmp(argument, "-l")==0)flag = 1;//long output
  struct Packet *message = malloc(sizeof(struct Packet));
  uint16_t num_root_dir;
  int i;
  struct direntry de;

  if (flag) { /* detailed format */
    /* print the table header */
    printf("\t*****************************\n");
    printf("\t** FILE ATTRIBUTE NOTATION **\n");
    printf("\t**                         **\n");
    printf("\t** R ------ READ ONLY FILE **\n");
    printf("\t** S ------ SYSTEM FILE    **\n");
    printf("\t** H ------ HIDDEN FILE    **\n");
    printf("\t** A ------ ARCHIVE FILE   **\n");
    printf("\t*****************************\n");
  }

  
  strcat(message->command, "traverse");
  message->argument = 0;//initialization argument
  if (sendto(socket_fd, (struct Packet*)message, sizeof(*message) , 0 , (struct sockaddr *)&address, sizeof(address))==-1)
  {die("Error sending to server");}
  //get info back from client
  if ((recvfrom(socket_fd, (struct Packet*)message, sizeof(*message), 0,(struct sockaddr *) &address,&addrlen )) < 0)
  {die("\nrecvfrom() failed");}
  int f;
  /*
  for (f=0; f < sizeof(uint16_t); f++)
        num_root_dir = (num_root_dir << 8) + ((unsigned char) message->data[f]);
        */
  num_root_dir = message->data[0]|message->data[1]<<8;
  //num_root_dir = (uint16_t)(
  //         (message->data[0]) << 8 | 
  //         (message->data[1])
  //);
  printf("\nroots:%h\n",num_root_dir);
  
  // now start to read the directory entries 
  /*
  for (i = 0; i < num_root_dir; i++) {
    memset(&de, 0, sizeof(de));
    memset(message->data, 0, sizeof(message->data));
    message->argument = sizeof(de);
    if (sendto(socket_fd, (struct Packet*)message, sizeof(*message) , 0 , (struct sockaddr *)&address, sizeof(address))==-1)
    {die("Error sending to server");}
    //get info back from client
    if ((recvfrom(socket_fd, (struct Packet*)message, sizeof(*message), 0,(struct sockaddr *) &address,&addrlen )) < 0)
    {die("\nrecvfrom() failed");}
    
    
    
    strncpy(de.name,message->data[0],sizeof(de.name));
    strncpy(de.ext,message->data[8],sizeof(de.ext));
    de.attribute = message->data[11];
    strncpy(de.resv,message->data[12],sizeof(de.resv));
    
    
    
    if (de.name[0] == SLOT_EMPTY) {
      // this slot has not been used, means the end of the directory 
      break;
    }
    if ((uint8_t)de.name[0] == SLOT_DELETED) {
      // this is a deleted slot
      continue;
    }
    if ((de.attribute & ATTR_VOLUME) != 0) {
      continue;
    } else { // otherwise print the entry 
      displayEntry(&de, flag);
    }
  }
  */
}

/////////////////////////////////////////////////////////////
void showsector(short sectorNum)
{
    struct Packet *message = malloc(sizeof(struct Packet));
    strcat(message->command, "showsector");
    message->argument = sectorNum;
    if (sendto(socket_fd, (struct Packet*)message, sizeof(*message) , 0 , (struct sockaddr *)&address, sizeof(address))==-1)
    {
            die("Error sending to server");
    }
    if ((recvfrom(socket_fd, (struct Packet*)message, sizeof(*message), 0,(struct sockaddr *) &address,&addrlen )) < 0)
    {
            die("\nrecvfrom() failed");
    }
    
    int counter=0,i,j,z;
    unsigned int title=0x00;
    unsigned int col=0x00;
    
    
    printf("\nSector : %u\n",sectorNum);
    for(z=0; z<16; z++){   //format column
        printf("%4x", col);
        col=col+0x01;}
    printf("\n");
    for(i=0; i<32;i++){   //format row
        printf("%4x\t", title);
        title=title+0x10;
        for(j=0; j<16; j++){
            printf("%4x", message->data[counter]);
            counter++;
        }
        printf("\n");
    }
}
/////////////////////////////////////////////////////////////
int main(void)
{
    
    char *arguments[10];
    char input[100];
    
    short connected = 0;//variable so we know if we are connected to server or not
    
    printf("\nClient-Server floppy reader");
    printf("\n===========================");
    printf("\nNOTE: when using fmount, enter any hostname, this needs to be fixed later");
    printf("\nEnter a command:");
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
	        	connected = 0;
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
    
    return 0;
}
