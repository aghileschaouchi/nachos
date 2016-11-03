#ifdef CHANGED

#include "machine.h"
#include "addrspace.h"
#include "thread.h"
#include "system.h"

extern int do_ThreadCreate(int f, int arg);
extern void do_ThreadExit();
static void StartUserThread(void *schmurtz);
#endif //CHANGED
