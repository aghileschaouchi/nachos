#include <syscall.h>

int main() {
  int n;
  PutString("Test de GetInt : \n");
  PutString("Donner un nombre entier : \n");
  GetInt(&n);
  PutString("Le nombre est : ");
  PutInt(n);
  PutChar('\n');
  Halt();
}
