typedef struct {
		int pid;
		int page;
} Invert;

typedef struct {
		int frameNo;
		bool valid;
		bool dirty;
} PTE;

typedef struct {
		int hitCount;
		int missCount;
} STATS;

typedef struct {
		PTE *entry;  //PTE entry[NUM_PAGE];
} PT;


