#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H


#include<queue>
using namespace std;

typedef struct node{
	int value;
	int color;
	node *left;
	node *right;
	node *parent;

	explicit node(int);
		
} node;


class RedBlackTree{
	private:
		node *root;
		int num_Readers;
		int num_Writers;
		queue<int> searchQueue;
		queue<string> modifyQueue;
	protected:
		void fixInsertNode(node *&);
		void rotateLeft(node *&);
		void rotateRight(node *&);
		void fixDeleteNode(node *&);
		node* deleteNodeHelper(node *&, int);
		void deleteNode(int);
		node* getMinValueNode(node *&);
		node* getMaxValueNode(node *&);
		void inOrderTraversal(node *&);
		void preOrderTraversal(node *&);

	public:
		bool searchNode(int);
		void setRoot(node *&);
		RedBlackTree();
		void insertNode(int);
		void inOrder();
		void preOrder();
		void setNumReaders(int);
		void setNumWriters(int);
		int getNumReaders();
		int getNumWriters();
		queue<int> getSearchQueue();
		queue<string> getModifyQueue();
		void addSearchInvocation(int);
		void addModifyInvocation(string);
};

void setNodeColor(node *&, int);
int getNodeColor(node *&);
#endif 