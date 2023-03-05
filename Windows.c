// default libs
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/shm.h>

int sem, memcp, memp;
int nbArrivee;
int *type_win_hack;

struct sembuf Pmutex = {0, -1, 0};
struct sembuf Vmutex = {0, 1, 0};
struct sembuf PwindowsQueue = {1, -1, 0};
struct sembuf VwindowsQueue = {1, 1, 0};
struct sembuf PhackerQueue = {2, -1, 0};
struct sembuf VhackerQueue = {2, 1, 0};
struct sembuf Psembarrier = {3, -1, 0};
struct sembuf Vsembarrier = {3, 1, 0};
struct sembuf Pmutex2 = {4, -1, 0};
struct sembuf Vmutex2 = {4, 1, 0};
void barrier(int n)
{
    semop(sem, &Pmutex2, 1);
    type_win_hack[2]++;
    // printf("on board now %d", &type_win_hack[2]);
    if (type_win_hack[2] >= n)
    {
        semop(sem, &Vmutex2, 1);
        for (int i = 0; i < n; i++)
        {
            semop(sem, &Vsembarrier, 1);
            // nbArrivee = 0;
        }
    }
    else
    {
        semop(sem, &Vmutex2, 1);
        semop(sem, &Psembarrier, 1);
    }
}
void row_boat(int p)
{
    int i;
    printf("bateau demmarer par processus %d!!\n", p);

}; // end row

// la fct handler pour qst 6
void handler()
{

    printf("\nje vais quitter\n");

    row_boat(getpid());
    exit(2);
}

void declarer_arrivee(int n_hackers, int n_serfs)
{
    printf("boaaaard h: %d, w: %d\n", n_hackers, n_serfs);
} //  end print boat

int main(void)
{
    int is_captain = 0;
    int p;
    key_t clef;

    // printf("\n starting windows program");
    // recuperatino des semaphores
    clef = ftok("./creat.c", 2);
    if (clef == -1)
    {
        printf("\n erreur : la clé n’a pas été créée");
        exit(1);
    };
    sem = semget(clef, 5, 0666);
    if (sem == -1)
    {
        printf("\n Erreur de récupération des sémaphores");
        exit(2);
    }
    // recuperation de la memoire partagee

    clef = ftok("creat.c", 3);
    if (clef == -1)
    {

        printf("\n erreur : la clé n’a pas été créée");
        exit(1);
    }
    memp = shmget(clef, 3 * sizeof(int), 0666);
    if (memp == -1)
    {
        printf("\n Erreur de récupération de la mémoire partagée");
        exit(2);
    }
    /* Attachement des segments partagés  */
    type_win_hack = shmat(memp, 0, 0);

    /////////////////////////////////////////////////

    // it's a Hacker!
    semop(sem, &Pmutex, 1);
    type_win_hack[1]++;

    // The last thread (person) enters here:
    if (type_win_hack[1] == 4)
    {
        for (int i = 0; i < 4; i++)
            semop(sem, &VhackerQueue, 1);

        declarer_arrivee(type_win_hack[0], type_win_hack[1]);
        is_captain = 1;

        // ajoutee pour la question 6
        signal(SIGINT, handler);

        // ajouter pour faire kill 2 pour captain
        printf("\ni'm captain my pid est %d\n", getpid());
    }
    else if (type_win_hack[1] == 2 && type_win_hack[0] == 2)
    {
        semop(sem, &VhackerQueue, 1);
        semop(sem, &VwindowsQueue, 1);
        semop(sem, &VwindowsQueue, 1);

        declarer_arrivee(type_win_hack[0], type_win_hack[1]);
        is_captain = 1; // it becomes the captain

        // ajoutee pour la question 6
        signal(SIGINT, handler);

        // ajouter pour faire kill 2 pour captain
        printf("\ni'm captain my pid est %d\n", getpid());
    }
    else
    {
        // If it's not the last processus, it enters here

        // ajouter pour faire kill 2 pour n'importe quel process
        printf("\ni'm not captain my pid est %d\n", getpid());
        semop(sem, &Vmutex, 1);
        declarer_arrivee(type_win_hack[0], type_win_hack[1]);
    }
    barrier(4);
    sleep(15);
    if (is_captain == 1)
    {

        row_boat(getpid());
        semop(sem, &Vmutex, 1);
    }
    return 0;
}
