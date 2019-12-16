#include <stdio.h>
#include "vm.h"
#include "disk.h"
#include "list.h"

struct Node *head = NULL;
int currentFifoFrame=0;
int lru_List[257];
int lru_counter;

void init_replacement()
{
		if(replacementPolicy == FIFO)  
			currentFifoFrame = 0;
		else if(replacementPolicy == LRU) 
		{
			int i;	
			lru_counter = 0;
			for(i=0;i<NUM_FRAME;i++)
			{
				lru_List[i] = 0;
			}
		}
}
int random()
{
		int next = rand() % NUM_FRAME;
		debug("next = %d\n", next);
		return next;
}

int fifo()
{
		int next = currentFifoFrame;
		currentFifoFrame++;
		if(currentFifoFrame >= NUM_FRAME)
			currentFifoFrame = currentFifoFrame % NUM_FRAME;
		return next;
}

int lru()
{
		int next, min=-1,i;
		for(i=0;i<NUM_FRAME;i++)
		{
			if(lru_List[i] < min || min == -1)
			{
				min = lru_List[i];
				next = i;
			}
		debug("%d ", lru_List[i]);
		}
		return next;
}

int page_replacement()
{
		int frame;
		if(replacementPolicy == RANDOM)  frame = random(); 
		else if(replacementPolicy == FIFO)  frame = fifo();
		else if(replacementPolicy == LRU) frame = lru();
		return frame;
}
void page_update(int frame)
{
	if(replacementPolicy == LRU) 
	{
		lru_counter++;
		lru_List[frame]=lru_counter;
	}
	debug("update%d\n", lru_List[frame]);	
}