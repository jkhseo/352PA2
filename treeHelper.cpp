#include <unistd.h>
#include <stdio.h>
#include <string>
#include "RedBlackTree.h"
#include <iostream>
#include <fstream>
#include <queue>
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
	cout<<"init"<<endl;
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
	int num = stoi(desc.substr(desc.length()-1,1));
	return num;
}

int descToColor(string desc){
	if(desc.substr(desc.length()-1,1)=="b") return 1;
	return 0;
}


node* makeNode(string desc){
	if(desc == "f") {
		return NULL;
	}
	int value = descToValue(desc);
	node* node = new struct node(value);
	setNodeColor(node, descToColor(desc));
	return node;
}

node* constructRBTFromPreOrder(vector<string> &preOrderVector){
	if(preOrderVector.size()==0)
		return NULL;
	struct node* root = makeNode(preOrderVector[0]);
	vector<string> left_preorder;
	vector<string> right_preorder;
	int i = 1; 
	if(preOrderVector[i].size() == 1){
		root->left = NULL;
		i++;
	}
	if(preOrderVector[i].size() == 1){
		root->right = NULL;
		i++;
		return root;
	}
	while(descToValue(preOrderVector[i]) < root->value && i < preOrderVector.size()){
		left_preorder.push_back(preOrderVector[i++]);
	}
	while(i < preOrderVector.size()){
		right_preorder.push_back(preOrderVector[i++]);
	}
	root->left = constructRBTFromPreOrder(left_preorder);
	root->right = constructRBTFromPreOrder(right_preorder);
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
	/*
	for(int i =0; i < preOrderVector.size();i++){
		cout<< preOrderVector[i] <<endl;
	}
	*/
	struct node* root = constructRBTFromPreOrder(preOrderVector);
	RedBlackTree* rbt = new RedBlackTree();
	rbt->setRoot(root);
	rbt->setNumReaders(numOfReadersWriters(input[1]));
	rbt->setNumWriters(numOfReadersWriters(input[2]));
	cout<<"before"<<endl;
	for(int i = 3; i < input.size(); i++){
		line = input[i];
		processInvocations(line, rbt);
	}
	cout<<"after"<<endl;
	return rbt;





}

