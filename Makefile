all: huffman_v huffman_c

huffman_v: obj/huffman_v.o obj/main_v.o
	g++ obj/huffman_v.o obj/main_v.o -o huffman_v
obj/huffman_v.o: src/huffman_v.cpp
	g++ -c src/huffman_v.cpp -o obj/huffman_v.o
obj/main_v.o: src/main_v.cpp
	g++ -c src/main_v.cpp -o obj/main_v.o

huffman_c: obj/huffman_c.o obj/main_c.o
	g++ obj/huffman_c.o obj/main_c.o -o huffman_c
obj/huffman_c.o: src/huffman_c.cpp
	g++ -c src/huffman_c.cpp -o obj/huffman_c.o
obj/main_c.o: src/main_c.cpp
	g++ -c src/main_c.cpp -o obj/main_c.o