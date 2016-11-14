#ifdef CHANGED
#include <syscall.h>

int main() {
    int n1 = 1234567;
    int n2 = -85698;

    PutString("n1 = ");
    PutInt(n1);
    PutChar('\n');

    PutString("n2 = ");
    PutInt(n2);
    PutChar('\n');

    //output attendu:
    //n1 = 1234567
    //n2 = -85698

    Halt();
}
#endif // CHANGED