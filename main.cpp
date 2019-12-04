#include <unistd.h>
#include <stdio.h>
#include <string>
#include <queue>
#include "RedBlackTree.h"
#include "treeHelper.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>

using namespace std;

int readcount, sqCount, mqCount;
pthread_mutex_t rc, wsem, rq, output, size_sq, size_mq;

RedBlackTree* redBlackTree;

void initalize(RedBlackTree *rbt){
	pthread_mutex_init(&rc, NULL);
	pthread_mutex_init(&wsem, NULL);
	pthread_mutex_init(&rq, NULL);
	pthread_mutex_init(&output, NULL);
	pthread_mutex_init(&size_sq, NULL);
	pthread_mutex_init(&size_mq, NULL);
	readcount = 0;
	sqCount = rbt->getSearchQueueSize();
	mqCount = rbt->getModifyQueueSize();
	redBlackTree = rbt;
}

int insertOrDelete(string desc){
	if(desc.substr(0,1) == "i") return 0;
	return 1;
}

int getValue(string desc){
	int value = stoi(desc.substr(1,desc.length()-1));
	return value;
}



void *search(void *threadid){
	while(true){
		pthread_mutex_lock(&rc);
		readcount++;
		if(readcount == 1){
			pthread_mutex_lock(&wsem);
		}
		pthread_mutex_unlock(&rc);


		pthread_mutex_lock(&size_sq);
		sqCount--;
		if(sqCount < 0) {
			pthread_mutex_unlock(&size_sq);
			break;
		}
		pthread_mutex_unlock(&size_sq);



		pthread_mutex_lock(&rq);
		int value =  redBlackTree->popSearchInvocation();
		pthread_mutex_unlock(&rq);
		


		bool exist = redBlackTree->searchNode(value);
		if(exist == true){
			pthread_mutex_lock(&output);
			cout << value << " -> true, performed by thread: " << (long)threadid << endl;
			pthread_mutex_unlock(&output);
		}
		else {
			pthread_mutex_lock(&output);
			cout << value << " -> false, performed by thread: " << (long)threadid << endl;
			pthread_mutex_unlock(&output);
		}


		pthread_mutex_lock(&rc);
		readcount--;
		if(readcount == 0){
			pthread_mutex_unlock(&wsem);
		}
		pthread_mutex_unlock(&rc);

	}
	pthread_mutex_unlock(&wsem);
}

void *modify(void *threadid){
	while(true){
		pthread_mutex_lock(&wsem);
		mqCount--;
		if(mqCount < 0){
			pthread_mutex_unlock(&wsem); 
			break;
		}
		string desc = redBlackTree->popModifyInvocation();
		int operation = insertOrDelete(desc);
		int value = getValue(desc);
		if(operation == 0){
			redBlackTree->insertNode(value);
			pthread_mutex_lock(&output);
			cout << value << " inserted, performed by thread: " << (long)threadid << endl;
			pthread_mutex_unlock(&output);

		}
		else{
			redBlackTree->deleteNode(value);
			pthread_mutex_lock(&output);
			cout << value << " deleted, performed by thread: " << (long)threadid << endl;
			pthread_mutex_unlock(&output);
		}
		pthread_mutex_unlock(&wsem);   
	}


}



int main(){
	
	RedBlackTree* rbt = parseInput();
	int writerThreads = rbt->getNumWriters();
	int readerThreads = rbt->getNumReaders();
	cout << "writers: " + to_string(rbt->getNumWriters()) << endl;
	cout << "readers: " + to_string(rbt->getNumReaders()) << endl;
	pthread_t writers_Threads[writerThreads];
	pthread_t readers_Threads[readerThreads];
	initalize(rbt);

	for(int i = 0; i < readerThreads; i++){
		pthread_create(&readers_Threads[i], NULL, search, (void *)i);
		
	} 
	for(int i = 0; i < writerThreads; i++){
		pthread_create(&writers_Threads[i], NULL, modify, (void *)i);
		
	}

	for(int i = 0; i < readerThreads; i++){
		pthread_join(readers_Threads[i], NULL);
	}

	for(int i = 0; i < writerThreads; i++){
		pthread_join(writers_Threads[i], NULL);
	} 

	cout << "Ria is my love" << endl;







	return 0;
}