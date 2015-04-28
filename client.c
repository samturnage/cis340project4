#include <stdio.h>
#include	<signal.h>
#include	<errno.h>
#include	<strings.h>
#include 	<stdlib.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>

//Attach to the server
int fmount(char *hostname[])
{
int fd;
mode_t mode=S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
fd=open(hostname,O_RDONLY,mode);
perror("error open  ");
return fd;
	
}
//detach from the server
void fumount(int fd)    //in main the fd closed is socket_fd
{
    close(fd);
    printf("file unmounted\n");	
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
void structure(int fd)   //if we can pass in the fd it will be so much easier
{
unsigned char buff[512];

//lseek(fd,0,SEEK_SET);		this will be in the server
//int n =read(fd,buff,512);

//buff = getfromserver

perror("error read  ");

printf("flop structure\n");
printf("\t\tnumber of Fat:\t\t\t\t%d\n",buff[16]);
printf("\t\tnumber of sectors used by FAT:\t\t%d\n",buff[22]);
printf("\t\tnumber of sectors per cluster:\t\t%d\n",buff[13]);
printf("\t\tnumber of ROOT Enteries:\t\t%d\n",buff[17]|buff[18]<<8);
printf("\t\tnumber of bytes per sector:\t\t%d\n",buff[11]|buff[12]<<8);

printf("\t\t---Sector #---\t\t---Sector Types---\n");
//int x,y,z;


}
/////////////////////////////////////////////////////////////
void traverse(short flag)
{
}
/////////////////////////////////////////////////////////////
void showsector(int sectorNum, int fd)  //same here to pass in the fd handler will be easier!
{
int counter=0,i,j,z;
unsigned int title=0x00;
unsigned int col=0x00;

lseek(fd,sectorNum*512,SEEK_SET);
read(fd,buff,512);
perror("error read");

printf("flop:showsector %d\n",n);
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

}
//////////////////////////////////////////////////////////////
void parse(char* input, char* arguments[]){
	arguments[0] = strtok(input, "\n ");
	i = 0;
	do{
		i++;
		arguments[i] = strtok(NULL, "\n ");
	}while(arguments[i] != NULL);  
}
//////////////////////////////////////////////////////////
int main( int argc, const char* argv[] )
{
	  char input[100];
	  char *arguments[5];
	  char* chrptr; 
	  
	  
	  printf( "\n--Client Started--\n\n" );
	  //client has to request one or more sectors from server---not sure how to accomplish this
	  while(1)
	  {
	  	printf("\nfloppy: ");
		chrptr = fgets(input, 100, stdin);
		if(chrptr == NULL){
			printf("\nError: could not read input\n");
		}
		parse(input, arguments);
		//the first argument[0] is the command/////////
		if(strcmp(arguments[0], "quit") == 0){
			exit(EXIT_SUCCESS);
		}
		else if(strcmp(arguments[0], "help") == 0){
			help();
		}
		else if(strcmp(arguments[0], "fmount") == 0){
			//fmount(argument[1]);
		}
		else if(strcmp(arguments[0], "fumount") == 0){
			//fumount(argument[1]);
		}
		else if(strcmp(arguments[0], "structure") == 0){
			//structure();
		}
		else if(strcmp(arguments[0], "traverse") == 0){
			//int flag = 
			//traverse(flag);
		}
		else if(strcmp(arguments[0], "showsector") == 0){
			help();
		}
		
	      //get input
	      /*
	        help
	        fmount [hostname]
	        fumount [hostname]
	        structure
	        traverse -l
	        showsector
	        quit
	      */
	  
	  }
	  
	  return 0;
}
