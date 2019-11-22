#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

int numDiskRead = 0;
int numDiskWrite = 0;

char **SWAP_DISK;

void swap_init()
{
		int i;
		SWAP_DISK = (char**) malloc(sizeof(char*) * NUM_PROC);
		if(SWAP_DISK == NULL) {
				fprintf(stderr, "swap disk init failed. Please execute the program again.\n");
				exit(0);
		}

		for(i = 0; i < NUM_PROC; i++)
		{
				SWAP_DISK[i] = (char*) malloc(sizeof(char) * NUM_PAGE);
		}
}

void swap_read(int frame, int pid, int page)
{
		numDiskRead++;
		PHYSICAL_MEM[frame] = SWAP_DISK[pid][page];
		debug("swap out (disk -> mem): pid %d, page %d, frame %d, byte %c\n", pid, page, frame, SWAP_DISK[pid][page]);
}

void swap_write(int frame, int pid, int page)
{
		numDiskWrite++;
		SWAP_DISK[pid][page]= PHYSICAL_MEM[frame];
		debug("swap in (mem -> disk): pid %d, page %d, frame %d, byte %c\n", pid, page, frame, PHYSICAL_MEM[frame]);
}

void swap_print_stats()
{
		printf("Swap Disk read: %d\n", numDiskRead);
		printf("Swap Disk write: %d\n", numDiskWrite);
}