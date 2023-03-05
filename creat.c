// default libs
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>

int nbArrivee = 0;

int sem, memcp, memp;

int main(void)
{
    int p, err;
    // int n_hackers = atoi(argv[0]);
    // int n_windows = atoi(argv[1]);

    key_t clef;

    /* Création des sémaphores */

    clef = ftok("./creat.c", 2);
    sem = semget(clef, 5, IPC_CREAT | 0666);
    if (sem == -1)
    {
        printf("\n Erreur de creation des sémaphores");
        exit(1);
    }

    /* Initialisation des sémaphores */

    semctl(sem, 0, SETVAL, 1); // mutex
    semctl(sem, 1, SETVAL, 0); // windowsque
    semctl(sem, 2, SETVAL, 0); // hackerque
    semctl(sem, 3, SETVAL, 0); // sembarrier
    semctl(sem, 4, SETVAL, 1); // mutex2

    /* Création des segments partagés */
    // nombre de hackers et windows dans le bateau
    clef = ftok("creat.c", 3);

    memp = shmget(clef, 3 * sizeof(int), IPC_CREAT | 0666);
    if (memp == -1)
    {
        printf("\n Erreur de creation de la mémoire partagée");
        exit(2);
    }

    // clef = ftok("TP_main.c", 4);

    // memcp = shmget(clef, sizeof(int), IPC_CREAT | 0666);
    // if (memcp == -1)
    // {
    //     printf("\n Erreur de creation de la mémoire partagée");
    //     exit(2);
    // }
    // int nbrArrivee = shmat(memcp, 0, 0);
    // nbrArrivee = 0;
    /* Attachement */
    int *type_win_hack = shmat(memp, 0, 0);
    type_win_hack[0] = 0; // nbr windows
    type_win_hack[0] = 0; // nbr hackers
    type_win_hack[0] = 0; // nbr personnes dans le bateau

    /*detachement */
    // shmdt(nbrArrivee);
    shmdt(type_win_hack);

    // creation des processus
    int NB_HACKERS = 2;
    int NB_WINDOWS = 2;
    for (int i = 0; i < NB_HACKERS; i++)
    {
        p = fork();
        if (p < 0)
        {
            printf("prblm %d", p);
        }
        if (p == 0)
        {
            printf("gonna start  hacker");
            err = execl("./hacker", "Hacker", NULL);
            printf("\n Erreur de chargement du programme Hacker N° erreur %d\n", err);
            exit(4);
        }
    }

    for (int i = 0; i < NB_WINDOWS; i++)
    {
        p = fork();
        if (p < 0)
        {
            printf("prblm %d", p);
            p = wait(NULL);
            while (p != -1)
            {
                printf("\n  Fils terminé %d", p);
                p = wait(NULL);
            }
        }
        else if (p == 0)
        {
            printf("gonna start  windows");
            err = execl("./windows", "Windows", NULL);
            printf("\n Erreur de chargement du programme Windows N° erreur %d\n", err);
            exit(4);
        }
    }

    p = wait(NULL);
    while (p != -1)
    {
        // printf("\n  Fils terminé %d", p);
        p = wait(NULL);
    }
    semctl(sem, IPC_RMID, 0);
    shmctl(memp, IPC_RMID, 0);
    shmctl(memcp, IPC_RMID, 0);

    return 0;
}