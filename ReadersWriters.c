#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

/* 
    Elaborado por Juan Lopez
    Codigo: 104617010731
*/

void *lector(void *parIndice);
void *escritor(void *parIndice);

sem_t semAccesoRecursos;
sem_t semMutex;
sem_t semServicioDeCola;

int varContadorLectores = 0;

void *lector(void *parIndice)
{
    int varIndice = *(int *)parIndice;

    sem_wait(&semServicioDeCola);
    sem_wait(&semMutex);
    if (varContadorLectores == 0)
    {
        sem_wait(&semAccesoRecursos);
    }
    varContadorLectores++;
    printf("lector %d esta leyendo.\n", varIndice);

    sem_post(&semServicioDeCola);
    sem_post(&semMutex);

    //proceso de lectura
    sleep(10);

    sem_wait(&semMutex);
    varContadorLectores--;
    printf("Lector %d termino de leer.\n", varIndice);

    if (varContadorLectores == 0)
    {
        sem_post(&semAccesoRecursos);
    }
    sem_post(&semMutex);
}

void *escritor(void *parIndice)
{
    int varIndice = *(int *)parIndice;

    sem_wait(&semServicioDeCola);
    sem_wait(&semAccesoRecursos);

    printf("Escritor %d esta modificando la base de datos.\n", varIndice);

    sem_post(&semServicioDeCola);

    sleep(5);
    printf("Escritor %d termino de modificarla base de datos.\n", varIndice);

    sem_post(&semAccesoRecursos);
}
int main()
{
    int varIndice[10] = {0,1,2,3,4,5,6,7,8,9};

    pthread_t hiloLectores[10], hiloEscritores[3];

    sem_init(&semAccesoRecursos, 0, 1);
    sem_init(&semMutex, 0, 1);
    sem_init(&semServicioDeCola, 0, 1);

    // creacion de hilos para lectores
    for (int i = 0; i < 10; i++)
    {
        pthread_create(&hiloLectores[i], NULL, (void *)lector, (void *)&varIndice[i]);
    }
    //Creacion de hilos de escritores
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&hiloEscritores[i], NULL, (void *)escritor, (void *)&varIndice[i]);
    }

    //sincronizacion lectores
    for (int i = 0; i < 10; i++)
    {
        pthread_join(hiloLectores[i], NULL);
    }

    //sincronizacion Escritores
    for (int i = 0; i < 3; i++)
    {
        pthread_join(hiloEscritores[i], NULL);
    }

    sem_destroy(&semAccesoRecursos);
    sem_destroy(&semMutex);
    sem_destroy(&semServicioDeCola);
}