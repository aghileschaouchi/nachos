#ifdef CHANGED
#include "userfork.h"
#include "thread.h"
#include "addrspace.h"
#include "synch.h"
#include "system.h"
#include "console.h"

struct Fork_args {
    AddrSpace* space;
    int exit; //contient l'adresse de la fonction Exit
};

void
Process(void* schmurtz) {
    Fork_args* args = (Fork_args*) schmurtz;
    
    // On affecte comme espace d'adresse a notre thread l'argument
    currentThread -> space = args -> space;
    currentThread -> space->InitRegisters();
    currentThread -> space->RestoreState();

    //Return puis Exit
    machine->WriteRegister(31, args -> exit);
    
    free(schmurtz);
    machine -> Run();
    ASSERT(FALSE); // machine->Run never returns;
}

int do_ForkExec(char *filename, int exit) {
    struct Fork_args *schmurtz = (Fork_args*) malloc(sizeof (struct Fork_args));
    OpenFile *executable;
    AddrSpace *space;
    
    //On crée un thread noyau qui se chargera d'executer notre fichier
    Thread* process = new Thread("Processus");
    
    //On ouvre l'executable et on le stock dans un OpenFile*
    executable = fileSystem->Open(filename);
    if (executable == NULL) {
        printf("Unable to open file %s\n", filename);
        return 1;
    }
    
    //On crée un nouvel espace d'adressage à partir de notre executable
    space = new AddrSpace(executable);
    schmurtz -> space = space;
    schmurtz -> exit = exit;
    
    //On lance la création du thread noyau avec comme espace d'adressage celui qu'on vient de créer
    process->Start(Process, schmurtz);
    
    incNumberProc(); // On incrémente le nombre de processus existant

    delete executable;
    return 0;
}
#endif
