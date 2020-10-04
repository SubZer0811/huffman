#ifndef HUFFMAN_H
#define HUFFMAN_H

#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

struct node{
	bool leaf;
	char c;
	int count;
	node *lptr, *rptr;

	node(bool leaf, char c, int count, node *lptr, node *rptr)
		:leaf(leaf), c(c), count(count), lptr(lptr), rptr(rptr){}
};

class huffman_v{
	
	private:
		map<char, string> encoding_table;
		node *tree;
		string uncomp_file, comp_file;

	public:

		//Functions for huffman encoding
		void generate_huffman_tree ();		//This function generates a huffman tree from characters of input file
		void generate_encoding (node *, int = 0);
		void save_compressed_file ();
		void compress_file();
		void show_encoding ();				//This function shows the encoding of each character while encoding

		//Function for huffman decoding
		void uncompress_file();
		
		//Constructors
		huffman_v (string, string);		//first argument: uncompressed file; second argument compressed file
		huffman_v ();			//use this for calling functions manually

};

#endif