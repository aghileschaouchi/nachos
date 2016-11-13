// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#ifdef CHANGED
#include "synchconsole.h"
#include "userthread.h"
#endif // CHANGED

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------

static void
UpdatePC() {
    int pc = machine->ReadRegister(PCReg);
    machine->WriteRegister(PrevPCReg, pc);
    pc = machine->ReadRegister(NextPCReg);
    machine->WriteRegister(PCReg, pc);
    pc += 4;
    machine->WriteRegister(NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which) {
    int type = machine->ReadRegister(2);

    switch (which) {
        case SyscallException:
        {
            switch (type) {
                case SC_Halt:
                {
                    DEBUG('s', "Shutdown, initiated by user program.\n");
                    interrupt->Halt();
                    break;
                }
#ifdef CHANGED
                case SC_Exit:
                {
                    DEBUG('s', "Exit\n");
                    //récupérer la valeur de retour stockée dans r4
                    int n = machine->ReadRegister(4);
                    printf("main() returned %d\n", n);
                    interrupt -> Halt();
                    break;
                }
                case SC_PutChar:
                {
                    DEBUG('s', "PutChar\n");
                    //récupérer le caractère (arg1 de la fonction Putchar) stocké dans r4
                    int c = machine->ReadRegister(4);
                    synchconsole->SynchPutChar(c);
                    break;
                }
                case SC_PutString:
                {
                    DEBUG('s', "PutString\n");

                    //l'adresse du 1er caractere de la chaine
                    int from = machine -> ReadRegister(4);

                    //buffer
                    char *buf = (char*) malloc(sizeof (char)*MAX_STRING_SIZE);
                    int nb_read = 0;

                    do {
                        //copier au plus MAX_STRING_SIZE - 1 caractères au buffer
                        nb_read = copyStringFromMachine(from, buf, MAX_STRING_SIZE);

                        //afficher le contenu de buf
                        synchconsole->SynchPutString(buf);

                        //bouger le pointer from pour continuer la lecture
                        from += MAX_STRING_SIZE - 1;

                    } while (nb_read == MAX_STRING_SIZE - 1);

                    //libérer le buffer
                    free(buf);

                    break;
                }
                case SC_GetChar:
                {
                    DEBUG('s', "GetChar\n");

                    //récupérer le caractère
                    int c;
                    c = synchconsole->SynchGetChar();

                    //stocker le dans r2 comme valeur retournée de la fonction
                    machine->WriteRegister(2, c);

                    break;
                }
                case SC_GetString:
                {
                    DEBUG('s', "GetString\n");

                    //récupérer arg1 et arg2 de l'appel système
                    int to = (int) machine -> ReadRegister(4);
                    unsigned size = (unsigned) machine -> ReadRegister(5);

                    //allocation de mémoire
                    char *str = (char*) malloc(sizeof (char) * size);
                    synchconsole -> SynchGetString(str, size);

                    copyStringToMachine(str, to, size);
                    free(str);

                    break;
                }
                case SC_PutInt:
                {
                    DEBUG('s', "PutInt\n");

                    //récupérer l'entier (arg1 de la fonction PutInt) stocké dans r4
                    int n = machine -> ReadRegister(4);

                    //allouer mémoire; 16 est-il suffisant ?
                    char *buff = (char*) malloc(sizeof (char)*16);

                    //écrire l'entier 'n' dans le buffer
                    snprintf(buff, 16, "%d", n);

                    //on affiche le contenue de buff (l'entier en question) avec un simple 'SynchPutString'
                    synchconsole -> SynchPutString(buff);

                    free(buff);
                    break;
                }
                case SC_GetInt:
                {
                    DEBUG('s', "GetInt\n");

                    int n;
                    //on recupére l'adresse virtuelle  ou écrire l'entier lu après
                    int to = machine -> ReadRegister(4);

                    //on alloue une chaine de caractère pour y écrire l'entier après lecture de l'entrée standard
                    char *from = (char*) malloc(sizeof (char) * 16);

                    synchconsole -> SynchGetString(from, 16);
                    //on transforme la chaine de caractére lu en entier
                    sscanf(from, "%d", &n);
                    //on ecris l'entier lu dans l'adresse virtuelle de la variable
                    machine->WriteMem(to, sizeof (int), n);

                    free(from);

                    break;
                }

	    case SC_ThreadCreate: //working on it!
                {
		     DEBUG('s', "ThreadCreate\n");
		        //r5 contient l'argument arg nécessaire à la création du thread
		     int f = machine->ReadRegister(4),
		        //Récupérer la valeur de retour stockée dans r4 (la fonction f dans ce cas la)
		         arg = machine->ReadRegister(5),
		        //Récupérer l'adresse de la fonction ThreadExit stocké dans le r6
		         r = machine->ReadRegister(6);
		     
		     do_ThreadCreate(f, arg,r);
        
		     break;
                }

		case SC_ThreadExit:
                {
                    DEBUG('s', "ThreadExit\n");
		    //printf("\n--->i was here \n");
                    do_ThreadExit();
                    break;
                }
#endif // CHANGED
                default:
                {
                    printf("Unimplemented system call %d\n", type);
                    ASSERT(FALSE);
                }
            }

            // Do not forget to increment the pc before returning!
            UpdatePC();
            break;
        }

        case PageFaultException:
            if (!type) {
                printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
                ASSERT(FALSE);
            } else {
                printf("Page Fault at address %x at PC %x\n", type, machine->registers[PCReg]);
                ASSERT(FALSE); // For now
            }

        default:
            printf("Unexpected user mode exception %d %d at PC %x\n", which, type, machine->registers[PCReg]);
            ASSERT(FALSE);
    }
}

#ifdef CHANGED

/**
 * Implementation d'une procédure similaire à strncpy.
 * La copie de la chaîne se termine toujours par '\0'
 * 
 * Note: la zone mémoire allouée pour 'to' doit permettre contenir au moins
 * (size) caractères dont 1 caractère de plus pour le '\0' au cas où 
 * (size - 1) caractères sont déjà écrits
 * 
 * @param from l'adresse virtuelle MIPS de la chaîne source
 * @param to l'adresse physique de la chaîne dest
 * @param size au plus size-1 caractères doivent être écrits
 * @return nombre de caractères écrits (<= size - 1, '\0' ne sera pas compté)
 */
int
copyStringFromMachine(int from, char *to, unsigned size) {
    unsigned i = 0;
    int ch;

    while (1) {
        //si (size - 1) caractères sont écrits: terminer la chaîne avec '\0' et sortir
        if (i == size - 1) {
            to[i] = '\0';
            break;
        }

        //récupérer le caractère stocké à l'adresse virtuelle (from + i)
        //Note: 'from' est une adresse virtuelle, readMem la transforme en adresse physique
        machine->ReadMem(from + i, sizeof (char), &ch);
        to[i] = ch;

        //si le caractère récemment écrit est '\0': on est à la fin de la chaîne d'entrée
        if (ch == '\0')
            break;

        i++;
    }

    //retourner le nombre de caractères écrits
    return i;
}

/**
 * Implementation d'une procédure similaire à fgets().
 * La copie de la chaîne se termine toujours par '\0'
 * 
 * @param from l'adresse physique de la chaîne source
 * @param to l'adresse virtuelle de la chaîne dest
 * @param size au plus size-1 caractères doivent être lus
 * @return nombre de caractères lus (<= size - 1, '\0' ne sera pas compté)
 */
int
copyStringToMachine(char *from, int to, unsigned size) {
    unsigned i = 0;
    int ch;

    while (1) {
        //si (size - 1) caractères sont lus: terminer la chaîne avec '\0' et sortir
        if (i == size - 1) {
            machine->WriteMem(to + i, sizeof (char), '\0');
            break;
        }

        //récupérer le caractère à from[i] et stocker à l'adresse (to+i)
        ch = from[i];
        machine->WriteMem(to + i, sizeof (char), ch);

        //si le caractère récemment lu est '\0': on est à la fin de la chaîne d'entrée
        if (ch == '\0')
            break;

        i++;

        //si le caractère récemment lu est '\n': terminer la lecture
        if (ch == '\n') {
            machine->WriteMem(to + i, sizeof (char), '\0');
            break;
        }
    }

    //retourner le nombre de caractères lus
    return i;

}
#endif //CHANGED
