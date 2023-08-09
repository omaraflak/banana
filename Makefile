all:
	g++ src/main.cpp lib/vm.cpp lib/instructions.cpp lib/byteutils.cpp lib/fileutils.cpp -o main -std=c++17
	g++ src/assembler.cpp lib/vm.cpp lib/instructions.cpp lib/byteutils.cpp lib/fileutils.cpp -o assembler -std=c++17
	g++ src/compiler.cpp lib/ast.cpp lib/instructions.cpp lib/byteutils.cpp lib/vm.cpp -o compiler -std=c++17

asm: all
	for i in examples/*.asm; do ./assembler $$i $$i.obj; done