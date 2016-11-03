#ifdef CHANGED
#include <syscall.h>

void print(char c) {
    int i;

    for (i = 0; i < 3; i++) {
        PutChar(c + i);
    }
    PutChar('\n');
}

int main() {//working on it!
  char c = 'X';
  //appel de ThreadCreate avec la fonction print qui fait des PutChar
  int ret =ThreadCreate(print, c);
  ThreadExit();
  
  //le thread principale ne doit pas se terminer
  while(1);
  
  //Halt();

}
#endif // CHANGED
