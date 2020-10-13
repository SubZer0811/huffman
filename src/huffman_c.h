#include<iostream>
#include<fstream>
#include<bits/stdc++.h>

using namespace std;

class huffman_c{

	private:
		map<char, string> encoding_table;
        string uncomp_file, comp_file;
	
	public:
		void generate_encoding ();
		void show_encoding ();
		void save_compressed_file ();
		void uncompress_file ();
        string convertToBinary (unsigned int, int);
        huffman_c (string, string, bool);
		huffman_c ();

};