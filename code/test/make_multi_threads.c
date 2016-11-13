#ifdef CHANGED
#include <syscall.h>

void putstring(char* arg) {
  
  PutString(arg);

}
void
putchar(char ch){
  int i = 1;
  for(i ; i < 5 ;i++){
  PutChar(ch);
  }
  PutChar(i-1+'0');
  PutChar('\n');

}


int main() {
  
  //multiThreading
  ThreadCreate(putchar,'a');
  ThreadCreate(putchar,'a');
  ThreadCreate(putchar,'a');
  ThreadCreate(putchar,'a');
  ThreadCreate(putchar,'a');
  ThreadCreate(putchar,'a');

  ///le thread principale ne doit pas se terminer
  while(1);


}
#endif // CHANGED
