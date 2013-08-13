#include <iostream>
#include "DLT.h"

using namespace std;

/***********
 *** DLT ***
 ***********/

void DLT::buildTree( vector<unsigned int> examples, vector<unsigned int> attributes,
					 treeNode *def, treeNode *cur ) {
	unsigned int m = mode( examples );
	vector<unsigned int> ex1;
	vector<unsigned int> attr1;
	if ( examples.size() == 0 ) { 					// No example to use, use default 
		cur = def;
	} else if ( sameClassification( examples ) ) {
		// create a leaf with examples classification
		treeNode *c = new treeNode( train_label[examples[1]], 0, examples, attributes, NULL );
	} else if ( attributes.size() == 0 ) {
		// create a leaf with mode as classification
		treeNode *c = new treeNode( m, 0, examples, attributes, NULL );
	} else if ( num_node < depth_limit ) {
		unsigned int info_gain;
		unsigned int attri = chooseBestAttribute( examples, attributes, info_gain );
		treeNode *new_root = new treeNode( 0, attri, examples, attributes, def );
		if ( cur == NULL ) {
			cur = new_root;
			// this new node is the root of the tree
			root = new_root;
		} else {
			cur->classification = 0;
			cur->wordId = attri;
		}
		num_node += 1;
		
		for( unsigned int i = 0; i < 2; i += 1 ) {
			ex1.clear();
			// Update example set
			for( unsigned int j = 0; j < examples.size(); j += 1 ) {
				if ( train_data[examples[j]][attri] == i ) {
					ex1.push_back( examples[j] );
				}
			}
			// Update attribute set
			for( unsigned int j = 0; j < attributes.size(); j += 1 ) {
				if ( attributes[j] != attri ) {
					attr1.push_back( attributes[j] );
				}
			}
			treeNode *new_default = new treeNode( m, 0, examples, attributes, def );
			unsigned int new_m = mode( ex1 );
			treeNode *sub_node = new treeNode( new_m, 0, ex1, attr1, new_default );

			// Calculate priority
			// (b)
			if ( !queueType ) {
				attri *= ex1.size();
			}

			queueNode *que = new queueNode( attri, sub_node );
			queue = queue->addElement( que );
			if ( i = 0 ) {
				cur->left = sub_node;
			} else {
				cur->right = sub_node;
			}
		}
		
		if ( queue != NULL ) {
			treeNode *next = queue->attribute;
			if ( num_node < depth_limit ) {
				buildTree( next->examples, next->attributes, next->def, next );
			}
		}
	}
	// The tree hit the depth limit, do nothing
}

bool DLT::sameClassification( vector<unsigned int> &e ) {
	unsigned int c = train_label[e[0]];
	for( unsigned int i = 1; i < e.size(); i += 1 ) {
		if ( train_label[e[i]] != c ) return false;
	}
	return true;
}

unsigned int DLT::mode( vector<unsigned int> &e ) {
	// Calculate mode of examples
	unsigned int count1 = 0, count2 = 0;
	for( unsigned int i = 0; i < e.size(); i += 1 ) {
		if ( train_label[e[i]] == 1 ) {
			count1 += 1;
		} else {
			count2 += 1;
		}
	}
	return ( count1 >= count2 ? 1 : 2 );
}

unsigned int DLT::chooseBestAttribute( vector<unsigned int> &e, vector<unsigned int> &a
									   , unsigned int &ig ) {
	double max = informationGain( e, a[0] );
	ig = a[0];
	for( unsigned int i = 1; i < a.size(); i += 1 ) {
		double cur_ig = informationGain( e, a[i] );
		if ( cur_ig > ig ) {
			max = cur_ig;
			ig = a[i];
		}
	}
	return max;
}

double DLT::informationGain( vector<unsigned int> &e, unsigned int word ) {
	unsigned int p = 0;
	unsigned int n = 0;

	for( unsigned int i = 0; i < e.size(); i += 1 ) {
		if ( train_label[e[i]] == 1 ) {
			p += 1;
		} else {
			n += 1;
		}
	}
	double sum_pn = (double)p + (double)n;
	double a = (double)p/sum_pn;
	double b = (double)n/sum_pn;
	return information( a, b ) - remainder( e, word, p, n );
}

double DLT::information( double a, double b ) {
	if( a == 0 || b == 0 ) return 0;
	double k1 = log(a) / log(2);
	double k2 = log(b) / log(2);
	return ( -a * k1 ) + ( -b * k2 );
}

double DLT::remainder( vector<unsigned int> &e, unsigned int word, unsigned int p, unsigned int n ) {
	double result = 0;
	for( unsigned int i = 0; i < 2; i += 1 ) {
		double pi = 0, ni = 0;
		for( unsigned int j = 0; j < e.size(); j += 1 ) {
			if ( train_data[e[j]][word] == i ) {
				if ( train_label[e[j]] == 1 ) {
					pi += 1;
				} else {
					ni += 1;
				}
			}
		}
		double ki = ( pi + ni ) / ( p + n );
		if ( ki != 0 ) {
			double ai = pi / ( pi + ni );
			double bi = ni / ( pi + ni );
			result += ki * information( ai, bi );
		}
	}
	return result;
}

/**********************
 *** Priority Queue ***
 **********************/

queueNode* queueNode::removeFront() {
	return this->next;
}

queueNode* queueNode::addElement( queueNode* add ) {
	queueNode *start = this;
	queueNode *previous = NULL;
	queueNode *ret = this;

	while( start != NULL ) {
		if ( start->priority < add->priority ) {
			if ( previous != NULL ) {
				previous->next = add;
			} else {
				ret = add;
			}
			add->next = start;
			return ret;
		}
		previous = start;
		start = start->next;
	}
	previous->next = add;
	add->next = NULL;
	return ret;
}

unsigned int DLT::getPrediction( unsigned int e, treeNode *node, bool type ) {
	if ( node->classification != 0 ) return node->classification;
	treeNode *next = NULL;
	bool same = false;
	// Type = true -> train_data, false -> test_data
	if ( type && ( train_data[e][node->wordId] == 0 ) ) same = true;
	if ( !type && ( test_data[e][node->wordId] == 0 ) ) same = true;

	if ( same ) {
		next = node->left;
	} else {
		next = node->right;
	}
	return getPrediction( e, next, type );
}
