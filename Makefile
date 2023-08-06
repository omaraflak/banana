all:
	g++ src/assembler.cpp src/instructions.cpp src/byteutils.cpp -o assembler -std=c++17
	g++ src/main.cpp src/vm.cpp src/instructions.cpp src/byteutils.cpp -o main -std=c++17