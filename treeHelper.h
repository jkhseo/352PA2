#ifndef TREEHELPER_H
#define TREEHELPER_H

#include <string>
using namespace std;


void tokenizeInputs(string line, const char* delim, vector<string> &tokens);
void processInvocations(string line, RedBlackTree* rbt);
int descToValue(string desc);
int numOfReadersWriters(string desc);
int descToColor(string desc);
node* makeNode(string desc);
node* constructRBTFromPreOrder(vector<string> &preOrderVector);
RedBlackTree* parseInput();



#endif