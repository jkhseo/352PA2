/**
 * @author Kyung Seo
 * @date 12/06/2019
 * RedBlackTree.cpp is the file for the RedBlackTree class.
 * Credits to https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/ for teaching me the logic and helping me implement the RBT.
 */
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <stack> 
#include <climits>
#include "RedBlackTree.h"

using namespace std;

//enum representing node colors
enum Color {RED, BLACK};

/**
 * This functio returns of the color of the node
 * @param node Node we are looking at 
 * @return color of the node
 */
int getNodeColor(NodePtr node){
	if(node == NULL) return BLACK;
	return node->color;
}


/**
 * This function sets the node's color as the specificed color
 * @param node Node we are interested in
 * @param color Color we want to change to
 */
void setNodeColor(NodePtr node, int color) {
    if (node == NULL){
        return;
    }
    node->color = color;
}

/**
 * Contructor for the Node struct
 * @param value Value of the node
 */
Node::Node(int value){
	this->value = value;
	color = RED;
	parent = NULL;
	left = NULL;
	right = NULL;
}

/** 
 * Empty constructor for the Node struct
 */
Node::Node(){
}

/**
 * This function returns the node that represents the null node.
 * @return the null node representation.
 */
NodePtr RedBlackTree::getNullNode(){
	return this->null;
}

/**
 * This function returns the root of the RBT.
 * @return RedBlackTree's root
 */
NodePtr RedBlackTree::getRootNode(){
	return this->root;
}

/**
 * This function desconstructs the tree and free all the memory of the nodes by post-order traversal. 
 * @param node the node object that we are looking at during the traversal. 
 */
void RedBlackTree::deconstruct(NodePtr node){
	if(node == null){
		return;
	}
	deconstruct(node->left);
	deconstruct(node->right);
	delete node;
}

/**
 * This function deletes the null node representation. 
 */
void RedBlackTree::deleteNullNode(){
	delete getNullNode();
}

/**
 * This function writes to the output file the pre-order traversal of the tree.
 * @param outputFile is the ofstream object that is used to write the tree. 
 */
void RedBlackTree::recreateTree(ofstream &outputFile){
	string color;
	stack<NodePtr> nodeStack;
	nodeStack.push(this->root);
	while(!nodeStack.empty()){
		NodePtr temp = nodeStack.top();
		nodeStack.pop();
		if(temp == null){
			outputFile << ",f";
			continue;
		}
		
		else{
			color = temp->color==RED ? "r" : "b";
			if(temp->value == this->root->value){
				outputFile << to_string(temp->value) + color;
			}
			else{
				outputFile << "," + to_string(temp->value) + color;
			}
			nodeStack.push(temp->right);
			nodeStack.push(temp->left);
		}
	}

}

/**
 * This fu
 * @param root
 * @param indent
 * @param last
 */
void RedBlackTree::printHelper(NodePtr root, string indent, bool last){
	if (root != null) {
		   cout<<indent;
    if (last) {
        cout<<"R----";
        indent += "     ";
    }   
    else {
        cout<<"L----";
        indent += "|    ";
    }
            
   string sColor = root->color? "BLACK":"RED";
   cout<<root->value<<"("<<sColor<<")"<<endl;
   printHelper(root->left, indent, false);
   printHelper(root->right, indent, true);
 }
		
}

/**
 * This function takes in two nodes and reassign pointers to maintain the structure of the redBlackTree after deletion.
 * @param n1 node 1 
 * @param n2 node 2 
 */
void RedBlackTree::rbTransplant(NodePtr n1, NodePtr n2){
	if (n1->parent == NULL) {
		root = n2;
	} else if (n1 == n1->parent->left){
		n1->parent->left = n2;
	} else {
		n1->parent->right = n2;
	}
	n2->parent = n1->parent;
}

/**
 *
 */
void RedBlackTree::prettyPrint() {
	    if (root) {
    		printHelper(this->root, "", true);
	    }
	}

/**
 * This function returns the size of the searchQueue.
 * @return size of searchQueue
 */
int RedBlackTree::getSearchQueueSize(){
	return this->searchQueue.size();
}

/**
 * This function returns the size of the modifyQueue.
 * @return size of modifyQueue
 */
int RedBlackTree::getModifyQueueSize(){
	return this->modifyQueue.size();
}

/**
 * This function checks of the searchQueue is empty
 * @return true if searchQueue is empty
 */
bool RedBlackTree::searchQueueEmpty(){
	if(this->searchQueue.empty()) return true;
	return false;
}

/**
 * This function checks of the modifyQueue is empty
 * @return true if modifyQueue is empty
 */
bool RedBlackTree::modifyQueueEmpty(){
	if(this->modifyQueue.empty()) return true;
	return false;
}

/**
 * This function grabs the next value to search and removes that value from the searchQueue. 
 * @return the next value to search. 
 */
int RedBlackTree::popSearchInvocation(){
	if(this->searchQueueEmpty()) return INT_MIN;
	int value = this->searchQueue.front();
	this->searchQueue.pop();
	return value;
}

/**
 * This function grabs the next value to either insert or delete and removes that value from the modifyQueue.
 * @return the value to modify. 
 */
string RedBlackTree::popModifyInvocation(){
	string value = this->modifyQueue.front();
	this->modifyQueue.pop();
	return value;
}

/**
 * This function searchs the value in the tree. 
 * @param value is the value we want to search in the tree. 
 * @return true if exsits and false if it doesn't
 */
bool RedBlackTree::searchNode(int value){
	NodePtr node = root;
	while(node != NULL){
		if(value > node->value){
			node = node->right;
		}
		else if(value < node->value){
			node = node->left;
		}
		else{
			return true;
		}
	}
	return false;
}

/**
 * This returns the searchQueue
 * @return search Queue
 */
queue<int> RedBlackTree::getSearchQueue(){
	return this->searchQueue;
}

/**
 * This function returns the modifyQueue
 * @return modifyQueue
 */
queue<string> RedBlackTree::getModifyQueue(){
	return this->modifyQueue;
}

/**
 * This function takes in a value and adds it to the searchQueue
 * @param value that we want to put into the searchQueue. 
 */
void RedBlackTree::addSearchInvocation(int value){
	this->searchQueue.push(value);
}

/**
 * This function takes in the description of the node we want to either insert or delete and adds it to the list. 
 * @param next is the description of the node that we want to modify next. 
 */
void RedBlackTree::addModifyInvocation(string next){
	this->modifyQueue.push(next);
}

/**
 * This function is the constructor for the RedBlackTree. 
 */
RedBlackTree::RedBlackTree(){
	null = new Node;
	null->color = BLACK;
	null->left = NULL;
	null->right = NULL;
	root = null;
}

/**
 * This function takes in a node and sets the root of the RBT.
 * @param node that we want to set as the new root. 
 */
void RedBlackTree::setRoot(NodePtr node){
	this->root = node;
}

/**
 * This function takes in a node and finds the minimum value node within the subtree. 
 * @param node that we want to search
 * @return the minimum value node. 
 */
NodePtr RedBlackTree::getMinValueNode(NodePtr node){
	NodePtr temp = node;
	while(temp->left != null){
		temp = temp->left;
	}
	return temp;
}


/**
 * Takes in a node and sets its left child as the null node representation. 
 * @param node
 */
void RedBlackTree::addNullLeftChild(NodePtr  node){
	node->left = null;
}

/**
 * Takes in a node and sets its right child as the null node representation. 
 * @param node
 */
void RedBlackTree::addNullRightChild(NodePtr  node){
	node->right = null;
}

/**
 * Takes in a node and performs a left rotation. 
 * @param node
 */
void RedBlackTree::rotateLeft(NodePtr node){
	NodePtr rChild = node->right;
	node->right = rChild->left;
		if (rChild->left != null) {
			rChild->left->parent = node;
		}
		rChild->parent = node->parent;
		if (node->parent == NULL) {
			this->root = rChild;
		} else if (node == node->parent->left) {
			node->parent->left = rChild;
		} else {
			node->parent->right = rChild;
		}
		rChild->left = node;
		node->parent = rChild;
	}

/**
 * Takes in a node and performs a right rotation. 
 * @param node
 */
void RedBlackTree::rotateRight(NodePtr node){
	NodePtr lChild = node->left;
	node->left = lChild->right;
		if (lChild->right != null) {
			lChild->right->parent = node;
		}
		lChild->parent = node->parent;
		if (node->parent == NULL) {
			this->root = lChild;
		} else if (node == node->parent->right) {
			node->parent->right = lChild;
		} else {
			node->parent->left = lChild;
		}
		lChild->right = node;
		node->parent = lChild;
	}

/**
 * After inserting a node, we want to restructure an fix the RBT so that the properties of the RBT holds. 
 * @param node
 */
void RedBlackTree::fixInsertNode(NodePtr node){
	NodePtr temp;
	while (node->parent->color == RED) {
			if (node->parent == node->parent->parent->right) {
				temp = node->parent->parent->left; 
				if (temp->color == RED) {
					temp->color = BLACK;
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
				} else {
					if (node == node->parent->left) {
						node = node->parent;
						rotateRight(node);
					}
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					rotateLeft(node->parent->parent);
				}
			} else {
				temp = node->parent->parent->right; 

				if (temp->color == RED) {
					temp->color = BLACK;
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
				} else {
					if (node == node->parent->right) {
						node = node->parent;
						rotateLeft(node);
					}
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					rotateRight(node->parent->parent);
				}
			}
			if (node == root) {
				break;
			}
		}
		root->color = BLACK;
	}


/**
 * This function takes in a value and inserts a new node into the RBT with this value
 * @param value
 */
void RedBlackTree::insertNode(int value){
	NodePtr node = new Node;
	node->parent = NULL;
	node->value = value;
	node->left = null;
	node->right = null;
	node->color = RED;

	NodePtr prev = NULL;
	NodePtr curr = this->root;
	while (curr != null) {
			prev = curr;
			if (node->value < curr->value) {
				curr = curr->left;
			} else {
				curr = curr->right;
			}
		}
		node->parent = prev;
		if (prev == NULL) {
			root = node;
		} else if (node->value < prev->value) {
			prev->left = node;
		} else {
			prev->right = node;
		}
		if (node->parent == NULL){
			node->color = BLACK;
			return;
		}
		if (node->parent->parent == NULL) {
			return;
		}

		// Fix the tree
		fixInsertNode(node);
	}

/**
 * After deleting a node, we want to restructure an fix the RBT so that the properties of the RBT holds.  
 * @param node
 */
void RedBlackTree::fixDeleteNode(NodePtr node){
	NodePtr temp;
	while (node != root && node->color == BLACK) {
			if (node == node->parent->left) {
				temp = node->parent->right;
				if (temp->color == RED) {
					temp->color = BLACK;
					node->parent->color = RED;
					rotateLeft(node->parent);
					temp = node->parent->right;
				}

				if (temp->left->color == BLACK && temp->right->color == BLACK) {
					temp->color = RED;
					node = node->parent;
				} else {
					if (temp->right->color == BLACK) {
						temp->left->color = BLACK;
						temp->color = RED;
						rotateRight(temp);
						temp = node->parent->right;
					}
					temp->color = node->parent->color;
					node->parent->color = BLACK;
					temp->right->color = BLACK;
					rotateLeft(node->parent);
					node = root;
				}
			} else {
				temp = node->parent->left;
				if (temp->color == RED) {
					temp->color = BLACK;
					node->parent->color = RED;
					rotateRight(node->parent);
					temp = node->parent->left;
				}

				if (temp->right->color == BLACK && temp->left->color == BLACK) {
					temp->color = RED;
					node = node->parent;
				} else {
					if (temp->left->color == BLACK) {
						// catempe 3.3
						temp->right->color = BLACK;
						temp->color = RED;
						rotateLeft(temp);
						temp = node->parent->left;
					}

					// catempe 3.4
					temp->color = node->parent->color;
					node->parent->color = BLACK;
					temp->left->color = BLACK;
					rotateRight(node->parent);
					node = root;
				}
			}
		}
		node->color = BLACK;
	}

/**
 * This is a helper function takes in the root of the RBT and the value we want to delete. If the value 
 * exists in the tree, we will delete it. 
 * @param node
 * @param value
 */
void RedBlackTree::deleteNodeHelper(NodePtr node, int value){
	NodePtr temp1 = null;
	NodePtr temp2;
	NodePtr temp3;
	while (node != null){
			if (node->value == value) {
				temp1 = node;
			}

			if (node->value <= value) {
				node = node->right;
			} else {
				node = node->left;
			}
	}

	if (temp1 == null) {
		return;
	}

	temp3 = temp1;
	int temp3_original_color = temp3->color;
	if (temp1->left == null) {
		temp2 = temp1->right;
		rbTransplant(temp1, temp1->right);
	} else if (temp1->right == null) {
		temp2 = temp1->left;
		rbTransplant(temp1, temp1->left);
	} else {
		temp3 = getMinValueNode(temp1->right);
		temp3_original_color = temp3->color;
		temp2 = temp3->right;
		if (temp3->parent == temp1) {
			temp2->parent = temp3;
		} else {
			rbTransplant(temp3, temp3->right);
			temp3->right = temp1->right;
			temp3->right->parent = temp3;
		}

		rbTransplant(temp1, temp3);
		temp3->left = temp1->left;
		temp3->left->parent = temp3;
		temp3->color = temp1->color;
	}
	delete temp1;
	if (temp3_original_color == BLACK){
		fixDeleteNode(temp2);
	}
} 

/**
 * This function takes in a value that we want to delete from the tree and calls the delete helper function.
 * @param value
 */
void RedBlackTree::deleteNode(int value){
	deleteNodeHelper(this->root, value);
}


/**
 * This function takes in an integer and sets it as the number of readerThreads we can have running concurrently. 
 * @param num_Readers
 */
void RedBlackTree::setNumReaders(int num_Readers){
	this->num_Readers = num_Readers;
}

/**
 * This function takes in an integer and sets it as the number of writerThreads we can have running concurrently.
 * @param num_Writers
 */
void RedBlackTree::setNumWriters(int num_Writers){
	this->num_Writers = num_Writers;
}

/**
 * This function returns the number of concurrent ReaderThreads we can have. 
 * @return
 */
int RedBlackTree::getNumReaders(){
	return this->num_Readers;
}

/**
 * This function returns the number of concurrent WriterThreads we can have. 
 * @return
 */
int RedBlackTree::getNumWriters(){
	return this->num_Writers;
}
