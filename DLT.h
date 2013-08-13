#ifndef _DLT_H_
#define _DLT_H_

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class queueNode;				/* Forward declaration */
class treeNode;				/* Forward declaration */
extern queueNode *priority_queue;	/* priority queue */
extern treeNode *decision_tree;	/* decision tree */

/* Priority queue (single linked list) */

class queueNode {
 public:
 queueNode( unsigned int p, treeNode *t ) : priority( p ), attribute( t ) {}
	~queueNode() {}
	treeNode *attribute;
	unsigned int priority;
	queueNode *next;

	queueNode* removeFront();
	queueNode* addElement( queueNode* add );
};

/* Tree node */
class treeNode {
 public:
 treeNode( unsigned int c, unsigned int attr, vector<unsigned int> e, 
		   vector<unsigned int> a, treeNode *d ) :
	classification( c ), attribute( attr ), examples( e ), attributes( a ), def( d ) {}

	unsigned int classification; /* 0 - no classification, with word feature. 
									1 and 2 represent classification */
	unsigned int wordId;
	unsigned int attribute;
	treeNode *def;
	vector<unsigned int> examples; 	/* Saved for later used */
	vector<unsigned int> attributes;	/* Saved for later used */
	treeNode *left;			/* left of tree */
	treeNode *right;			/* right of tree */
};

/* Decision Learning Tree algorithm (binary tree) */
class DLT {
 public:
 DLT( unsigned int **train_d, unsigned int *train_l, 
	  unsigned int **test_d, unsigned int *test_l, bool qt ) : 
	train_data( train_d ), train_label( train_l ), test_data( test_d ), test_label( test_l ) {
		/* Initialize priority queue */
		queue = new queueNode( 0, NULL );
		queueType = qt;
		root = NULL;
	}
	
	void buildTree( vector<unsigned int> examples, vector<unsigned int> attributes, 
					treeNode *def, treeNode *cur );
	bool sameClassification( vector<unsigned int> &e );
	unsigned int mode( vector<unsigned int> &e );
	unsigned int chooseBestAttribute( vector<unsigned int> &e, vector<unsigned int> &a,
									  unsigned int &ig );
	double informationGain( vector<unsigned int> &e, unsigned int word );
	double information( double a, double b );
	double remainder( vector<unsigned int> &e, unsigned int word, unsigned int p, unsigned int n );
	unsigned int getPrediction( unsigned int e, treeNode *node, bool type );
	
	unsigned int **test_data;	// test data
	unsigned int *test_label;	// test label
	unsigned int **train_data;	// train data
	unsigned int *train_label;	// train label

	treeNode *root;
	unsigned int depth_limit;
	unsigned int num_node;
	queueNode *queue;
	bool queueType;
	
};

#endif
