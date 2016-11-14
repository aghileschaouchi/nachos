#ifdef CHANGED
#include <syscall.h>

void print(char c) {
    int i;

    for (i = 0; i < 3; i++) {
        PutChar(c + i);
    }
    PutChar('\n');

    ThreadExit();
}

int main() {
    int ret;
    //appel de ThreadCreate avec la fonction print qui fait des PutChar
    if ((ret = ThreadCreate((void*) print, 'A')) == -1)
        return 1; //renvoie 1 si c'est un échec

    //le thread principale ne doit pas se terminer
    while (1);
}
#endif // CHANGED
