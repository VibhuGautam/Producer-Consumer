#include <bits/stdc++.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
using namespace std;

int buffer[BUFFER_SIZE];

int front = -1;
int rear = -1;

pthread_mutex_t mutex1;
sem_t full;
sem_t empty;

void sem_init()
{
    pthread_mutex_init(&mutex1 , nullptr);
    sem_init(&full , 0 , 0);
    sem_init(&empty , 0 , BUFFER_SIZE);
}

int insert_item(int item)
{
    if(front == 0 and rear == BUFFER_SIZE - 1 or front == rear + 1)
        return -1;
    else
    {  
        if(front == -1)        
          front = 0,
          rear = 0;
        
        else
          rear=(rear + 1) % BUFFER_SIZE;

        buffer[rear] = item;
        return 0;
    }
}

int remove_item()
{
    if(front == -1)
        return -1;
    int d_item = buffer[front];

    if(front == rear)
        front = -1 , rear = - 1;
    else 
        front = (front + 1) % BUFFER_SIZE;
    
    return d_item;
}

void *producer(void *param)
{
    int item;
    while(1)
    {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex1);

        int time = 1 + rand() % 5;
        sleep(time);

        item = rand() % 10;

        if(insert_item(item) == -1)
            cout << "Producer Error\n";
        else 
            cout << "Producer produced " << item <<  '\n';
        
        pthread_mutex_unlock(&mutex1);
        sem_post(&full);
    }
}

void *consumer(void *param)
{
    int item;
    while(1)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex1);

        int time = 1 + rand() % 5;
        sleep(time);

        item = remove_item();

        if(insert_item(item) == -1)
            cout << "Consumer Error\n";
        else 
            cout << "Consumer consumed " << item <<  '\n';
        
        pthread_mutex_unlock(&mutex1);
        sem_post(&empty);
    }
}

int main(int argc , char *argv[])
{
    int sleepTime = atoi(argv[1]);
    int noOfProducers = atoi(argv[2]);
    int noOfConsumers = atoi(argv[3]);
    pthread_t producerThreads[noOfProducers];
    pthread_t consumerThreads[noOfConsumers];
    sem_init();

    for(int i = 0; i < noOfProducers; ++i)
        pthread_create(&producerThreads[i] , nullptr , producer , nullptr);
    
    for(int i = 0; i < noOfConsumers; ++i)
        pthread_create(&consumerThreads[i] , nullptr , consumer , nullptr);
    
    sleep(sleepTime);

    for(int i = 0; i < noOfProducers; ++i)
        pthread_join(producerThreads[i] , nullptr);
    for(int i = 0; i < noOfConsumers; ++i)
        pthread_join(consumerThreads[i] , nullptr);
}


