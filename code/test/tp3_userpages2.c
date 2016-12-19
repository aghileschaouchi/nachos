#include "syscall.h"
#define THIS "ccc"
#define THAT "ddd"

const int N = 10;
volatile int cont = 0;
void puts(const char *s){
  const char *p;
  for (p = s; *p != '\0'; p++)
    PutChar(*p);
}

void f(void *arg){
  const char *s = arg;
  int i;
  for(i = 0; i < N; i++)
    puts(s);
  PutString("\nJai fini !\n");
  PutInt(++cont);
  PutChar('\n');
  ThreadExit();
}

int main(){
  ThreadCreate(f, THIS);
  ThreadCreate(f, THAT);
  
  ThreadCreate(f, THIS);
  ThreadCreate(f, THAT);
  ThreadCreate(f, THIS);
  ThreadCreate(f, THAT);
  ThreadCreate(f, THIS);
  ThreadCreate(f, THAT);
  ThreadCreate(f, THIS);
  ThreadCreate(f, THAT);
  ThreadCreate(f, THIS);
  
  f(THAT);
  ThreadExit();
}
