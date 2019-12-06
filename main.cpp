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
#include <algorithm> 
#include <chrono> 

using namespace std;
pthread_mutex_t monitor, output; 
pthread_cond_t canRead, canWrite, canPrint;
int readcount, sqCount, mqCount, waitingReaders;
bool busy;
vector<string> searchResults;
RedBlackTree* redBlackTree;

void initalize(RedBlackTree *rbt){
	pthread_mutex_init(&monitor, NULL);
	pthread_mutex_init(&output, NULL);
	pthread_cond_init(&canRead, NULL);
	pthread_cond_init(&canWrite, NULL);
	pthread_cond_init(&canPrint, NULL);
	sqCount = rbt->getSearchQueueSize();
	mqCount = rbt->getModifyQueueSize();
	readcount = 0;
	waitingReaders = 0;
	busy = false;
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
		pthread_mutex_lock(&monitor);
		sqCount--;
		if(sqCount < 0) {
			pthread_mutex_unlock(&monitor);
			break;
		}
		waitingReaders++;
		while(busy){
			pthread_cond_wait(&canRead, &monitor);
		}
		waitingReaders--;
		readcount++;
		int value =  redBlackTree->popSearchInvocation();
		pthread_mutex_unlock(&monitor);
		bool exist = redBlackTree->searchNode(value);
		if(exist == true){
			pthread_mutex_lock(&output);
			string temp = to_string(value) + " -> true, performed by thread: " + to_string((long)threadid);
			searchResults.push_back(temp);
			pthread_mutex_unlock(&output);
		}
		else {
			pthread_mutex_lock(&output);
			string temp = to_string(value) + " -> false, performed by thread: " + to_string((long)threadid);
			searchResults.push_back(temp);
			pthread_mutex_unlock(&output);
		}
		pthread_mutex_lock(&monitor);
		readcount--;
		if(readcount == 0){
			pthread_cond_signal(&canWrite);
		}
		pthread_mutex_unlock(&monitor);
	}
}

void *modify(void *threadid){
	while(true){
		pthread_mutex_lock(&monitor);
		mqCount--;
		if(mqCount < 0) {
			pthread_mutex_unlock(&monitor);
			break;
		}
		while(busy || readcount > 0){
			pthread_cond_wait(&canWrite, &monitor);
		}
		busy = true;
		string desc = redBlackTree->popModifyInvocation();
		int operation = insertOrDelete(desc);
		int value = getValue(desc);
		if(operation == 0){
			redBlackTree->insertNode(value);

		}
		else{
			redBlackTree->deleteNode(value);
		}
		busy = false;
		if(waitingReaders > 0){
			pthread_cond_signal(&canRead);
		}
		else{
			pthread_cond_signal(&canWrite);
		}
		pthread_mutex_unlock(&monitor);
		
	}


}



int main(){
	auto start = chrono::steady_clock::now(); 
	RedBlackTree* rbt = parseInput();
	int writerThreads = rbt->getNumWriters();
	int readerThreads = rbt->getNumReaders();
	cout << "writers: " + to_string(rbt->getNumWriters()) << endl;
	cout << "readers: " + to_string(rbt->getNumReaders()) << endl;
	pthread_t writers_Threads[writerThreads];
	pthread_t readers_Threads[readerThreads];
	initalize(rbt);
	int max = writerThreads > readerThreads ? writerThreads : readerThreads;

	for(int i = 0; i < max; i++){
		if(i < readerThreads){
			pthread_create(&readers_Threads[i], NULL, search, (void *)i);
		}
		if(i < writerThreads){
			pthread_create(&writers_Threads[i], NULL, modify, (void *)i);	
		}

		
	}  
	for(int i = 0; i < readerThreads; i++){
		pthread_join(readers_Threads[i], NULL);
	}

	for(int i = 0; i < writerThreads; i++){
		pthread_join(writers_Threads[i], NULL);
	} 
	ofstream outputFile;
	outputFile.open("writeFile.txt");
	auto end = chrono::steady_clock::now(); 
	auto diff = end - start;
	outputFile << chrono::duration <double, milli> (diff).count() << " ms" << endl;
	for(int i = 0; i < searchResults.size(); i++){
		outputFile << searchResults[i] << endl;
	}
	rbt->recreateTree(outputFile);
	outputFile.close();
	rbt->deconstruct(rbt->getRootNode());
	rbt->deleteNullNode();
	return 0;
}