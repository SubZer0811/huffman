#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
#include"huffman_c.h"

using namespace std;

int main (int argc, char *argv[]){

    int i;
	cout<<"Press 1 to compress file\nPress 2 to uncompress file\n";
	cin>>i;
	string uncomp, comp;
	if(i == 1){
		cout<<"Enter path to uncompressed file: ";
		cin>>uncomp;
		cout<<"Enter path to output file: ";
		cin>>comp;
		huffman_c huff(uncomp, comp);
		huff.generate_encoding();
		huff.save_compressed_file();
	}
	else{
		cout<<"Enter path to compressed file: ";
		cin>>uncomp;
		cout<<"Enter path to output file: ";
		cin>>comp;
		huffman_c huff(comp, uncomp);
		huff.uncompress_file();
	}

	return 0;
}