make compile:
	g++ -c constantgen.cpp sha256.cpp
	g++ -o sha256 constantgen.o sha256.o
