#ifdef CHANGED
#include <syscall.h>

void putchar(char ch) {
    int i;
    for (i = 0; i < 5; i++) {
        PutChar(ch);
    }
    PutChar(i + '0');
    PutChar('\n');
}

int main() {
    char c;
    //multiThreading: chaque thread a sa propre pile
    for (c = 'a'; c <= 'z'; c++)
        ThreadCreate((void*) putchar, (void*) c);

    //terminer le thread main
    ThreadExit();
    
    ///le thread principal ne doit pas se terminer
    while (1);
}
#endif // CHANGED
