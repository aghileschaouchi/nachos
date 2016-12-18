#ifdef CHANGED

#include <stddef.h>

#include "syscall.h"

int main() {
    char buf[1024];

    PutChar('X');
    PutChar('\n');
    PutString("Je suis une chaine!!!\n");
    PutString("Je suis GetString: Donnez-moi quelque chose à manger à stdin!!!\n");

    do {
        GetString(&buf, 1024);
        PutString(&buf);
    } while (buf[0] != '\0');
    PutChar('\n');

    return 99;
}
#endif // CHANGED