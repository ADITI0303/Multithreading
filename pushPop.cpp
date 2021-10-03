#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>
#include<pthread.h>
#include<algorithm>
#include<list>

using namespace std;

int array[] = {67, 89, 90, 56, 100, 45, 78, 56, 76, 67};
list<int> ready;
list<int> :: iterator itr;
list<int> running;
sem_t mutex;
sem_t empty;
sem_t full;

void* producer(void* arg){
	while(!ready.empty()){
		sem_wait(&full);
		sem_wait(&mutex);
		int item = ready.front();
		running.push_back(item);
		cout<<"The produced item is: "<<item<<endl;
		ready.pop_front();
		sem_post(&mutex);
		sem_post(&empty);
	}
}

void* consumer(void* arg){
	while(!ready.empty()){
		sem_wait(&empty);
		sem_wait(&mutex);
		int item = running.front();
		cout<<"The consumed item is: "<<item<<endl;
		running.pop_front();
		sem_post(&mutex);
		sem_post(&full);
	}
}

int main(){
	for(int i=0; i<10; i++){
		ready.push_back(array[i]);
	}
	itr = ready.begin();
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, 1);
	sem_init(&empty, 0, 0);
	pthread_t threads[2];
	pthread_create(&threads[0], NULL, producer, (void*)0);
	pthread_create(&threads[1], NULL, consumer, (void*)1);
	for(int i=0; i<2; i++){
		pthread_join(threads[i], NULL);
	}
}