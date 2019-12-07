/**
 * @author Kyung Seo
 * @date 12/06/2019
 * treeHelper.cpp is the file responsible for processing the input file. It will handle all invocations
 * and construct the tree from the input.
 */
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


/**
 * This function parses the user input so that we can process the command properly
 * @param user_in contains the entire single command in a single string
 * @param delim  is the delimiter that will be used to tokenize the string into a list of strings.
 * @param tokens is where the tokenized strings will be stored.
 */
void tokenizeInputs(string line, const char* delim, vector<string> &tokens){
    char* current = strtok(const_cast<char*>(line.c_str()), delim);
    while(current != NULL){
      tokens.push_back(string(current));
      current = strtok(NULL , delim);
    }
}

/**
 * This function takes in the list of invocations and populate the search and modify queue.
 * @param line the line of the input.txt file that contains the list of invocations
 * @param rbt the redBlackTree object.
 */
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

/**
 * This function takes in the node description and returns the value
 * @param desc node description for example 17b, or 16r
 * @return This returns the value of the node.
 */
int descToValue(string desc){
	if(desc.length() == 1) return INT_MIN;
	int value = stoi(desc.substr(0, desc.length()-1));
	return value;
}

/**
 * This function takes in the input file and finds the number of search threads
 * @param desc is the line Search threads: x
 * @return x
 */
int numOfReadersWriters(string desc){
	int num = stoi(desc.substr(15,desc.length()-15));
	return num;
}

/**
 * This function takes in the input file and finds the number of modify threads
 * @param desc is the line Modify threads: x
 * @return x
 */
int descToColor(string desc){
	if(desc.substr(desc.length()-1,1)=="b") return 1;
	return 0;
}

/**
 * This function takes in the node description and creates a new node object
 * @param desc node description for example 17b, or 16r
 * @return The node object from the description.
 */
Node* makeNode(string desc){
	if(desc == "f") {
		return NULL;
	}
	int value = descToValue(desc);
	Node* node = new struct Node(value);
	setNodeColor(node, descToColor(desc));
	return node;
}

/**
 * This function takes in all of the nodes in the input file and constructs the tree based off the preorder traversal.
 * @param preOrderVector is the list of all nodes in a vector in the same order as the testfile.
 * @param index is the index of the preOrderVector that we are working with.
 * @param rbt is the RedBlackTree object we are working with.
 * @param used is the boolean array that lets us know whether we have created the node or not.
 * @return
 */
Node* constructRBTFromPreOrder(vector<string> &preOrderVector, int index, RedBlackTree *rbt, vector<bool> &used){
	if(index >= preOrderVector.size()){
		return rbt->getNullNode();

	}
	if(preOrderVector[index].size() == 1){
		used[index] = true;
		return rbt->getNullNode();
	}
	struct Node* root = makeNode(preOrderVector[index]);
	used[index] = true;
	index++;
	struct Node* left = constructRBTFromPreOrder(preOrderVector, index, rbt, used);
	root->left = left;
	left->parent = root;
	while(used[index] == true){
		index++;
	}
	struct Node* right = constructRBTFromPreOrder(preOrderVector,index, rbt, used);
	root->right = right;
	right->parent = root;
	return root;
}

/**
 * This function is reponsible for creating the RedBlackTree and finding out the number of search and modify threads
 * @return The redblacktree constructed from the inputfile.
 */
RedBlackTree* parseInput(ifstream &infile){
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
	vector<bool> used(arrsize, false); 

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

