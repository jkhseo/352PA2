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

int readcount;
pthread_mutex_t rc, wsem;
queue<int> searchQueue;
queue<string> modifyQueue;
RedBlackTree* redBlackTree;

void initalize(RedBlackTree *rbt){
	pthread_mutex_init(&rc, NULL);
	pthread_mutex_init(&wsem, NULL);
	readcount = 0;
	modifyQueue = rbt->getModifyQueue();
	searchQueue = rbt->getSearchQueue();
	redBlackTree = rbt;
}

void *insertNode(int value){

}

void *deleteNode(int value){

}


void *search(void *threadid){
	while(!searchQueue.empty()){
		cout<< (long)threadid <<  " reader is trying to enter" << endl;
		pthread_mutex_lock(&rc);
		readcount++;
		if(readcount == 1){
			pthread_mutex_lock(&wsem);
		}
		cout<< (long)threadid << " reader is inside" << endl;
		pthread_mutex_unlock(&rc);
		/*
		bool exist = redBlackTree->searchNode(searchQueue.front());
		if(exist == true){
			cout << searchQueue.front() << " -> true, performed by thread: " << (long)threadid << endl;
		}
		else {
			cout << searchQueue.front() << " -> false, performed by thread: " << (long)threadid << endl;
		}*/
		searchQueue.pop();

		pthread_mutex_lock(&rc);
		readcount--;
		if(readcount == 0){
			pthread_mutex_unlock(&wsem);
		}
		pthread_mutex_unlock(&rc);
		cout<< (long)threadid <<  " reader is done" << endl;
	}
}

void *modify(void *threadid){
	while(!modifyQueue.empty()){
		cout<< (long)threadid << " writer is trying to enter" << endl;
		pthread_mutex_lock(&wsem);
		cout<< (long)threadid << " writer has entered" << endl;
		cout << modifyQueue.front() <<endl;
		modifyQueue.pop();
		pthread_mutex_unlock(&wsem);   
		cout<< (long)threadid << " writer is leaving" << endl;
	}


}



int main(){
	RedBlackTree* rbt = parseInput();
	//rbt->preOrder();
	//rbt->inOrder();
	int writerThreads = rbt->getNumWriters();
	int readerThreads = rbt->getNumReaders();
	cout << "writers: " + to_string(rbt->getNumWriters()) << endl;
	cout << "readers: " + to_string(rbt->getNumReaders()) << endl;
	pthread_t writers_Threads[writerThreads];
	pthread_t readers_Threads[readerThreads];
	initalize(rbt);
	for(int i = 0; i < writerThreads; i++){
		pthread_create(&writers_Threads[i], NULL, search, (void *)i);
	}
	for(int i = 0; i < readerThreads; i++){
		pthread_create(&readers_Threads[i], NULL, modify, (void *)i);
	}
	pthread_exit(NULL);
	return 0;
}