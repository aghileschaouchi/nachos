#ifdef CHANGED
#include <syscall.h>

int main() {
    int c = GetChar();
    PutChar('<');
    PutChar(c);
    PutChar('>');
    PutChar('\n');
    return 13;
}
#endif // CHANGED