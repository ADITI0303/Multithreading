//Readers Writers

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

//Declare two semaphores
sem_t wrMutex;
sem_t rdMutex;
int numOfRdrWrt;
int readers;
int rdCount = 0;

void* writer(void* arg){
	intptr_t id = (intptr_t)arg;
	while(1){
		//Two writers cannot write together
		sem_wait(&wrMutex);
		//Write here
		cout<<"Writer is writing to the file\n";
		// sem_post(&rdMutex);
		sem_post(&wrMutex);
		sleep(1);
	}
}

void* reader(void* arg){
	intptr_t id = (intptr_t)arg;
	while(1){
		//A reader cannot read when a writer is writing
		//Two readers can read together
		sem_wait(&rdMutex);
		//Only one reader can access this at one time
		readers++;
		rdCount++;
		if(readers == 1){
			sem_wait(&wrMutex);
		}
		sem_post(&rdMutex);
		//Read here
		cout<<"Reader "<<id<<" is reading from the file\n";
		// sem_wait(&rdMutex);
		// if(rdCount%2 == 0){
		// 	cout<<"Thread "<<id<<" is accessing\n";
		// 	sem_post(&wrMutex);
		// 	sem_wait(&rdMutex);
		// }
		// sem_post(&rdMutex);
		// cout<<"Im here\n";
		sem_wait(&rdMutex);
		readers--;
		if(readers == 0){
			// cout<<"Can't go here.\n";
			sem_post(&wrMutex);
		}
		sem_post(&rdMutex);
		sleep(1);
	}
}

int main(){
	sem_init(&wrMutex, 0, 1);
	sem_init(&rdMutex, 0, 1);
	numOfRdrWrt = 3;
	readers = 0;
	pthread_t threads[numOfRdrWrt];
	// for(int i=0; i<numOfRdrWrt/2; i++){
		pthread_create(&threads[0], NULL, writer, (void*)0);
	// }
	for(int i=numOfRdrWrt/2; i<numOfRdrWrt; i++){
		pthread_create(&threads[i], NULL, reader, (void*)i);
	}
	for(int i=0; i<numOfRdrWrt; i++){
		pthread_join(threads[i], NULL);
	}
}