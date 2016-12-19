// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#include "syscall.h"
#include "new"

#ifdef CHANGED
#include "synch.h"
static void ReadAtVirtual(OpenFile* executable, int virtualaddr, int numBytes, int position, TranslationEntry* pageTable, unsigned numPages);
#endif //CHANGED
//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
	WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{
    NoffHeader noffH;
    unsigned int i, size;
    
#ifdef CHANGED
    //Initialisation d'un BitMap de 12 (NB_MAX_THREAD == 12) slots
    bitmap = new BitMap(NB_MAX_THREAD);

    //initialise sémaphore
    thread_sem = new Semaphore("Pas de débordement", NB_MAX_THREAD);
    thread_sem_1 = new Semaphore("Incrementation et dec", 1);
    //Allocation d'un tableaux de t_thread
    buffThread = new t_thread[NB_MAX_THREAD];
    //Initialisation du tableau
    for (int i = 0; i < NB_MAX_THREAD; i++) {
        buffThread[i].thread = NULL;
        buffThread[i].on = 0;
    }

    //nombre de threads (y compris le thread initial)
    nb_thread = 1;
#endif //CHANGED
    
    executable->ReadAt (&noffH, sizeof (noffH), 0);
    //ReadAtVirtual(executable, noffH.noffMagic, sizeof(noffH.noffMagic), 0, pageTable, numPages);
    if ((noffH.noffMagic != NOFFMAGIC) &&
	(WordToHost (noffH.noffMagic) == NOFFMAGIC))
	SwapHeader (&noffH);
    /* Check that this is really a MIPS program */
    ASSERT (noffH.noffMagic == NOFFMAGIC);

// how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStacksAreaSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;

    // check we're not trying
    // to run anything too big --
    // at least until we have
    // virtual memory
    if (numPages > NumPhysPages)
	    throw std::bad_alloc();

    DEBUG ('a', "Initializing address space, num pages %d, total size 0x%x\n",
	   numPages, size);
// first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    for (i = 0; i < numPages; i++)
      {
#ifdef CHANGED
	
	pageTable[i].physicalPage = pageprovider-> GetEmptyPage();
	
#endif //CHANGED
	  pageTable[i].valid = TRUE;
	  pageTable[i].use = FALSE;
	  pageTable[i].dirty = FALSE;
	  pageTable[i].readOnly = FALSE;	// if the code segment was entirely on 
	  // a separate page, we could set its 
	  // pages to be read-only
      }

// then, copy in the code and data segments into memory
    if (noffH.code.size > 0)
      {
	  DEBUG ('a', "Initializing code segment, at 0x%x, size 0x%x\n",
		 noffH.code.virtualAddr, noffH.code.size);
#ifdef CHANGED
        ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size,
                noffH.code.inFileAddr, pageTable, numPages);
#endif //CHANGED
      }
    if (noffH.initData.size > 0)
      {
	  DEBUG ('a', "Initializing data segment, at 0x%x, size 0x%x\n",
		 noffH.initData.virtualAddr, noffH.initData.size);
#ifdef CHANGED
	  
        ReadAtVirtual(executable, noffH.initData.virtualAddr, noffH.initData.size,
                noffH.initData.inFileAddr, pageTable, numPages);
	  
#endif //CHANGED
      }

    DEBUG ('a', "Area for stacks at 0x%x, size 0x%x\n",
	   size - UserStacksAreaSize, UserStacksAreaSize);

    pageTable[0].valid = FALSE;			// Catch NULL dereference
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
  // LB: Missing [] for delete
  // delete pageTable;
#ifdef CHANGED
  //Libérer les pages 
  unsigned int i;
  for (i = 0; i < numPages; i++){
    
    machine-> pageprovider-> ReleasePage( pageTable[i].physicalPage );
  }
  delete thread_sem;
  delete thread_sem_1;
  delete buffThread;
  delete bitmap;
#endif // CHANGED
  delete [] pageTable;
  // End of modification

}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, USER_START_ADDRESS);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to 0x%x\n",
	   numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}

#ifdef CHANGED

void
AddrSpace::ClearBitMap() {
    bitmap->Clear(currentThread->getId());
    //signaler qu'une place est dispo
    thread_sem->V();
}

int
AddrSpace::AllocateUserStack() {
    //attendre jusqu'a ce qu'une place soit disponible
    thread_sem->P();
    
    int id = bitmap -> Find();
    
    ASSERT(id != -1);
    
    currentThread->setId(id);
    
    //Chaque thread a une pile différente que celle du précedent.
    //Avec un espace de 256 octets pour chaque thread, pour l'instant
    //on ne peut exécuter que 4 threads à la fois pour assurer que
    //l'espace utilisateur UserStacksAreaSize (1024) ne sera pas débordé.

    return numPages * PageSize - id * 256;
}

/**
 * Methode qui met le thread créé dans le tableau de threads
 * @param thread
 * @param i indice
 */
void
AddrSpace::PutBuffThread(Thread *thread, int i) {
    thread_sem_1 -> P();
  buffThread[i].thread = thread;
    thread_sem_1 -> V();
}

/**
 * Parcourir le tableau de threads, chercher l'emplacement du thread qu'on veut terminer et le mettre a NULL
 * @param thread
 * @param nb_thread
 * @return 
 */
int
AddrSpace::PutNullInBuff(Thread *thread, int nb_thread) {
    int i;
  //On parcourt tous les threads du processus courant, on cherche le thread courant et on met son adresse à NULL
  for(i = 0; i < nb_thread; i++)
    if(buffThread[i].thread == thread){
      buffThread[i].thread = NULL;
            return 0;
        }
    return -1;
}

/**
 * Parcourir le tableau de threads du processus courant et appeler Finish() sur chacun
 */
void
AddrSpace::ExitAllThread(int nb_thread) {
    int i;
    thread_sem_1 -> P();
  //printf("<<<<<<%d>>>>>",currentThread->space->getNbThread()-1);
  for(i = 0; i < nb_thread; i++)
    if(buffThread[i].thread != NULL){
      // printf("addrese:%p", buffThread[i]);
      buffThread[i].thread -> Finish();
        }
    thread_sem_1 -> V();
}

/**
 * Renvoyer le nombre de threads du processus courant
 * @return 
 */
int
AddrSpace::getNbThread() {
    return nb_thread;
}

/**
 * Incrémenter le nombre de threads
 */
void
AddrSpace::incNbThread() {
    thread_sem_1 -> P();
    nb_thread++;
    //printf("<<<<<<%d>>>>>",currentThread->space->getNbThread()-1);
    thread_sem_1 -> V();
}

/**
 * Décrementer le nombre de threads
 */
void
AddrSpace::decrNbThread() {
    thread_sem_1 -> P();
    nb_thread--;
    //printf("<<<<<<%d>>>>>",currentThread->space->getNbThread()-1);
    thread_sem_1 -> V();
}
//TP3 Bonus 2
int
AddrSpace::OnPutString(Thread* thread){
  for(int i = 0; i < nb_thread; i++)
    if(buffThread[i].thread == thread){
      buffThread[i].on = 1;
      return 0;
    }
}

int
AddrSpace::LeftPutString(Thread* thread){
  for(int i = 0; i < nb_thread; i++)
    if(buffThread[i].thread == thread){
      buffThread[i].on = 0;
      return 0;
    }
}
int
AddrSpace::NbOnPutString(){
  int n(0);
  for(int i = 0; i < nb_thread; i++)
    if(buffThread[i].thread != NULL && buffThread[i].on == 1)
      n++;
  return n;
}

static void ReadAtVirtual(OpenFile* executable, int virtualaddr, int numBytes, int position, TranslationEntry* pageTable, unsigned numPages) {
    char buffer[numBytes];
    int nb_write = 0;

    TranslationEntry *old_pageTable = machine->pageTable;
    unsigned int old_pageTableSize = machine->pageTableSize;
    
  //échange pour notre traitement
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
    
    executable->ReadAt(buffer, numBytes, position);
    while (nb_write < numBytes) {
        machine->WriteMem(virtualaddr + nb_write, 1, buffer[nb_write]);
        nb_write++;
    }
    
  //Restoration 
    machine->pageTable = old_pageTable;
    machine->pageTableSize = old_pageTableSize;
}


#endif //CHANGED
