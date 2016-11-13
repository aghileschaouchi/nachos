#ifdef CHANGED
#include "userthread.h"
#include "synch.h"
struct thread_args{
  int f;
  int arg;
  int r;//il va contien l'adresse de la fonction ThreadExit
};
//nombres de threads
int nb_thread = 0;

int
do_ThreadCreate(int f, int arg,int r){
  

  //Création d'un nouveau thread qu'on lancera dans l'espace utilisateur
  Thread *thread = new Thread("newThread");
  
  thread -> space -> Sem_Thread -> P();
  
  //On sauvegarde l'argument et la fonction a faire passer au thread dans une structure a fin de les faire passer a StartUserThread
  struct thread_args *schmurtz = (thread_args*)malloc(sizeof(struct thread_args));
  schmurtz -> f = f;
  schmurtz -> arg = arg;
  schmurtz -> r = r;
  
  //Faire passer le thread en utilisateur
  thread -> Start(StartUserThread, schmurtz);
  
  nb_thread++;
  
  thread -> space ->  Sem_Thread -> V();
  
  //On renvoyer le status de thread crée
  return 19;
  //return thread -> status;
}
//Pour l'espace d'adressage il ne faut rien faire ! (currentThread -> space) cet adresse est partager par tous les thread
void
do_ThreadExit(){
  currentThread -> space ->  Sem_Thread -> P();
  
  //vider un slot
  currentThread -> space -> ClearBitMap();
  
  if(--nb_thread == 0)
    interrupt -> Halt();
  
  currentThread -> space -> Sem_Thread -> V();
  
  currentThread -> Finish();
}

static void StartUserThread(void *schmurtz){
  int i;
  thread_args *thread = (thread_args*)schmurtz;

  //initialisation des registres
  for(i = 0; i < NumTotalRegs; i++)
    machine->WriteRegister (i, 0);
  
  //Placer les paramétre dans le registre 4 
  machine->WriteRegister(4, thread -> arg);
  
  // Initial program counter -- must be location of "Start"
  machine->WriteRegister (PCReg, thread -> f);
  
  //Affecter a l'adresse de retour du Threadd l'adresse de la fonction ThreadExit
  machine->WriteRegister(31,thread -> r);
  
  // Need to also tell MIPS where next instruction is, because
  // of branch delay possibility

  //machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
  machine->WriteRegister (NextPCReg, (thread -> f) + 4);
  
  // Set the stack register to the end of the address space, where we
  // allocated the stack; but subtract off a bit, to make sure we don't
  // accidentally reference off the end!
  machine->WriteRegister (StackReg, currentThread -> space -> AllocateUserStack());
  
  machine -> Run();
  
  free(schmurtz);
}
#endif //CHANGED
