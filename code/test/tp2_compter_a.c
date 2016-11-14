#ifdef CHANGED
#include <syscall.h>

void putchar(int nb) {
    volatile int i = 1;
    for (i; i <= nb; i++) {
        PutChar('a');
    }
    PutChar(i - 1 + '0');
    PutChar('\n');
    ThreadExit();
}

int main() {
    //multiThreading: chaque thread a sa propre pile
    ThreadCreate((void*) putchar, (void*) 5);
    ThreadCreate((void*) putchar, (void*) 4);
    ThreadCreate((void*) putchar, (void*) 3);

    ThreadExit();
    
    ///le thread principal ne doit pas se terminer
    while (1);


}
#endif // CHANGED
