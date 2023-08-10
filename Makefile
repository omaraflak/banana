all: main assembler compiler test asm

main: bin/main.o bin/vm.o bin/instructions.o bin/byteutils.o bin/fileutils.o
	g++ -o main bin/main.o bin/vm.o bin/instructions.o bin/byteutils.o bin/fileutils.o -std=c++17

assember: bin/assembler.o bin/vm.o bin/instructions.o bin/byteutils.o bin/fileutils.o
	g++ -o assembler bin/assembler.o  bin/vm.o bin/instructions.o bin/byteutils.o bin/fileutils.o -std=c++17

compiler: bin/compiler.o bin/ast.o bin/vm.o bin/instructions.o bin/byteutils.o
	g++ -o compiler bin/compiler.o bin/ast.o bin/vm.o bin/instructions.o bin/byteutils.o -std=c++17

test: bin/test.o bin/scanner.o bin/fileutils.o
	g++ -o test bin/test.o bin/scanner.o bin/fileutils.o -std=c++17

bin/test.o: src/test.cpp lib/scanner.h lib/fileutils.h
	g++ -c src/test.cpp -o bin/test.o -std=c++17

bin/compiler.o: src/compiler.cpp lib/ast.h lib/vm.h lib/instructions.h lib/byteutils.h
	g++ -c src/compiler.cpp -o bin/compiler.o -std=c++17

bin/assembler.o: src/assember.cpp lib/vm.h lib/instructions.h lib/byteutils.h lib/fileutils.h
	g++ -c src/assember.cpp -o bin/assember.o -std=c++17

bin/main.o: src/main.cpp lib/vm.h lib/instructions.h lib/byteutils.h lib/fileutils.h
	g++ -c src/main.cpp -o bin/main.o -std=c++17

bin/instructions.o: lib/instructions.cpp lib/instructions.h
	g++ -c lib/instructions.cpp -o bin/instructions.o -std=c++17

bin/byteutils.o: lib/byteutils.cpp lib/byteutils.h
	g++ -c lib/byteutils.cpp -o bin/byteutils.o -std=c++17

bin/fileutils.o: lib/fileutils.cpp lib/fileutils.h
	g++ -c lib/fileutils.cpp -o bin/fileutils.o -std=c++17

bin/vm.o: lib/vm.cpp lib/vm.h lib/instructions.cpp lib/instructions.h
	g++ -c lib/vm.cpp -o bin/vm.o -std=c++17

bin/ast.o: lib/ast.cpp lib/ast.h
	g++ -c lib/ast.cpp -o bin/ast.o -std=c++17

bin/scanner.o: lib/scanner.cpp lib/scanner.h
	g++ -c lib/scanner.cpp -o bin/scanner.o -std=c++17

asm: assembler
	for i in examples/*.asm; do ./assembler $$i $$i.obj; done