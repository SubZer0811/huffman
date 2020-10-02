huffman: obj/huffman.o obj/main.o
	g++ obj/huffman.o obj/main.o -o huffman
	# g++ -c src/huffman.cpp -o obj/huffman.o
	# g++ -c src/main.cpp -o obj/main.o

obj/huffman.o: src/huffman.cpp
	g++ -c src/huffman.cpp -o obj/huffman.o
obj/main.o: src/main.cpp
	g++ -c src/main.cpp -o obj/main.o