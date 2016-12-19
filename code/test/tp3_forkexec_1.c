#ifdef CHANGED
#include "syscall.h"

int
main(){
  ForkExec("test/tp3_userpages2");
  ForkExec("test/tp3_userpages1");
  ForkExec("test/tp3_userpages2");
  ForkExec("test/tp3_userpages1");
  ForkExec("test/tp3_userpages2");
  ForkExec("test/tp3_userpages1");
  ForkExec("test/tp3_userpages2");
  ForkExec("test/tp3_userpages1");
  ForkExec("test/tp3_userpages2");
  ForkExec("test/tp3_userpages1");
  ForkExec("test/tp3_userpages2");
  ForkExec("test/tp3_userpages1");
}
#endif //CHANGED
