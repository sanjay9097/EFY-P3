#include <stdio.h>
#include <fcntl.h>   // File Control
#include <termios.h> // POSIX Terminal Control Definitions 
#include <unistd.h>  // UNIX Standard Definitions 	   
#include <errno.h>   // ERROR Number Definitions           
#include<string.h>
#include<signal.h>
#include<stdlib.h>

int fd = 0;
FILE *fil;
/* This isr is called when u press ctrl+c , it will close all opened file properly */
void isr(int n)
#include <stdio.h>
#include <fcntl.h>   // File Control
#include <termios.h> // POSIX Terminal Control Definitions 
#include <unistd.h>  // UNIX Standard Definitions 	   
#include <errno.h>   // ERROR Number Definitions           
#include<string.h>
#include<signal.h>
#include<stdlib.h>

int fd = 0;
FILE *fil;
/* This isr is called when u press ctrl+c , it will close all opened file properly */
void isr(int n)
{
	fclose(fil);
	close(fd);
	exit(0);
}

int main()
{
	char ch;
	char read_buffer[30];   /* Buffer to store the data received              */
	int  bytes_read = 0;    /* Number of bytes read by the read() system call */
	int i = 0;

	signal(2,isr);
	/* opening the file to save data*/
	fil=fopen("data_from_arduino.txt","w");
	if(fil==NULL)
		printf("error in opening\n");

	//Opening the Serial Port 
	fd = open("/dev/ttyACM0",O_RDWR | O_NOCTTY | O_NDELAY);	                          
	if(fd == -1)	/* Error Checking */
		printf("\n  Error! in Opening ttyUSB0i/ttyACM0  ");
	else
		printf("\n  ttyUSB0/ttyACM0 Opened Successfully ");

	// Setting the Attributes of the serial port using termios structure 
	struct termios SerialPortSettings;	// Create the structure                          
	tcgetattr(fd, &SerialPortSettings);	// Get the current attributes of the Serial port 
	// Setting the Baud rate 
	cfsetispeed(&SerialPortSettings,B9600); // Set Read  Speed as 9600                       
	SerialPortSettings.c_cflag &= ~PARENB;   //Disables the Parity Enable bit(PARENB),So No Parity
	SerialPortSettings.c_cflag &= ~CSTOPB;   // CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit 
	SerialPortSettings.c_cflag &= ~CSIZE;	 // Clears the mask for setting the data size             
	SerialPortSettings.c_cflag |=  CS8;      // Set the data bits = 8                                 
	SerialPortSettings.c_cflag &= ~CRTSCTS;       // No Hardware flow Control                         
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; // Enable receiver,Ignore Modem Control lines       
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          // Disable XON/XOFF flow control both i/p and o/p 
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Non Cannonical mode 
	SerialPortSettings.c_cc[VMIN] = 10; /* Read at least 10 characters */
	SerialPortSettings.c_cc[VTIME] =0; /* Wait indefinetly   */

	tcflush(fd, TCIFLUSH);   /* Discards old data in the rx buffer            */

	while(1)
	{
		bytes_read = read(fd,read_buffer,30); /* Read the data                   */
	        for(i=0;i<bytes_read;i++)
		{	//printing only the received characters
			ch=read_buffer[i];
			printf("%c",ch);
			fputc(ch,fil);
		}
	}
}




