all:
	g++ src/assembler.cpp src/vm.cpp src/instructions.cpp src/byteutils.cpp -o assembler -std=c++17
	g++ src/main.cpp src/vm.cpp src/instructions.cpp src/byteutils.cpp -o main -std=c++17

asm: all
	for i in examples/*.asm; do ./assembler $$i $$i.obj; done