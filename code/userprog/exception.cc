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
                        //On réserve la place pour le '\0'
                        nb_read = copyStringFromMachine(from, buf, MAX_STRING_SIZE - 1);

                        //afficher le contenu de buf
                        synchconsole->SynchPutString(buf);

                        //bouger le pointer from pour continuer la lecture
                        from += MAX_STRING_SIZE - 1;

                    } while (nb_read == MAX_STRING_SIZE - 1);

                    //libérer le buffer
                    free(buf);

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
 * (size + 1) caractères dont 1 caractère de plus pour '\0' au cas où 
 * (size) caractères sont écrits
 * 
 * @param from l'adresse virtuelle MIPS de la chaîne source
 * @param to l'adresse physique de la chaîne dest
 * @param size au plus size caractères doivent être écrits
 * @return nombre de caractères écrits ('\0' ne sera pas compté)
 */
int
copyStringFromMachine(int from, char *to, unsigned size) {
    unsigned i = 0;
    int ch;

    while (1) {
        //si (size) caractères son écrits: terminer la chaîne avec '\0' et sortir
        if (i == size) {
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
#endif //CHANGED
