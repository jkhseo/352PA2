/**
 * @author Kyung Seo
 * @date 12/06/2019
 * treeHelper.h is the header file for treeHelper.cpp
 */
#ifndef TREEHELPER_H
#define TREEHELPER_H

#include <string>
using namespace std;


void tokenizeInputs(string line, const char* delim, vector<string> &tokens);
void processInvocations(string line, RedBlackTree* rbt);
int descToValue(string desc);
int numOfReadersWriters(string desc);
int descToColor(string desc);
Node* makeNode(string desc);
Node* constructRBTFromPreOrder(vector<string> &preOrderVector, int index, RedBlackTree *rbt, vector<bool> &used);
RedBlackTree* parseInput(ifstream &infile);



#endif