#include <stdio.h>
#include <stdlib.h>
#include "vm.h"
#include "disk.h"
#include "pagetable.h"

PT *pageTable;
Invert *frame_table;
STATS stats;
int freeFrameNumber=0;

int is_page_hit(int pid, int pageNo, char type)
{
	if(pageTable[pid].entry[pageNo].valid == true)
	{
		return pageTable[pid].entry[pageNo].frameNo;
	}
	else 
	{
		return -1;
	}
}

int MMU(int pid, int addr, char type, bool *hit)
{
		int frameNo;
		int pageNo = (addr >> 8);
		int offset = addr - (pageNo << 8);
		int physicalAddr = 0;
		
		if(pageNo >= NUM_PAGE) {
				printf("invalid page number (NUM_PAGE = 0x%x): pid %d, addr %x\n", NUM_PAGE, pid, addr);
				return -1;
		}
		if(pid > (NUM_PROC-1)) {
				printf("invalid pid (valid pid = 0 - %d): pid %d, addr %x\n", NUM_PROC-1, pid, addr);
				return -1;
		}
				
		frameNo = is_page_hit(pid, pageNo, type);
		// hit
		if(frameNo >= 0) {
				stats.hitCount++;
				physicalAddr = (frameNo << 8) + offset;
				*hit = true;
				page_update(frameNo);
				debug("hit  frameNo %d, pageNo %d physicalAddr %d\n", frameNo, pageNo,physicalAddr);
				return physicalAddr;
		}
		
		stats.missCount++;
		*hit = false;
		debug("miss  pid %d, pageNo %d", pid, pageNo);
		// miss. handle 
		if(freeFrameNumber<NUM_FRAME)
		{
			
			frameNo = freeFrameNumber;
			page_update(frameNo);
			++freeFrameNumber;
			pageTable[pid].entry[pageNo].frameNo = frameNo;
			pageTable[pid].entry[pageNo].valid = true;
			pageTable[pid].entry[pageNo].dirty = false;
			frame_table[frameNo].pid = pid;
			frame_table[frameNo].page = pageNo;
			debug("free frame case  frameNo %d, pageNo %d\n", frameNo, pageNo);

		}
		// pagefault
		else{
			frameNo = page_replacement();
			page_update(frameNo);
			int oldpid = frame_table[frameNo].pid;
			int oldpageNo = frame_table[frameNo].page;
		
			if(pageTable[oldpid].entry[oldpageNo].valid == true)
			{
				debug("validity check true");
				if(pageTable[oldpid].entry[oldpageNo].dirty == true)
				{
					debug("validity check true");
					swap_write(frameNo, oldpid, oldpageNo);
				}
				pageTable[oldpid].entry[oldpageNo].valid = false;
				pageTable[oldpid].entry[oldpageNo].dirty = false;
			}

			frame_table[frameNo].pid = pid;
			frame_table[frameNo].page = pageNo;

			pageTable[pid].entry[pageNo].frameNo = frameNo;
			pageTable[pid].entry[pageNo].valid = true;
			pageTable[pid].entry[pageNo].dirty = false;
		
			swap_read(frameNo, pid, pageNo);
			debug("replacement case  frameNo %d, pageNo %d\n", frameNo, pageNo);
		}
		if(type=='W')
			pageTable[pid].entry[pageNo].dirty = true;
		
		physicalAddr = (frameNo << 8) + offset;
		debug("miss physicalAddr %d\n", physicalAddr);
		return physicalAddr;
}

void pt_print_stats()
{
		int req = stats.hitCount + stats.missCount;
		int hit = stats.hitCount;
		int miss = stats.missCount;

		printf("Request: %d\n", req);
		printf("Page Hit: %d (%.2f%%)\n", hit, (float) hit*100 / (float)req);
		printf("Page Miss: %d (%.2f%%)\n", miss, (float)miss*100 / (float)req);

}

void pt_init()
{
		int i,j;
		pageTable = (PT*) malloc(sizeof(PT) * NUM_PROC);
		frame_table = (Invert*) malloc(sizeof(Invert*) * NUM_FRAME);
		stats.hitCount = 0;
		stats.missCount = 0;

		for(i = 0; i < NUM_PROC; i++) {
				pageTable[i].entry = (PTE*) malloc(sizeof(PTE) * NUM_PAGE);
				for(j = 0; j < NUM_PAGE; j++) {
						pageTable[i].entry[j].valid = false;
						pageTable[i].entry[j].dirty = false;
				}
		}
}