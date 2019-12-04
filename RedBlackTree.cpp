#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "RedBlackTree.h"

using namespace std;

enum Color {RED, BLACK};

int getNodeColor(NodePtr node){
	if(node == NULL) return BLACK;
	return node->color;
}

void setNodeColor(NodePtr node, int color) {
    if (node == NULL){
        return;
    }
    node->color = color;
}


Node::Node(int value){
	this->value = value;
	color = RED;
	parent = NULL;
	left = NULL;
	right = NULL;
}

Node::Node(){
}

NodePtr RedBlackTree::getNullNode(){
	return this->null;
}

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

void RedBlackTree::prettyPrint() {
	    if (root) {
    		printHelper(this->root, "", true);
	    }
	}

int RedBlackTree::getSearchQueueSize(){
	return this->searchQueue.size();
}
int RedBlackTree::getModifyQueueSize(){
	return this->modifyQueue.size();
}


bool RedBlackTree::searchQueueEmpty(){
	if(this->searchQueue.empty()) return true;
	return false;
}

bool RedBlackTree::modifyQueueEmpty(){
	if(this->modifyQueue.empty()) return true;
	return false;
}


int RedBlackTree::popSearchInvocation(){
	if(this->searchQueueEmpty()) return INT_MIN;
	int value = this->searchQueue.front();
	this->searchQueue.pop();
	return value;
}



string RedBlackTree::popModifyInvocation(){
	string value = this->modifyQueue.front();
	this->modifyQueue.pop();
	return value;
}

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

queue<int> RedBlackTree::getSearchQueue(){
	return this->searchQueue;
}
queue<string> RedBlackTree::getModifyQueue(){
	return this->modifyQueue;
}

void RedBlackTree::addSearchInvocation(int value){
	this->searchQueue.push(value);
}

void RedBlackTree::addModifyInvocation(string next){
	this->modifyQueue.push(next);
}

RedBlackTree::RedBlackTree(){
	null = new Node;
	null->color = BLACK;
	null->left = NULL;
	null->right = NULL;
	root = null;
}

void RedBlackTree::setRoot(NodePtr node){
	this->root = node;
}

NodePtr RedBlackTree::getMinValueNode(NodePtr node){
	NodePtr temp = node;
	while(temp->left != null){
		temp = temp->left;
	}
	return temp;
}

NodePtr RedBlackTree::getMaxValueNode(NodePtr node){
	NodePtr temp = node;
	while(temp->right != null){
		temp = temp->right;
	}
	return temp;
}

void RedBlackTree::addNullLeftChild(NodePtr  node){
	node->left = null;
}

void RedBlackTree::addNullRightChild(NodePtr  node){
	node->right = null;
}

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

void RedBlackTree::fixInsertNode(NodePtr node){
	NodePtr temp;
	while (node->parent->color == RED) {
			if (node->parent == node->parent->parent->right) {
				temp = node->parent->parent->left; // tempncle
				if (temp->color == RED) {
					// case 3.RED
					temp->color = BLACK;
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
				} else {
					if (node == node->parent->left) {
						// case 3.2.2
						node = node->parent;
						rotateRight(node);
					}
					// case 3.2.RED
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					rotateLeft(node->parent->parent);
				}
			} else {
				temp = node->parent->parent->right; // tempncle

				if (temp->color == RED) {
					// mirror case 3.RED
					temp->color = BLACK;
					node->parent->color = BLACK;
					node->parent->parent->color = RED;
					node = node->parent->parent;
				} else {
					if (node == node->parent->right) {
						// mirror case 3.2.2
						node = node->parent;
						rotateLeft(node);
					}
					// mirror case 3.2.RED
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

		// prev is parent of curr
		node->parent = prev;
		if (prev == NULL) {
			root = node;
		} else if (node->value < prev->value) {
			prev->left = node;
		} else {
			prev->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == NULL){
			node->color = BLACK;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == NULL) {
			return;
		}

		// Fix the tree
		fixInsertNode(node);
	}


void RedBlackTree::fixDeleteNode(NodePtr node){
	NodePtr temp;
	while (node != root && node->color == BLACK) {
			if (node == node->parent->left) {
				temp = node->parent->right;
				if (temp->color == RED) {
					// catempe 3.RED
					temp->color = BLACK;
					node->parent->color = RED;
					rotateLeft(node->parent);
					temp = node->parent->right;
				}

				if (temp->left->color == BLACK && temp->right->color == BLACK) {
					// catempe 3.2
					temp->color = RED;
					node = node->parent;
				} else {
					if (temp->right->color == BLACK) {
						// catempe 3.3
						temp->left->color = BLACK;
						temp->color = RED;
						rotateRight(temp);
						temp = node->parent->right;
					}

					// catempe 3.4
					temp->color = node->parent->color;
					node->parent->color = BLACK;
					temp->right->color = BLACK;
					rotateLeft(node->parent);
					node = root;
				}
			} else {
				temp = node->parent->left;
				if (temp->color == RED) {
					// catempe 3.RED
					temp->color = BLACK;
					node->parent->color = RED;
					rotateRight(node->parent);
					temp = node->parent->left;
				}

				if (temp->right->color == BLACK && temp->right->color == BLACK) {
					// case 3.2
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
		cout<<"Couldn't find value in the tree"<<endl;
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

void RedBlackTree::deleteNode(int value){
	deleteNodeHelper(this->root, value);
}
	

void RedBlackTree::inOrderTraversal(NodePtr node) {
    if (node == null)
        return;

    inOrderTraversal(node->left);
    cout << node->value << " " << node->color << endl;
    inOrderTraversal(node->right);
}

void RedBlackTree::inOrder() {
    inOrderTraversal(root);
}

void RedBlackTree::preOrderTraversal(NodePtr node) {
    if (node == null)
        return;

    cout << node->value << " " << node->color << endl;
    preOrderTraversal(node->left);
    preOrderTraversal(node->right);
}

void RedBlackTree::preOrder() {
    preOrderTraversal(root);
}

void RedBlackTree::setNumReaders(int num_Readers){
	this->num_Readers = num_Readers;
}

void RedBlackTree::setNumWriters(int num_Writers){
	this->num_Writers = num_Writers;
}

int RedBlackTree::getNumReaders(){
	return this->num_Readers;
}

int RedBlackTree::getNumWriters(){
	return this->num_Writers;
}
