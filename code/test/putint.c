#include <syscall.h>

int main() {
  int n;
  PutString("Donner un nombre entier : \n");
  GetInt(&n);
  PutString("Test de PutInt : \n");
  PutString("Le nombre est : ");
  PutInt(n);
  PutChar('\n');
  Halt();
}
