#include "huffman.h"
#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

int main (){
	
	int i;
	cout<<"Press 1 to compress file\nPress 2 to uncompress file\n";
	cin>>i;
	string uncomp, comp;
	if(i == 1){
		cout<<"Enter path to uncompressed file: ";
		cin>>uncomp;
		cout<<"Enter path to output file: ";
		cin>>comp;
		huffman huff(uncomp, comp);
		huff.compress_file();
	}
	else{
		cout<<"Enter path to compressed file: ";
		cin>>uncomp;
		cout<<"Enter path to output file: ";
		cin>>comp;
		huffman huff(comp, uncomp);
		huff.uncompress_file();
	}

	return 0;
}