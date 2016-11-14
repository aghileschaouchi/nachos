#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvailHandler(void *arg) {
    (void) arg;
    readAvail->V();
}

static void WriteDoneHandler(void *arg) {
    (void) arg;
    writeDone->V();
}

SynchConsole::SynchConsole(const char *in, const char *out) {
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    
    Sem_PutChar = new Semaphore("only one thread on SynchPutChar", 1);
    Sem_GetChar = new Semaphore("only one thread on SynchGetChar", 1);

    Sem_PutString = new Semaphore("only one thread on SynchPutString", 1);
    Sem_GetString = new Semaphore("only one thread on SynchGetString", 1);
    
    console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, 0);
}

SynchConsole::~SynchConsole() {
    delete console;
    delete writeDone;
    delete readAvail;
    delete Sem_GetChar;
    delete Sem_PutChar;
    delete Sem_PutString;
    delete Sem_GetString;
}

void SynchConsole::SynchPutChar(int ch) {
  Sem_PutChar -> P();
  
  console->PutChar(ch);
  writeDone->P();

  Sem_PutChar -> V();
}

int SynchConsole::SynchGetChar() {
  Sem_GetChar -> P();

  
  readAvail->P();
  int return_val  = console->GetChar();
  
  Sem_GetChar -> V();
  return return_val;
}

void SynchConsole::SynchPutString(const char s[]) {
  Sem_PutString -> P();
  int i = 0;
  while (s[i] != '\0') {
    SynchPutChar(s[i]);
    i++;
  }
  Sem_PutString -> V();
}

void SynchConsole::SynchGetString(char *s, int n) {
  Sem_GetString -> P();
  int i = 0;
  char ch;
  while (i < n - 1) {
    ch = SynchGetChar();
    
    if (ch == EOF)
            break;
    
    s[i] = ch;
    i++;
    
    if (ch == '\n')
            break;
    
  }
  s[i] = '\0';
  
  Sem_GetString -> V();
}
#endif // CHANGED
