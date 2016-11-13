#ifdef CHANGED
#include <syscall.h>
volatile int a =0;
void print(char c) {
  int i;
  
  for (i = 0; i < 3; i++) {
    PutChar(c + i);
  }
  PutChar('\n');
  a = 1;
  //ThreadExit();
}

void putstring(char* arg) {
  PutChar('x');
  PutString(arg);
  PutChar('\n');
  a = 1;
  //ThreadExit();
}


int main() {//working on it!
  char c = 'X';
  //appel de ThreadCreate avec la fonction print qui fait des PutChar
  //int ret =ThreadCreate(print, c);
  ThreadCreate(putstring, "hghgjhgfgsdfgsdfgsdfgsdfgdwfgh");
  //PutChar par le thread main
  PutChar('\n');
  PutChar('G');
  PutChar('\n');
  //PutString par le thread main
  PutChar('\n');
  PutString("teabababababababababababast");
  PutChar('\n');
  
  //le thread principale ne doit pas se terminer
  while(!a);
  
  //Halt();

}
#endif // CHANGED
