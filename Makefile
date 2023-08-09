all:
	g++ src/assembler.cpp src/vm.cpp src/instructions.cpp src/byteutils.cpp src/fileutils.cpp -o assembler -std=c++17
	g++ src/main.cpp src/vm.cpp src/instructions.cpp src/byteutils.cpp src/fileutils.cpp -o main -std=c++17
	g++ src/compiler.cpp src/ast.cpp src/instructions.cpp src/byteutils.cpp src/vm.cpp -o compiler -std=c++17

asm: all
	for i in examples/*.asm; do ./assembler $$i $$i.obj; done