#ifdef CHANGED
#include <syscall.h>

void putstring(char* arg) {
    PutChar('U');
    PutString(arg);
    PutChar('\n');
    //    ThreadExit();
}

int main() {
    ThreadCreate((void*) putstring, "UserThread: Holla!");

    //PutChar & PutString par le thread main
    PutChar('M');
    PutString("MainThread: Hello World!");
    PutChar('\n');

    ThreadExit();

    //le thread principale ne doit pas se terminer
    while (1);

    //Halt();

}
#endif // CHANGED
