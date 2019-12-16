#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vm.h"
#include "disk.h"

int replacementPolicy;
int NUM_FRAME;
int NUM_PAGE;
int NUM_PROC;
char *PHYSICAL_MEM;

int READ_PHYSICAL_MEM(int addr, char *byte)
{
		int frameNo = (addr >> 8);
		if(frameNo < 0 || frameNo >= NUM_FRAME) return -1;

		*byte = PHYSICAL_MEM[frameNo];
		debug("read physical mem frameNo %d char %c\n", frameNo,PHYSICAL_MEM[frameNo]);
		
		return 1;
}

int WRITE_PHYSICAL_MEM(int addr, char byte)
{
		int frameNo = (addr >> 8);
		if(frameNo < 0 || frameNo >= NUM_FRAME) return -1;
		PHYSICAL_MEM[frameNo] = byte;
		debug("write physical mem frameNo %d char %c\n", frameNo,PHYSICAL_MEM[frameNo]);
		return 1;
}

int main(int argc, char **argv)
{
		char input[256];
		int pid, addr, ret;
		char type, byte;
		bool hit;
		
		//srand(time(NULL));
		srand(100);

		if(argc < 5) {
				fprintf(stderr, "usage: ./vm [# PAGE] [# FRAME] [# PROCESS] [REPLACEMENT_POLICY]\n");
				fprintf(stderr, " # PAGE, # FRAME, # PROCESS: 1 ~ 256\n");
				fprintf(stderr," REPLACEMENT_POLICY: 0 - RANDOM, 1 - FIFO, 2 - LRU\n");
				return -1;
		}

		NUM_PAGE = atoi(argv[1]);
		NUM_FRAME = atoi(argv[2]);
		NUM_PROC = atoi(argv[3]);
		replacementPolicy = atoi(argv[4]);

		printf("# PAGE: %d, # FRAME: %d, # PROC: %d, Replacement Policy: %d - ", NUM_PAGE, NUM_FRAME, NUM_PROC, replacementPolicy);
		if(replacementPolicy == RANDOM) printf("RANDOM\n");
		else if(replacementPolicy == FIFO) printf("FIFO\n");
		else if(replacementPolicy == LRU) printf("LRU\n");
		else {
				printf("UNKNOWN!\n");
				return -1;
		}

		if(NUM_PAGE < 1 || NUM_PAGE > 256 || NUM_FRAME < 1 || NUM_FRAME > 256 || NUM_PROC < 1 || NUM_PROC > 256) {
				fprintf(stderr, "ERROR:  # PAGE, # FRAME, and # PROCESS should be between 1 and 256\n");
				return -1;
		}
				
		PHYSICAL_MEM = (char*) malloc(sizeof(char) * NUM_FRAME);
		pt_init();
		swap_init();
		init_replacement();
		while(fgets(input, 256, stdin))
		{
				if(input[0] == '#') continue;
				ret =sscanf(input, "%d %c 0x%x %c", &pid, &type, &addr, &byte);
				if((type == 'R' && ret < 3) || (type == 'W' && ret < 4))
				{
						printf("Error: invalid format\n");
						return 0;
				}
				int physicalAddr = MMU(pid, addr, type, &hit);
				if(physicalAddr < 0) return 0;

				if(type == 'R') {
						ret = READ_PHYSICAL_MEM(physicalAddr, &byte);
						if(ret < 0) printf("[pid %d, %c] 0x%x --> 0x%x: Memory Read Failed.\n", pid, type, addr, physicalAddr);
						else {
								printf("[pid %d, %c] 0x%x --> 0x%x: %c", pid, type, addr, physicalAddr, byte);
								if(hit) printf(" [hit]\n");
								else printf(" [miss]\n");
						}
				}	else if(type == 'W')	{
						ret = WRITE_PHYSICAL_MEM(physicalAddr, byte);
						if(ret < 0) printf("[pid %d, %c] 0x%x --> 0x%x: %c Memory Write Failed.\n", pid, type, addr, physicalAddr, byte);
						else { 
								printf("[pid %d, %c] 0x%x --> 0x%x: %c", pid, type, addr, physicalAddr, byte);
								if(hit) printf(" [hit]\n");
								else printf(" [miss]\n");
						}

				} else {
						printf("Error: invalid request type: %c\n", type);
						return 0;
				}
		}
		
		printf("=====================================\n");
		pt_print_stats();
		swap_print_stats();
}

