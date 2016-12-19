#ifdef CHANGED
#include "syscall.h"

int
main(){
  ForkExec("test/tp3_gotthread");
  ForkExec("test/tp3_gotthread");
  ForkExec("test/tp3_gotthread");
  ForkExec("test/tp3_gotthread");
  ForkExec("test/tp3_gotthread");
  ForkExec("test/tp3_gotthread");
}
#endif //CHANGED
