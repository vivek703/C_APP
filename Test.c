#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

sem_t thereBeData;

void* readFromFifoSendToFile(void) {

    FILE *fp = stdin;
    char buffer[100];
    FILE *file;

    file = fopen("./file", "a+");

    while(1) {
       fscanf(fp, "%s", buffer);

       fprintf(file,"%s\n",buffer);
       fflush(file);

       sem_post(&thereBeData); /*signal the consumer*/
    }
}

void* readFromFileAndPrint(void) {
    FILE *fp = 0;
    char buffer[100];
    int counter = 0;

    while(1) {
       sem_wait(&thereBeData); /*Waiting for the producer*/

       if (!fp) fp = fopen("file", "r");
       fscanf(fp, "%s", buffer);

       printf("%s\n", buffer);
    }    
}


int main(void)
{
    pthread_attr_t attr;
    pthread_t thread1;
    pthread_t thread2;

    sem_init(&thereBeData, 0,0);
    pthread_attr_init(&attr);    
    pthread_create(&thread1, &attr, readFromFifoSendToFile, (void*)0);
    pthread_create(&thread2, &attr, readFromFileAndPrint, (void*)0);

    sleep(10);
}
