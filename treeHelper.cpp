#include <unistd.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include "RedBlackTree.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <climits>
#include <vector>

using namespace std;



void tokenizeInputs(string line, const char* delim, vector<string> &tokens){
    char* current = strtok(const_cast<char*>(line.c_str()), delim);
    while(current != NULL){
      tokens.push_back(string(current));
      current = strtok(NULL , delim);
    }
}

void processInvocations(string line, RedBlackTree* rbt){
	vector<string> invocations; 
	string invocation; 
	string value;
	tokenizeInputs(line, " || ", invocations);
	for(int i = 0; i<invocations.size(); i++){
		invocation = invocations[i];
		value = value = invocation.substr(7,invocation.size()-8);
		if(invocation.substr(0,1) == "s"){
			rbt->addSearchInvocation(stoi(value));
		}
		else if(invocation.substr(0,1) == "d"){
			rbt->addModifyInvocation("d"+value);
		}
		else if(invocation.substr(0,1) == "i"){
			rbt->addModifyInvocation("i"+value);
		}
	}
	
}

int descToValue(string desc){
	if(desc.length() == 1) return INT_MIN;
	int value = stoi(desc.substr(0, desc.length()-1));
	return value;
}

int numOfReadersWriters(string desc){
	int num = stoi(desc.substr(15,desc.length()-15));
	return num;
}

int descToColor(string desc){
	if(desc.substr(desc.length()-1,1)=="b") return 1;
	return 0;
}


Node* makeNode(string desc){
	if(desc == "f") {
		return NULL;
	}
	int value = descToValue(desc);
	Node* node = new struct Node(value);
	setNodeColor(node, descToColor(desc));
	return node;
}

Node* consructTree(vector<string> &preOrderVector, RedBlackTree *rbt){
	for(int i = 0; i < preOrderVector.size(); i++){
		int value = descToValue(preOrderVector[i]);
		if(value == INT_MIN) continue; 
		rbt->insertNode(value);
	}
}

Node* constructRBTFromPreOrder(vector<string> &preOrderVector, int index, RedBlackTree *rbt, bool used[]){
	if(index >= preOrderVector.size()){
		return rbt->getNullNode();
	}
	used[index] = true;
	struct Node* root = makeNode(preOrderVector[index]);
	index++;
	if(preOrderVector[index].size() == 1){
		used[index] = true;
		rbt->addNullLeftChild(root);
		index++;
		if(index >= preOrderVector.size()){
			return root;
		}
		if(preOrderVector[index].size() == 1){
			rbt->addNullRightChild(root);
			used[index] = true;
			return root;
		}
		else{
			struct Node* right = constructRBTFromPreOrder(preOrderVector, index, rbt, used);
			root->right = right;
			right->parent = root;
			return root;
		}
	}
	if(index >= preOrderVector.size()){
		return rbt->getNullNode();
	}
	struct Node* left = constructRBTFromPreOrder(preOrderVector,index, rbt, used);
	root->left = left;
	left->parent = root;
	while(used[index] == true){
		index++;
	}
	if(preOrderVector[index].size() == 1){
		used[index] = true;
		rbt->addNullRightChild(root);
		return root; 
	}
	struct Node* right = constructRBTFromPreOrder(preOrderVector,index, rbt, used);
	root->right = right;
	right->parent = root;
	return root;
}

RedBlackTree* parseInput(){
	ifstream infile;
	infile.open("RBTinput.txt");
	string line;
	vector<string> input;
	while(getline(infile, line)){
		input.push_back(line);
	}
	infile.close();
	string preOrderString = input[0];
	vector<string> preOrderVector;
	tokenizeInputs(preOrderString, ",", preOrderVector);
	int arrsize = 0;
	if(preOrderVector.size() > 0){
		arrsize = preOrderVector.size();
	}
	bool used[arrsize];
	for(int i = 0; i < arrsize; i++){
		used[i] = false;
	}
	RedBlackTree* rbt = new RedBlackTree();
	struct Node* root = constructRBTFromPreOrder(preOrderVector,0, rbt, used);
	rbt->setRoot(root);
	rbt->setNumReaders(numOfReadersWriters(input[1]));
	rbt->setNumWriters(numOfReadersWriters(input[2]));
	for(int i = 3; i < input.size(); i++){
		line = input[i];
		processInvocations(line, rbt);
	}
	return rbt;





}

