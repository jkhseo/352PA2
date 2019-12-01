#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include "RedBlackTree.h"

using namespace std;

enum Color {RED, BLACK, DOUBLE_BLACK};


int getNodeColor(node *&node){
	if(node == NULL) return BLACK;
	return node->color;
}

void setNodeColor(node *&node, int color) {
    if (node == NULL){
        return;
    }
    node->color = color;
}


node::node(int value){
	this->value = value;
	color = RED;
	parent = NULL;
	left = NULL;
	right = NULL;
}

bool RedBlackTree::searchNode(int value){
	struct node* node = root;
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
	root = NULL;
}

void RedBlackTree::setRoot(node *&node){
	this->root = node;
}

node* RedBlackTree::getMinValueNode(node *&node){
	struct node* temp = node;
	while(temp->left != NULL){
		temp = temp->left;
	}
	return temp;
}

node* RedBlackTree::getMaxValueNode(node *&node){
	struct node* temp = node;
	while(temp->right != NULL){
		temp = temp->right;
	}
	return temp;
}

void RedBlackTree::rotateLeft(node *&node){
	struct node* rChild = node->right;
	node->right = rChild->left;

	if(node->right != NULL) node->right->parent = node;

	rChild->parent = node->parent;

	if(node->parent == NULL){
		root = rChild;
	}
	else if(node == node->parent->left){
		node->parent->left = rChild;
	}
	else{
		node->parent->right = rChild;
	}

	rChild->left = node;
	node->parent = rChild;
}

void RedBlackTree::rotateRight(node *&node){
	struct node* lChild = node->left;

	if(node->left != NULL) node->left->parent = node;

	lChild->parent = node->parent;

	if(node->parent == NULL){
		root = lChild;
	}
	else if(node == node->parent->left){
		node->parent->left = lChild;
	}
	else{
		node->parent->right = lChild;
	}

	lChild->left = node;
	node->parent = lChild;
}



void RedBlackTree::insertNode(int value){
	node* node = new struct node(value);
	if(root == NULL){
		root = node;
	}
	else{
		struct node* curr = root;
		struct node* prev = NULL;
		while(curr != NULL){
			prev = curr;
			if(value > curr->value){
				curr = curr->right;
			}
			else{
				curr = curr->left;
			}
		}
		if(value > prev->value){
			prev->right = node;
		}
		else{
			prev->left = node;
		}
		node->parent = prev;
	}
	fixInsertNode(node);
}

void RedBlackTree::fixInsertNode(node *&node){
	struct node* parent = NULL;
	struct node* gparent = NULL;
	while(node != root && getNodeColor(node) == RED && getNodeColor(node->parent) == RED){
		parent = node->parent;
		gparent = parent->parent;
		if(parent == gparent->left){
			struct node* aunt = gparent->right;
			if(getNodeColor(aunt) == RED){
				setNodeColor(aunt, BLACK);
				setNodeColor(parent, BLACK);
				setNodeColor(gparent, RED);
				node = gparent;
			}
			else{
				if(node == parent->right){
					rotateLeft(parent);
					node = parent;
					parent = node->parent;
				}
				rotateRight(gparent);
				int tempColor = getNodeColor(parent);
				setNodeColor(parent, getNodeColor(gparent));
				setNodeColor(gparent, tempColor);
				node = parent;
			}
		}
		else{
			struct node* aunt = gparent->left;
			if(getNodeColor(aunt) == RED){
				setNodeColor(aunt, BLACK);
				setNodeColor(parent, BLACK);
				setNodeColor(gparent, RED);
				node = gparent;
			}
			else{
				if(node == parent->left){
					rotateRight(parent);
					node = parent;
					parent = node->parent;
				}
				rotateLeft(gparent);
				int tempColor = getNodeColor(parent);
				setNodeColor(parent, getNodeColor(gparent));
				setNodeColor(gparent, tempColor);
				node = parent;
			}
		}

	}
	setNodeColor(root, BLACK);

}

void RedBlackTree::fixDeleteNode(node *&node){
	if(node == NULL) return;
	if(node == root){
		root = NULL;
		return;
	}
	if (getNodeColor(node) == RED || getNodeColor(node->left) == RED || getNodeColor(node->right) == RED) {
        struct node* child = node->left != NULL ? node->left : node->right;

        if (node == node->parent->left) {
            node->parent->left = child;
            if (child != NULL)
                child->parent = node->parent;
            setNodeColor(child, BLACK);
            delete (node);
        } else {
            node->parent->right = child;
            if (child != NULL)
                child->parent = node->parent;
            setNodeColor(child, BLACK);
            delete (node);
        }
    } else {
        struct node* sibling = NULL;
        struct node* parent = NULL;
        struct node* ptr = node;
        setNodeColor(ptr, DOUBLE_BLACK);
        while (ptr != root && getNodeColor(ptr) == DOUBLE_BLACK) {
            parent = ptr->parent;
            if (ptr == parent->left) {
                sibling = parent->right;
                if (getNodeColor(sibling) == RED) {
                    setNodeColor(sibling, BLACK);
                    setNodeColor(parent, RED);
                    rotateLeft(parent);
                } else {
                    if (getNodeColor(sibling->left) == BLACK && getNodeColor(sibling->right) == BLACK) {
                        setNodeColor(sibling, RED);
                        if(getNodeColor(parent) == RED)
                            setNodeColor(parent, BLACK);
                        else
                            setNodeColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    } else {
                        if (getNodeColor(sibling->right) == BLACK) {
                            setNodeColor(sibling->left, BLACK);
                            setNodeColor(sibling, RED);
                            rotateRight(sibling);
                            sibling = parent->right;
                        }
                        setNodeColor(sibling, parent->color);
                        setNodeColor(parent, BLACK);
                        setNodeColor(sibling->right, BLACK);
                        rotateLeft(parent);
                        break;
                    }
                }
            } else {
                sibling = parent->left;
                if (getNodeColor(sibling) == RED) {
                    setNodeColor(sibling, BLACK);
                    setNodeColor(parent, RED);
                    rotateRight(parent);
                } else {
                    if (getNodeColor(sibling->left) == BLACK && getNodeColor(sibling->right) == BLACK) {
                        setNodeColor(sibling, RED);
                        if (getNodeColor(parent) == RED)
                            setNodeColor(parent, BLACK);
                        else
                            setNodeColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    } else {
                        if (getNodeColor(sibling->left) == BLACK) {
                            setNodeColor(sibling->right, BLACK);
                            setNodeColor(sibling, RED);
                            rotateLeft(sibling);
                            sibling = parent->left;
                        }
                        setNodeColor(sibling, parent->color);
                        setNodeColor(parent, BLACK);
                        setNodeColor(sibling->left, BLACK);
                        rotateRight(parent);
                        break;
                    }
                }
            }
        }
        if (node == node->parent->left)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;
        delete(node);
        setNodeColor(root, BLACK);
    }

}

node* RedBlackTree::deleteNodeHelper(node *& node, int value){
	if(node == NULL){
		return node;
	}
	else if(value < node->value){
		return deleteNodeHelper(node->left, value);
	}
	else if(value > node->value){
		return deleteNodeHelper(node->right, value);
	}
	else if(node->right == NULL || root->left == NULL){
		return node;
	}
	struct node* replacement = getMinValueNode(root->right);
	node->value = replacement->value;
	return deleteNodeHelper(node->right, replacement->value);
	{
		
	};
	fixDeleteNode(node);

}

void RedBlackTree::deleteNode(int value){
	struct node* node = deleteNodeHelper(root, value);
	fixDeleteNode(node);

}

void RedBlackTree::inOrderTraversal(node *&ptr) {
    if (ptr == NULL)
        return;

    inOrderTraversal(ptr->left);
    cout << ptr->value << " " << ptr->color << endl;
    inOrderTraversal(ptr->right);
}

void RedBlackTree::inOrder() {
    inOrderTraversal(root);
}

void RedBlackTree::preOrderTraversal(node *&ptr) {
    if (ptr == NULL)
        return;

    cout << ptr->value << " " << ptr->color << endl;
    preOrderTraversal(ptr->left);
    preOrderTraversal(ptr->right);
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
