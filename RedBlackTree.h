#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H


#include<queue>
using namespace std;

struct Node{
	int value;
	int color;
	Node *left;
	Node *right;
	Node *parent;

	explicit Node(int);
	explicit Node();
		
};

typedef Node *NodePtr;


class RedBlackTree{
	private:
		NodePtr  root;
		NodePtr  null;
		int num_Readers;
		int num_Writers;
		queue<int> searchQueue;
		queue<string> modifyQueue;
	protected:
		void fixInsertNode(NodePtr  );
		void rotateLeft(NodePtr  );
		void rotateRight(NodePtr  );
		void fixDeleteNode(NodePtr  );
		void deleteNodeHelper(NodePtr  , int);
		Node* getMinValueNode(NodePtr  );
		Node* getMaxValueNode(NodePtr  );
		void inOrderTraversal(NodePtr  );
		void preOrderTraversal(NodePtr  );
		Node* getNode(NodePtr  , int);
		void rbTransplant(NodePtr   , NodePtr  );
		void printHelper(NodePtr  , string , bool );

	public:
		void recreateTree(ofstream &);
		Node* getNullNode();
		void addNullLeftChild(NodePtr  );
		void addNullRightChild(NodePtr  );
		void prettyPrint();
		bool searchNode(int);
		void setRoot(NodePtr  );
		RedBlackTree();
		void insertNode(int);
		void deleteNode(int);
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
		int popSearchInvocation();
		string popModifyInvocation();
		bool searchQueueEmpty();
		bool modifyQueueEmpty();
		int getModifyQueueSize();
		int getSearchQueueSize();
};

void setNodeColor(NodePtr  , int);
int getNodeColor(NodePtr  );
#endif 