#ifdef CHANGED
#include <syscall.h>

int main() {
    int n;
    PutString("Saisir un entier (signé): ");
    GetInt(&n);
    PutString("L'entier saisi est: ");
    PutInt(n);
    PutChar('\n');
    Halt();
}
#endif // CHANGED