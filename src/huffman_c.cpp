#include<iostream>
#include<fstream>
#include<bits/stdc++.h>
#include"huffman_c.h"

using namespace std;

/**
 * @brief Generates and encoding table for a given file
 */
void huffman_c::generate_encoding (){

	ifstream file;
	file.open(uncomp_file);
	char c;
	
	int count=0, highest=0;					// highest is the max value of count.
	map<char, int> pre_encoding_table;		// create a mao between character and its count.

	while((file.get(c))){					// read each character in file and append to map.
		
		// printf("%c", c);
		if(pre_encoding_table.find(c) == pre_encoding_table.end()){
			pre_encoding_table[c] = 1;
			count += 1;
		}
		else{
			pre_encoding_table[c] += 1;
		}
	}
	
	int bits = ceil(log2(count)) + 1;		// since this is constant constant huffman coding,
											// number of bits in each encoding needs to be constant.

	// assign an encoding to each character and append to encoding table
	for(auto &x : pre_encoding_table){
		// cout<<x.first<<": "<<x.second<<endl;
		string code;
		code = convertToBinary(count, bits);	
		count -= 1;
		// cout<<code<<endl;
		encoding_table[x.first] = code;
	}

	show_encoding();

	file.close();
}

/**
 * @brief Converts an unsigned integer 'n' to a 'bits' length binary number
 * 
 * @param n Unsigned integer to convert
 * @param bits Length of binary number
 * @return string 'bits' long Binary number
 */
string huffman_c::convertToBinary(unsigned int n, int bits)
{	
	string code;
	int temp = 0;
	while(n > 0){
		temp = n%2;
		code = to_string(temp) + code;
		n=n/2;
	}
	// cout<<code.length()<<endl;
	while(code.length() < bits){
		code = to_string(0) + code;
	}
	return code;
}

/**
 * @brief Print the encoding table to STDOUT
 */
void huffman_c::show_encoding (){
	for(auto &x : encoding_table){
		cout<<x.first<<": "<<x.second<<endl;
	}
}

/**
 * @brief Compress the input file using constant length Huffman encoding.
 */
void huffman_c::save_compressed_file (){

	ofstream file;
	file.open(comp_file, ios::out | ios::binary);

	/* 
	 * Write the Encoding table to the output file. 
	 * Each encoding entry is delimited by a space.
	*/
	for(auto& x : encoding_table){
		char enc = 0;
		string towrite;

		towrite.push_back(' ');
		towrite.push_back(x.first);
		
		file<<x.second<<towrite;
	}

	file<<"#"; 					// The encoding table is delimited with the compressed text by a '#'

	ifstream in_file;
	in_file.open(uncomp_file, ios::binary);
	char c=0;
	unsigned char writeto=0;
	int count = 0, rem = 0;
	string enc;

	/* 
	 * Read each character in the input file, append its encoding to `writeto`.
	 * If the length of `writeto` is 1 byte, then write it to the compressed file.
	*/
	while(in_file.get(c)){		

		enc = encoding_table[c];
		// cout<<enc<<" ";
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
	/**
	 * Write all the left over bits in `writeto` if there exists any.
	 */
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

/**
 * @brief Uncompress the constant length huffman compressed file.
 */
void huffman_c::uncompress_file (){
	
	ifstream file;
	file.open(comp_file, ios::binary);

	char c;
	map<string, char> encoding_table;

	file.get(c);

	/**
	 * Extract the encoding table from the compressed file.
	 */
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
	
	int j=1;
	unsigned char p;
	string buf;

	ofstream out_file;
	out_file.open(uncomp_file, ios::out);

	/**
	 * Decode the encoded text using the above extracted encoding table.
	 */
	bool end=0;
	while(file.get(c)){

		p = c;

		/**
		 * Append one bit at a time and check whether a valid huffman encoding can
		 * be found in the encoding table.
		 */
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

/**
 * @brief Compress or Uncompress using constant length huffman encoding.
 * 
 * @param uncomp_file Path to uncompressed file.
 * @param comp_file Path to compressed file.
 * @param opt set opt to 1 to compress, 0 to uncompress
 */
huffman_c::huffman_c(string uncomp_file, string comp_file, bool opt){
	this->uncomp_file = uncomp_file;
	this->comp_file = comp_file;

	if(opt){
		generate_encoding();
		save_compressed_file();
	}
	else{
		uncompress_file();
	}
}