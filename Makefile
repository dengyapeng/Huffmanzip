# target:reps

objects = Huffman.o main.o

#main.o : main.cpp
#	g++ -c -Wall main.cpp

edit : $(objects)
	g++   $(objects) -Wall -std=c++11 -o test 

huffman.o: Huffman.cpp Huffman.h
	g++ -std=c++11 -c -Wall Huffman.cpp 
 
main.o: main.cpp
	g++ -std=c++11 -c -Wall main.cpp
clean:
	rm -rf main.o Huffman.o test
