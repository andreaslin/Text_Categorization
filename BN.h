#ifndef _BN_H_
#define _BN_H_

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Child {
 public:
	Child() {}
	double exist[2];
	double not_exist[2];
};

class BayesNetwork {
 public:
 BayesNetwork( unsigned int **train_d, unsigned int *train_l, 
               unsigned int **test_d, unsigned int *test_l, vector<unsigned int> &e ) : 
	train_data( train_d ), train_label( train_l ), test_data( test_d ), test_label( test_l ), examples( e ) {
		attributes = new Child*[3566];
		for( unsigned int i = 0; i < 3566; i += 1 ) attributes[i] = NULL;
	}
	double h1, h2;
	Child **attributes;
	vector<unsigned int> examples;
	unsigned int **test_data;	// test data
	unsigned int *test_label;	// test label
	unsigned int **train_data;	// train data
	unsigned int *train_label;	// train label	

	void buildNetwork();
	double get_nd( unsigned int wordId, unsigned int value, unsigned int label );
};

#endif
