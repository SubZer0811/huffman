#include"huffman.h"
#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

struct comp{
	bool operator()(node* n1, node* n2){
		return n1->count > n2->count;
	}
};

huffman::huffman(string uncomp_file, string comp_file){
	this->uncomp_file = uncomp_file;
	this->comp_file = comp_file;
}

huffman::huffman(){
	
}

void huffman::generate_huffman_tree (){

	ifstream file;
	file.open(uncomp_file);
	map<char, int> histogram;
	priority_queue <node*, vector<node*>, comp> pq;
	node *ptr;
	char c;
	
	while((file.get(c))){
		cout<<c<<endl;
		if(histogram.find(c) == histogram.end()){
			histogram.insert(pair<char, int>(c, 1));
		}
		else{
			histogram[c] += 1;
		}
	}
	histogram['\0'] = 1;
	for(auto& x : histogram){
		// std::cout << x.first << ": " << x.second << std::endl;
		ptr = new node(1, x.first, x.second, NULL, NULL);
		pq.push(ptr);
	}

	node *left, *right;
	
	while(pq.size() > 1){
		
		left = pq.top();
		pq.pop();
		right = pq.top();
		pq.pop();

		ptr = new node(0, '\0', left->count+right->count, left, right);
		pq.push(ptr);
	}

	tree = pq.top();
    generate_encoding(tree);
	
}

void huffman::show_encoding (){
	for(auto& x : encoding_table){
		// cout<<encoding_table[i].first<<" "<<encoding_table[i].second<<endl;
		cout<<x.first<<": "<< x.second<<endl;
	}
}

void huffman::generate_encoding (node *n, int i){

	static char code[10];

	if(n->leaf){
		// printf("%c: ", n->c);
		// printf("%s\n", code);
		// encoding_table.push_back(make_pair(code, n->c));
		encoding_table[n->c] = code;
		return;
	}
	else{
		
		code[i] = '0';
		generate_encoding(n->lptr, i+1);
		code[i] = '1';
		generate_encoding(n->rptr, i+1);
	}
}

void huffman::save_compressed_file (){

	ofstream file;
	file.open(comp_file, ios::out | ios::binary);

	for(auto& x : encoding_table){
		char enc = 0;
		string towrite;

		towrite.push_back(' ');
		towrite.push_back(x.first);
		
		/*
		for(int i=0; i<x.second.length(); i++){
			enc <<= 1;
			// cout<<((int)x.second[i] & 1)<<"  ";
			enc = enc | ((int)x.second[i] & 1);
			// printf("%x  ", enc);
		}
		printf("%x ", enc);
		cout<<towrite<<endl;
		file<<enc<<towrite;
		*/

		file<<x.second<<towrite;


	}

	file<<"#";

	ifstream in_file;
	in_file.open(uncomp_file, ios::binary);
	char c=0;
	unsigned char writeto=0;
	int count = 0, rem = 0;
	string enc;

	while(in_file.get(c)){

		enc = encoding_table[c];
		// cout<<enc<<" ";
		for(int i=0; i<enc.length(); i++){
			if(count == 8){
				printf("%x ", writeto);
				file<<writeto;
				count = 0;
				writeto = 0;
			}
			writeto <<= 1;
			writeto |= (int)enc[i] & 1;
			count += 1;
		}
		// printf("%x\n", writeto);
	}

	printf("\nleft: %x\n", writeto);
	enc = encoding_table['\0'];
	cout<<"enc: "<<enc<<endl;
	for(int i=0; i<enc.length(); i++){
		if(count == 8){
			printf("%x ", writeto);
			file<<writeto;
			count = 0;
			writeto = 0;
		}
		writeto <<= 1;
		writeto |= (int)enc[i] & 1;
		count += 1;
	}
	printf("\npost null: %x \n", writeto);
	printf("\ncount = %d", count);
	if(count){
		while(count < 8){
			writeto <<= 1;
			writeto |= 0;
			count += 1;
		}
		file<<writeto;
	}
	// printf("%x\n", writeto);
	//Do stuff to ensure end is padded

	in_file.close();
	file.close();
}

void huffman::uncompress_file (){
	
	ifstream file;
	file.open(comp_file, ios::binary);

	string msg;

	char c;
	map<string, char> encoding_table;

	file.get(c);

	while(c != '#'){

		// int num = (int)c;
		string code;
		while(c != ' '){
			code.push_back(c);
			file.get(c);
		}

		// file.get(c);
		file.get(c);
		encoding_table[code] = c;

		file.get(c);
	}
	cout<<"\n\nEncoding table:\n";
	for(auto x: encoding_table){
		cout<<x.first<<": "<< x.second<<endl;
	}
	cout<<"EO Encoding table:\n\n";
	int j=1;
	unsigned char p;
	string buf;

	ofstream out_file;
	out_file.open(uncomp_file, ios::out);

	bool end=0;
	while(file.get(c)){

		p = c;

		for(int i=7; i>=0; i--){
			
			if((p>>i) & 1)
				buf.push_back('1');
			else
				buf.push_back('0');

			if(encoding_table.find(buf) != encoding_table.end()){
				if(encoding_table[buf] == '\0'){
					end = 1;
					break;
				}
				char write = encoding_table[buf];
				out_file<<write;
				buf.clear();
			}
			if(end){
				break;
			}
		}
		if(end){
			break;
		}
		j += 1;
	}
}

void huffman::compress_file (){

	generate_huffman_tree();
	show_encoding();
	save_compressed_file();
}