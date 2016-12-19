// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#include "translate.h"

#ifdef CHANGED
#include "bitmap.h"
#endif //CHANGED

#define UserStacksAreaSize		1024*4	// increase this as necessary!

#ifdef CHANGED
#define NB_MAX_THREAD                     4*4

class Semaphore;
class Thread;
//Struct contenant un pointeur vers un thread et une variable 'ON' qui peut être soit 0 soit 1 selon le fait que le thread est en train de faire un PutString ou pas
//Pour chaque processus, correspond un AddrSpace et donc un tableau de t_thread de taille NB_MAX_THREAD, chaque processus peut allouer un tel nombre de threads, qui seront stockés par la suite dans ce tableau, afin de pouvoir satisfaire ce qui a été exigé dans les bonus 1 et 2
typedef struct t_thread{
  Thread* thread;   //Thread utilisateur
    int on;//Est-ce que le thread est en train de faire un PutString où non (Bonus 2 du Tp3) 
}t_thread;
#endif //CHANGED


class AddrSpace:dontcopythis
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch
#ifdef CHANGED
    int AllocateUserStack();
    //pour ACTION 2.3 2.4
    void ClearBitMap();
    //TP3 Bonus 1, mettre un thread utilisateur crée dans le tableau 
  void PutBuffThread(Thread *thread,int i);
  // Enlever un thread du tableau et mettre NULL à sa place
  int PutNullInBuff(Thread *thread,int nb_thread);
  //Appeler Finish() pour tous les threads contenus dans le tableau
  void ExitAllThread(int nb_thread);
  //TP3 Bonus 2, mettre la variable 'on' du thread à vrai (le thread est en train de faire un PutString)
  int OnPutString(Thread* thread);
  // Mettre la variable 'on' du thread à faux (le thread a fini de faire un PutString)
  int LeftPutString(Thread* thread);
  // Récupérer le nombre de threads qui sont en train de faire un PutString
  int NbOnPutString();
  // Récupérer le nombre de threads du processus courant
  int getNbThread();
  // Incrémenter le nombre de threads du processus courant
  void incNbThread();
  // Décrementer le nombre de threads du processus courant
  void decrNbThread();
#endif //CHANGED

  private:
      TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    unsigned int numPages;	// Number of pages in the virtual 
    // address space
#ifdef CHANGED
    // création d'un bitmap
    BitMap *bitmap;
    // Thread semaphore
    Semaphore *thread_sem;
  //Un seule thread qui décremente ou incrémente le nombre de threads
  Semaphore *thread_sem_1;
  //TP3 Bonus 1, chaque processus peut avoir au plus 12 thread
  //il dispose d'un tableau de thread qui contiendra l'adresse des threads qu'on lui alloue
  t_thread *buffThread;
  //nombre de threads (y compris le thread initial)
  int nb_thread;
#endif //CHANGED
};

#endif // ADDRSPACE_H
