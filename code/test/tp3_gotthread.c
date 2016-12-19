#include "syscall.h"
#define THIS "aaa"
#define THAT "bbb"

const int N = 10;
volatile int cont = 0;
void puts(const char *s){
  const char *p;
  for (p = s; *p != '\0'; p++){
    PutChar(*p);
    
  }
}

void f(void *arg){
  const char *s = arg;
  int i;
  for(i = 0; i < N; i++){
    puts(s);
    
  }
  ThreadExit();
}

void q(void *arg){
  const char *s = arg;
  int i;
  for(i = 0; i < N; i++){
    puts(s);
    
  }
  Exit(13);
  //ThreadExit();
}


int main(){
  ThreadCreate(q, THIS);
  ThreadCreate(f, THIS);
  ThreadCreate(f, THIS);
  ThreadCreate(f, THIS);
  ThreadCreate(f, THIS);
  ThreadCreate(f, THIS);
  ThreadCreate(f, THIS);
  ThreadCreate(f, THIS);
  ThreadCreate(f, THIS);
  
  ThreadExit();
}
