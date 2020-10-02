# all: obj/huffman.o obj/main.o
all:
	g++ -c src/huffman.cpp -o obj/huffman.o
	g++ -c src/main.cpp -o obj/main.o
	g++ obj/huffman.o obj/main.o -o huffman

# obj/huffman.o:
# 	g++ -c src/huffman.cpp -o obj/huffman.o
# obj/main.o:
# 	g++ -c src/main.cpp -o obj/main.o