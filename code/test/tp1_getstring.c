#ifdef CHANGED
#include <syscall.h>

int main() {
    char buf[5];

    GetString(buf, 5);
    while (buf[0] != '\0') { // check for empty string
        PutString(buf);
        GetString(buf, 5);
    }

    return 99;
}
#endif // CHANGED