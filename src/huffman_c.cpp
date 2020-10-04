#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
#include"huffman_c.h"

using namespace std;

void huffman_c::generate_encoding (){

	ifstream file;
	file.open(uncomp_file);
	char c;
	
	int count=0, highest=0;				//highest is the max value of count.
	map<char, int> pre_encoding_table;

	while((file.get(c))){
		
		if(pre_encoding_table.find(c) == pre_encoding_table.end()){
			pre_encoding_table[c] = 1;
		}
		else{
			pre_encoding_table[c] += 1;
		}
	}

	for(auto &x : pre_encoding_table){
		// cout<<x.first<<": "<<x.second<<endl;
		if(x.second > highest){
			highest = x.second;
		}
	}
	
	int bits = ceil(log2(highest));
	cout<<highest<<" "<<bits<<endl;
	for(auto &x : pre_encoding_table){
		// cout<<x.first<<": "<<x.second<<endl;
		string code;
		code = convertToBinary(x.second, bits);
		// // cout<<code<<endl;
		encoding_table[x.first] = code;
	}

	file.close();
}

string huffman_c::convertToBinary(unsigned int n, int bits)
{	
	string code;
	int temp = 0;
	while(n > 0){
		temp = n%2;
		code = to_string(temp) + code;
		n=n/2;
	}
	cout<<code.length()<<endl;
	while(code.length() < bits){
		code = to_string(0) + code;
	}
	return code;
}

void huffman_c::show_encoding (){
	for(auto &x : encoding_table){
		cout<<x.first<<": "<<x.second<<endl;
	}
}

void huffman_c::save_compressed_file (){

	ofstream file;
	file.open(comp_file, ios::out | ios::binary);

	for(auto& x : encoding_table){
		char enc = 0;
		string towrite;

		towrite.push_back(' ');
		towrite.push_back(x.first);
		
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

	// printf("\nleft: %x\n", writeto);
	enc = encoding_table['\0'];
	// cout<<"enc: "<<enc<<endl;
	for(int i=0; i<enc.length(); i++){
		if(count == 8){
			// printf("%x ", writeto);
			file<<writeto;
			count = 0;
			writeto = 0;
		}
		writeto <<= 1;
		writeto |= (int)enc[i] & 1;
		count += 1;
	}
	// printf("\npost null: %x \n", writeto);
	// printf("\ncount = %d", count);
	if(count){
		while(count < 8){
			writeto <<= 1;
			writeto |= 0;
			count += 1;
		}
		file<<writeto;
	}
	// printf("%x\n", writeto);

	in_file.close();
	file.close();
}

void huffman_c::uncompress_file (){
	
	ifstream file;
	file.open(comp_file, ios::binary);

	string msg;

	char c;
	map<string, char> encoding_table;

	file.get(c);

	while(c != '#'){

		string code;
		while(c != ' '){
			code.push_back(c);
			file.get(c);
		}

		file.get(c);
		encoding_table[code] = c;

		file.get(c);
	}

	// cout<<"\n\nEncoding table:\n";
	// for(auto x: encoding_table){
	// 	cout<<x.first<<": "<< x.second<<endl;
	// }
	// cout<<"End Of Encoding table:\n\n";
	
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

huffman_c::huffman_c(string uncomp_file, string comp_file){
	this->uncomp_file = uncomp_file;
	this->comp_file = comp_file;
}