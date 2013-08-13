#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>					// exit
#include "DLT.h"					// Decision learning tree algorithm
#include <vector>					// vector
using namespace std;				// direct access to std

void readDataInput( ifstream *in, unsigned int **d ) {
	int in1, in2;
	unsigned int i, j;
	for ( ;; ) {
		*in >> in1;					// read an integer
		*in >> in2;					// read another integer
		i = (unsigned int)in1 - 1;
		j = (unsigned int)in2 - 1;
		d[i][j] = 1;
		if ( in->fail() ) break;			// failed ?
	} // for
}

void readLabelInput( ifstream *in, unsigned int *v ) {
	int in1;
	for ( unsigned int i = 0;; i += 1 ) {
		*in >> in1;					// read an integer
		v[i] = in1;
		if ( in->fail() ) break;			// failed ?
	} // for
}

bool convert( int &val, char *buffer ) {		// convert C string to integer
	std::stringstream ss( buffer );			// connect stream and buffer
	ss >> dec >> val;					// convert integer from buffer
	return ! ss.fail() &&				// conversion successful ?
		// characters after conversion all blank ?
		string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
} // convert

void usage( char *argv[] ) {
	cerr << "Usage: " << argv[0]
	     << "Train-file Train-label Test-file Test-label" << endl;
	exit( EXIT_FAILURE );				// TERMINATE
} // usage

int main( int argc, char *argv[] ) {
	ifstream *train_file;				// train file
	ifstream *train_label_file;			// train label file
	ifstream *test_file;				// test file
	ifstream *test_label_file;			// test label file

	ofstream out_train_acc0("train_acc0.txt");
	ofstream out_train_acc1("train_acc1.txt");
	ofstream out_test_acc0("test_acc0.txt");
	ofstream out_test_acc1("test_acc1.txt");

	unsigned int **test_data;	// test data
	unsigned int *test_label;	// test label
	unsigned int **train_data;	// train data
	unsigned int *train_label;	// train label

	switch ( argc ) {
	case 5:
		test_label_file = new ifstream( argv[4] );
		if ( test_label_file->fail() ) {			// open failed ?
			cerr << "Error! Could not open output file \"" << argv[4] << "\"" << endl;
			usage( argv );
		} // if

		test_file = new ifstream( argv[3] );
		if ( test_file->fail() ) {					// open failed ?
			cerr << "Error! Could not open input file \"" << argv[3] << "\"" << endl;
			usage( argv );
		} // if

		train_label_file = new ifstream( argv[2] );
		if ( train_label_file->fail() ) {			// open failed ?
			cerr << "Error! Could not open input file \"" << argv[2] << "\"" << endl;
			usage( argv );
		} // if

		train_file = new ifstream( argv[1] );
		if ( train_file->fail() ) {					// open failed ?
			cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
			usage( argv );
		} // if
		break;
	default:						// wrong number of options
		usage( argv );
	} // switch

	// Read from file
	train_data = new unsigned int*[1061];
	train_label = new unsigned int[1061];
	test_data = new unsigned int*[707];
	test_label = new unsigned int[707];

	// Initialize training and testing data
	for( unsigned int i = 0; i < 1061; i += 1 ) {
		train_label[i] = 0;
		unsigned int *row;
		row = new unsigned int[3566];
		train_data[i] = row;
		for( unsigned int j = 0; j < 3566; j += 1 ) train_data[i][j] = 0;
	}

	for( unsigned int i = 0; i < 707; i += 1 ) {
		test_label[i] = 0;
		unsigned int *row;
		row = new unsigned int[3566];
		test_data[i] = row;
		for( unsigned int j = 0; j < 3566; j += 1 ) test_data[i][j] = 0;
	}
	
	readDataInput( train_file, train_data );
	readLabelInput( train_label_file, train_label );
	readDataInput( test_file, test_data );
	readLabelInput( test_label_file, test_label );

	// Initialize examples and attributes
	vector< unsigned int > examples;
	vector< unsigned int > attributes;
	for( unsigned int i = 0; i < 1061; i += 1 ) examples.push_back(i);
	for( unsigned int i = 0; i < 3566; i += 1 ) attributes.push_back(i);

	// Build decision tree
	// queue type: true = (a), false = (b)

	for( unsigned int type = 0; type < 2; type += 1 ) {
		DLT dlt( train_data, train_label, test_data, test_label, ( type == 0 ) );
		dlt.num_node = 0;
		dlt.depth_limit = 1;	
		for( unsigned int i = 1; i <= 100; i += 1 ) {
			if ( dlt.queue == NULL ) { // Never build a tree before
				unsigned int train_m = dlt.mode( examples );			
				treeNode *def = new treeNode( train_m, 0, examples, attributes, NULL );
				dlt.buildTree( examples, attributes, def, dlt.root );	
			} else {					// Use tree node from priority queue
				dlt.depth_limit = i;
				treeNode *new_root = dlt.queue->attribute;
				dlt.buildTree( new_root->examples, new_root->attributes, new_root->def, new_root );
			}
			
			unsigned int train_correct = 0, test_correct = 0;
			for( unsigned int j = 0; j < 1061; j += 1 )
				if ( dlt.getPrediction( j, dlt.root, true ) == train_label[j] ) train_correct += 1;
			for( unsigned int j = 0; j < 707; j += 1 )
				if ( dlt.getPrediction( j, dlt.root, false ) == test_label[j] ) test_correct += 1;
			
			double train_percent = (double)train_correct/(double)1061;
			double test_percent = (double)test_correct/(double)707;
			
			if ( type == 0 ) {
				out_train_acc0 << train_percent << endl;
				out_test_acc0 << test_percent << endl;
			} else {
				out_train_acc1 << train_percent << endl;
				out_test_acc1 << test_percent << endl;
			}
		}		
	}
	// Close all files
	out_train_acc0.close();
	out_test_acc0.close();
	out_train_acc1.close();
	out_test_acc1.close();

	
} // main
