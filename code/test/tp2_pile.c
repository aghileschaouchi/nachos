#ifdef CHANGED
#include <syscall.h>

int factoriel(int i) {
    return i == 0 ? 1 : i * factoriel(i - 1);
}

void print(int i) {
    int result = factoriel(i);
    PutInt(result);
    PutChar('\n');
    
    ThreadExit();
}

int main() {
    int ret;
    //tester la pile du thread en appelant la fonction factoriel récursive
    if ((ret = ThreadCreate((void*) print, 9)) == -1) //9!
        return 1;

    if ((ret = ThreadCreate((void*) print, 6)) == -1) //6!
        return 1;
    
    ThreadExit();
    
    //le thread principale ne doit pas se terminer
    while (1);
}
#endif // CHANGED
