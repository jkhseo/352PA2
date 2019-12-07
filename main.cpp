/**
 * @author Kyung Seo
 * @date 12/06/2019
 * helper.h is the header file for helper.cpp
 */
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

/**
 * Initalizes all of the global variables needed for the readers writers. 
 * @param rbt RedBlackTree object that is created from the input
 */
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
/**
 * This function tells us whether we need to insert or delete based off of the description of the modification invocation.
 * @param desc description of the modifying invocation. 
 * @return 0 = insert, and 1 = delete. 
 */
int insertOrDelete(string desc){
	if(desc.substr(0,1) == "i") return 0;
	return 1;
}

/**
 * This function tells us the value of the insert/delete based off of the description of the modification invocation. 
 * @param desc description of the modifying invocation. 
 * @return value
 */
int getValue(string desc){
	int value = stoi(desc.substr(1,desc.length()-1));
	return value;
}


/**
 * The search thread enters here to do the searches. 
 * @param threadid it is the thread number. 
 * @return
 */
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

/**
 * The modifying thread enteres her to do the insert/delete
 * @param threadid the number of the thread. 
 * @return
 */
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


/**
 * Main method that parses the inputfile and creates the tree, starts all the threads and writes to the output files. 
 * @return 
 */
int main(int argc,char* argv[]){
	auto start = chrono::steady_clock::now(); 
	string filename = "RBTinput.txt";
	if(argc >= 2){
		filename.assign(argv[1]);

	}
	ifstream infile;
	infile.open(filename);
	if(infile.fail()){
        cout << "file doesn't exist" << endl;
        return 0;
    }
	ofstream outputFile;
	outputFile.open("writeFile.txt");
	RedBlackTree* rbt = parseInput(infile);
	int writerThreads = rbt->getNumWriters();
	int readerThreads = rbt->getNumReaders();
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