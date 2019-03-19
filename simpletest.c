#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
//#include </lib/modules/simple_char_driver.c>

#define DEVICE "/dev/simple_char_divice"
#define BUFFER_SIZE 1024

int main()
{
	char command, buffer[BUFFER_SIZE], temp[BUFFER_SIZE];
	int length, whence, newOffset;
	int file = open(DEVICE, O_RDWR);
	bool loopValue = true;
	while(loopValue)
	{
		printf("\nr) Press r to read from device\nw) Press w to write to the device\ns) Press s to seek to the device\ne) Press e to exit the device\n");
		scanf("%c", &command);
		switch (command)
		{
			case 'w':
				printf("Enter the data you want to write to the device: ");
				scanf("%s", buffer);
				write(file, buffer, BUFFER_SIZE);
				while(getchar() != '\n');
				break;
			case 'r':
				printf("How many bytes to read: ");
				scanf("%s", temp);
				read(file, buffer, length);
				printf("Data read from the device: %s\n", buffer);
				while(getchar() != '\n');
				break;
			case 's':
				printf("Enter the seek Command;\n '0' seek set\n '1' seek cur\n '2' seek end\n Enter whence: ");
				scanf("%d", &whence);
				printf("Enter an offset: ");
				scanf("%d", &newOffset);
				llseek(file, newOffset, whence);
				break;
			case 'e':
				printf("Exiting...\n");
				loopValue = false;
				break;
			default:
				printf("ERROR- not a value command\n");
				break;
		}
	}
	close(file);
	return 0;
} 
