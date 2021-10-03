//Banker's Algorithm
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<algorithm>
#include<time.h>

using namespace std;

//Processes are P0, P1, P2, P3, P4

#define numOfProcesses 5
#define numOfResources 3
int resourceA;
int resourceB;
int resourceC;
int resources[numOfResources];
int safeSequence[numOfProcesses];

void calculateAvailable(int allocated[numOfProcesses][numOfResources], int avail[numOfResources]){
	int i;
	int j;
	for(j=0; j<numOfResources; j++){
		int total = 0;
		for(i=0; i<numOfProcesses; i++){
			total = total + *((*(allocated+i))+j);
		}
		if(total < *(resources+j)){
			*(avail+j) = resources[j] - total;
		}
		else{
			cout<<"Cant allocate.\n";
			exit(0);
		}
	}
}

void findNeed(int allocation[numOfProcesses][numOfResources], 
	int max[numOfProcesses][numOfResources],
	int need[numOfProcesses][numOfResources+1]){
	for(int i=0; i<numOfProcesses; i++){
		int j;
		for(j=0; j<numOfResources; j++){
			*(*(need+i)+j) = *(*(max+i)+j) - *(*(allocation+i)+j);
		}
		j--;
		if((*(*(need+i)+j)) == 0 && 
			(*(*(need+i)+(j-1))) == 0 && 
			(*(*(need+i)+(j-2))) == 0){
			*(*(need+i)+numOfResources) = -1;	
		}
		else{
			*(*(need+i)+numOfResources) = 0;
		}
	}
}

void findSequence(int allocation[numOfProcesses][numOfResources], 
	int max[numOfProcesses][numOfResources], int available[numOfResources],
	int need[numOfProcesses][numOfResources+1]){
	findNeed(allocation, max, need);
	int flag1 = 0;
	// int flag2 = 0;
	int k = 0;
	int loops = 1;
	while(loops <= numOfProcesses){
		flag1 = 0;
		for(int i=0; i<numOfProcesses; i++){
			for(int j=0; j<numOfResources; j++){
				if(need[i][numOfResources] == -1){
					break;
				}
				if(need[i][j] > available[j]){
					break;
				}
				safeSequence[k] = i;
				k++;
				loops++;
				flag1 = 1;
				need[i][numOfResources] = -1;
				for(int l=0; l<numOfResources; l++){
					need[i][l] = 0;
					available[l] = available[l] + allocation[i][l];
				}
			}
		}
		if(flag1 == 0){
			cout<<"System has reached a deadlock.\n";
			break;
		}
	}
	for(int i=0; i<numOfProcesses; i++){
			cout<<"P"<<safeSequence[i];
			if(i!=numOfProcesses-1){
				cout<<"->";
			}
		}
	cout<<"\n";
	for(int i=0; i<numOfProcesses; i++){
		for(int j=0; j<numOfResources+1; j++){
			cout<<" | "<<need[i][j];
		}
		cout<<" |\n";
	}
}

int main(){
	resourceA = 10;
	resourceB = 5;
	resourceC = 7;
	resources[0] = resourceA;
	resources[1] = resourceB;
	resources[2] = resourceC;
	int allocation[numOfProcesses][numOfResources] = {{0, 1, 0},{2, 0, 0},
 													{3, 0, 2},{4, 1, 1},{0, 0, 2}};
	int max[numOfProcesses][numOfResources] = {{7, 5, 3},{3, 2, 2}, 
													{9, 0, 2}, {2, 2, 2},{4, 3, 3}};
	int available[numOfResources];
	calculateAvailable(allocation, available);
	for(int i=0; i<numOfResources; i++){
		cout<<" | "<<available[i];
	}
	cout<<" |\n";
	int need[numOfProcesses][numOfResources+1];
	findSequence(allocation, max, available, need);
	return 0;													
}