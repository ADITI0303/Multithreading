#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>
#include<pthread.h>
#include<algorithm>
#include<list>

using namespace std;

int array[] = {67, 89, 90, 56, 100, 45, 78, 56, 76, 67, 88, 35};
list<int> ready;
list<int> :: iterator itr;
list<int> running;
sem_t mutex;
sem_t empty;
sem_t full;
int numberOfproducers;
int numberOfconsumers;

void* consumer(void* arg){	
	intptr_t ptr = (intptr_t)arg;
	while(ready.size()>0){
		cout<<"Thread "<<ptr<<" is trying to consume"<<endl;
		sem_wait(&empty);
		sem_wait(&mutex);
		int item = running.front();
		cout<<"Thread "<<ptr<<" consumed item: "<<item<<endl;
		running.pop_front();
		sem_post(&mutex);
		sem_post(&full);    
	}
}

void* producer(void* arg){
	intptr_t ptr = (intptr_t)arg;
	while(ready.size()>numberOfproducers-1){
		cout<<"Thread "<<ptr<<" is trying to produce"<<endl;
		sem_wait(&full);
		sem_wait(&mutex);
		int item = ready.front();
		running.push_back(item);
		cout<<"Thread "<<ptr<<" produced item: "<<item<<endl;
		ready.pop_front();
		sem_post(&mutex);
		if(running.size() == numberOfproducers){sem_post(&empty);}
	}
}

int main(){
	for(int i=0; i<(sizeof(array)/sizeof(array[0])); i++){
		ready.push_back(array[i]);
	}
	cout<<"Enter the size of buffer: ";
	cin>>numberOfproducers;
	numberOfconsumers = 1;
	int total = numberOfconsumers+numberOfproducers;
	sem_init(&mutex, 0, 1);
	sem_init(&full, 0, numberOfproducers);
	sem_init(&empty, 0, 0);
	pthread_t threads[total];
	pthread_create(&threads[0], NULL, consumer, (void*)0);
	for(int i=1; i<total; i++){
		pthread_create(&threads[i], NULL, producer, (void*)i);
	}

	for(int i=0; i<total; i++){
		pthread_join(threads[i], NULL);
	}
}