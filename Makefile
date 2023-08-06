all:
	g++ src/main.cpp src/vm.cpp src/instructions.cpp src/byteutils.cpp -o main -std=c++17