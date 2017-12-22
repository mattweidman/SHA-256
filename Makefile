make compile:
	g++ -c constantgen.cpp
	g++ -c sha256.cpp
	g++ -o sha256 constantgen.o sha256.o

make comprun:
	make compile && ./sha256
