#include <stdbool.h>

#define NDEBUG

#define ZERO 0

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG(%s:%d) " M, __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define RANDOM 0
#define FIFO 1
#define LRU 2

extern int NUM_FRAME;
extern int NUM_PAGE;
extern int NUM_PROC;

extern int replacementPolicy;
extern char *PHYSICAL_MEM;

int page_replacement();
int MMU(int pid, int addr, char type, bool *hit);
void pt_print_stats();
void pt_init();
void swap_init();
void swap_print_stats();
