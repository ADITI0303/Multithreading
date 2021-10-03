#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>
#include<pthread.h>
#include<algorithm>

using namespace std;

int MAX = 2;
int buffer[2];
int fil = 0;
int use = 0;
int loops = 10;
int numOfThreads;

void put(int value){
	buffer[fil] = value;
	fil = (fil + 1)%MAX;
}

int get(){
	int tmp = buffer[use];
	use = (use+1)%MAX;
	return tmp;
}

//Declaring the semaphores
sem_t empty;
sem_t full;
sem_t mutex;

void* producer(void* arg){
	int i;
	for(i=0; i<loops; i++){
		// sem_wait(&mutex);							//P0
		sem_wait(&empty);							//P1
		sem_wait(&mutex);
		put(i);										//P2
		cout<<"The produced value is: "<<i<<endl;
		sem_post(&mutex);
		sem_post(&full);							//P3
		// sem_wait(&mutex);							//P4
	}
}

void* consumer(void* arg){
	int i, tmp = 0;
	for(i=0; i<loops/(numOfThreads-1); i++){
		// sem_wait(&mutex);							//C0
		sem_wait(&full);							//C1
		sem_wait(&mutex);
		tmp = get();								//C2
		cout<<"The consumed item is: "<<tmp<<endl;
		sem_post(&mutex);
		sem_post(&empty);							//C3
		// sem_wait(&mutex);							//C4
	}
}

int main(int argc, char const *argv[])
{
	/* code */
	sem_init(&empty, 0, MAX);
	sem_init(&full, 0, 0);
	sem_init(&mutex, 0, 1);
	numOfThreads = 3;
	pthread_t threads[numOfThreads];
	pthread_create(&threads[0], NULL, &producer, (void*)0);
	pthread_create(&threads[1], NULL, &consumer, (void*)1);
	pthread_create(&threads[2], NULL, &consumer, (void*)2);
	for(int i=0; i<numOfThreads; i++){
		pthread_join(threads[i], NULL);
	}
	return 0;
}