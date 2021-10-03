#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>
#include<pthread.h>
#include<algorithm>
#include<list>
#include<time.h>

using namespace std;

#define NPHIL 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
typedef struct _philosopher{
	int id;
	int status;
}philosopher;

philosopher phil[NPHIL];

sem_t forks[NPHIL];
sem_t mutex;

void thinking(int philNum){
	phil[philNum].status = THINKING;
	cout<<"Philosopher "<<phil[philNum].id<<" is thinking\n";
}

void hungry(int philNum){
	phil[philNum].status = HUNGRY;
	cout<<"Philosopher "<<phil[philNum].id<<" is hungry\n";
}

void eating(int philNum){
	phil[philNum].status = EATING;
	cout<<"Philosopher "<<phil[philNum].id<<" is eating\n";
}

void* executeLR(void* args){
	intptr_t id = (intptr_t)args;
	while(1){
		sleep(1);
		thinking(id);
		sleep(1);
		hungry(id);
		sem_wait(&forks[id]); //Pick up the left fork
		cout<<"Philosopher "<<phil[id].id<<" has picked up left fork "<<id<<endl;
		sem_wait(&forks[(id+1)%NPHIL]); //Pick up the right fork
		cout<<"Philosopher "<<phil[id].id<<" has picked up right fork "<<(id+1)%NPHIL<<endl;
		sleep(1);
		eating(id);
		sem_post(&forks[id]);
		cout<<"Philosopher "<<phil[id].id<<" has kept left fork "<<id<<endl;
		sem_post(&forks[(id+1)%NPHIL]);
		cout<<"Philosopher "<<phil[id].id<<" has kept right fork "<<(id+1)%NPHIL<<endl;
		thinking(id);
	}
}

void* executeRL(void* args){
	intptr_t id = (intptr_t)args;
	while(1){
		sleep(1);
		thinking(id);
		sleep(1);
		hungry(id);
		sem_wait(&forks[(id+1)%NPHIL]); //Pick up the right fork
		cout<<"Philosopher "<<phil[id].id<<" has picked up right fork "<<(id+1)%NPHIL<<endl;
		sem_wait(&forks[id]); //Pick up the left fork
		cout<<"Philosopher "<<phil[id].id<<" has picked up left fork "<<id<<endl;
		sleep(1);
		eating(id);
		sem_post(&forks[(id+1)%NPHIL]);
		cout<<"Philosopher "<<phil[id].id<<" has kept right fork "<<(id+1)%NPHIL<<endl;
		sem_post(&forks[id]);
		cout<<"Philosopher "<<phil[id].id<<" has kept left fork "<<id<<endl;
	}
}

int main(){
	pthread_t philosophers[NPHIL];
	int i;
	for(int j=0; j<NPHIL; j++){
		phil[j].id = j;
		phil[j].status = THINKING;
	}
	for(i=0; i<NPHIL-1; i++){
		pthread_create(&philosophers[i], NULL, executeLR,(void*)i);
	}
	pthread_create(&philosophers[i], NULL, executeRL, (void*)i);
	for(int j=0; j<NPHIL; j++){
		sem_init(&forks[j], 0, 1);
	}
	sem_init(&mutex, 0, 1);
	for(int j=0; j<NPHIL; j++){
		pthread_join(philosophers[j], NULL);
	}
	return 0;
}