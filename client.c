#include <stdio.h>


void fmount(char hostname[])
{
}

void fumount(char hostname[])
{
}

void help()
{
      printf(\nClient Floppy Image Reader\n);
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

void structure()
{
}

void traverse(short flag)
{
}

void showsector(int sectorNum)
{

}

int main( int argc, const char* argv[] )
{
	  printf( "\n--Client Started--\n\n" );
	  
	  while(0)
	  {
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
