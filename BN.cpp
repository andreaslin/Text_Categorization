#include <iostream>
#include "BN.h"

void BayesNetwork::buildNetwork() {
	double p = 0, n = 0;
	for( unsigned int i = 0; i < examples.size(); i += 1 ) {
		if ( train_label[examples[i]] == 1 ) {
			p += 1;
		} else {
			n += 1;
		}
	}
	h1 = p/(p+n);
	h2 = n/(p+n);
	for( unsigned int wordId = 0; wordId < 3566; wordId += 1 ) {
		attributes[wordId] = new Child();
		for( unsigned int i = 0; i < 2; i += 1 ) {
			for( unsigned int j = 1; j <= 2; j += 1 ) {
				double nd = get_nd( wordId, i, j );
				if ( i == 0 ) {
					attributes[wordId]->exist[j-1] = nd;
				} else {
					attributes[wordId]->not_exist[j-1] = nd;
				}
			} 
		}
	}
}

double BayesNetwork::get_nd( unsigned int wordId, unsigned int value, unsigned int label ) {
	unsigned int n = 0, d = 0;
	for( unsigned int i = 0; i < 1061; i += 1 ) {
		if ( train_data[i][wordId] == value && train_label[i] == label ) n += 1;
		if ( train_data[i][wordId] == value ) d += 1;
	}
	return (double)n/(double)d;
}
