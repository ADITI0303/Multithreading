#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>
#include<pthread.h>
#include<algorithm>
#include<list>

using namespace std;

#define numberOfPhilosophers 5
#define THINK 0
#define HUNGRY 1
#define EATING 2
int philosophers[2][numberOfPhilosophers] = {{1, 2, 3, 4, 5}, {0, 0, 0, 0, 0}};

sem_t mutex;
sem_t semArray[numberOfPhilosophers];

void checkNeighbors(int philNum){
	int philLeft = ((philNum)%numberOfPhilosophers);
	int philRight = ((philNum+(numberOfPhilosophers))%numberOfPhilosophers);
	if(philosophers[0][philNum-1] == HUNGRY && philosophers[0][philLeft] != EATING
		&& philosophers[0][philRight] != EATING){
		philosophers[1][philNum-1] = EATING;
		cout<<"Philosopher "<<philNum<<" takes chopstick "<<philLeft+1<<" on the left ";
		cout<<" and chopstick "<<philRight+1<<" on the right"<<endl;
		sem_post(&semArray[philNum-1]);
	}
}

void pickUp(int philNum){
	sem_wait(&mutex);
	//change the state from thinking to hungry
	philosophers[1][philNum-1] = HUNGRY;
	//to be able to eat, the neighbors must not be eating
	checkNeighbors(philNum);
	sem_post(&mutex);
	//if not able to eat, wait
	sem_wait(&semArray[philNum]);
}

void putDown(int philNum){
	sem_wait(&mutex);
	philosophers[1][philNum-1] = THINK;
	int checkLeft = (philNum)%numberOfPhilosophers;
	int checkRight = (philNum+numberOfPhilosophers)%numberOfPhilosophers;
	checkNeighbors(checkLeft);
	checkNeighbors(checkRight);
	sem_post(&mutex);
}

void* philosopher(void* arg){
	//Algorithm
	//THINK
	//PICK UP
	//EAT
	//PUT DOWN
	while(true){
		intptr_t ptr = (intptr_t)arg;
		pickUp(ptr);
		putDown(ptr);
	}
}

void initializePhilosophers(){
	//Initialize all to Thinking
	for(int i=0; i<numberOfPhilosophers; i++){
		philosophers[1][i] = THINK;
	}
}

int main(){
	//Each thread is each philosopher
	pthread_t philThreads[numberOfPhilosophers];
	for(int i=0; i<numberOfPhilosophers; i++){
		pthread_create(&philThreads[i], NULL, philosopher, (void*)(i+1));
	}
	initializePhilosophers();
	//mutex
	sem_init(&mutex, 0, 1);
	for(int i=0; i<numberOfPhilosophers; i++){
		sem_init(&semArray[i], 0, 0);
	}
	for(int i=0; i<numberOfPhilosophers; i++){
		pthread_join(philThreads[i], NULL);
	}
	return 0;
}