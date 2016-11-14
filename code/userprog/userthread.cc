#ifdef CHANGED
#include "userthread.h"
#include "synch.h"

struct thread_args {
    int f;
    int arg;
    int r; //il va contien l'adresse de la fonction ThreadExit
};

//nombre de threads (y compris le thread initial)
int nb_thread = 1;

int
do_ThreadCreate(int f, int arg, int r) {
    //Création d'un nouveau thread qu'on lancera dans l'espace utilisateur
    Thread *thread = new Thread("newUserThread");

    if (thread == NULL) //la création du thread a échoué
        return -1;

    //On sauvegarde l'argument et la fonction à faire passer au thread 
    //dans une structure a fin de les faire passer a StartUserThread
    struct thread_args *schmurtz = (thread_args*) malloc(sizeof (struct thread_args));
    schmurtz -> f = f;
    schmurtz -> arg = arg;
    schmurtz -> r = r;

    //Faire passer le thread en utilisateur
    thread -> Start(StartUserThread, schmurtz);

    //Incrémenter le nombre des threads créés
    nb_thread++;

    return 0;
}
//Pour l'espace d'adressage il ne faut rien faire ! 
//(currentThread -> space) cette adresse est partagée par tous les threads

void
do_ThreadExit() {
    if (currentThread->getId() > -1) // si cela n'est le thread main
        //vider un slot
        currentThread -> space -> ClearBitMap();

    if (--nb_thread == 0)
        interrupt -> Halt();

    currentThread -> Finish();
}

static void StartUserThread(void *schmurtz) {
    int i;
    thread_args *thread = (thread_args*) schmurtz;

    //initialisation des registres
    for (i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister(i, 0);

    //Placer les paramètres dans le registre 4 
    machine->WriteRegister(4, thread -> arg);

    //écrire l'adresse de la fonction f
    machine->WriteRegister(PCReg, thread -> f);

    //Affecter a l'adresse de retour du Threadd l'adresse de la fonction ThreadExit
    machine->WriteRegister(31, thread -> r);

    //écrire l'adresse de la prochaine instruction
    machine->WriteRegister(NextPCReg, (thread -> f) + 4);

    // Allouer la pile utilisateur
    machine->WriteRegister(StackReg, currentThread -> space -> AllocateUserStack());

    machine -> Run();

    free(schmurtz);
}
#endif //CHANGED
