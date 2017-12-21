make compile:
	g++ -c sha256.cpp
	g++ -o sha256 sha256.o

make comprun:
	make compile && ./sha256
