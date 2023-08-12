all: bin assembler banana

assembler: bin/assembler.o bin/vm.o bin/instructions.o bin/byteutils.o bin/fileutils.o
	g++ -o assembler bin/assembler.o  bin/vm.o bin/instructions.o bin/byteutils.o bin/fileutils.o -std=c++17

banana: bin/banana.o bin/scanner.o bin/parser.o bin/ast.o bin/instructions.o bin/vm.o bin/byteutils.o bin/fileutils.o
	g++ -o banana bin/banana.o bin/scanner.o bin/parser.o bin/ast.o bin/instructions.o bin/vm.o bin/byteutils.o bin/fileutils.o -std=c++17

bin/banana.o: src/banana.cpp lib/scanner.h lib/parser.h lib/ast.h lib/fileutils.h
	g++ -c src/banana.cpp -o bin/banana.o -std=c++17

bin/assembler.o: src/assembler.cpp lib/vm.h lib/instructions.h lib/byteutils.h lib/fileutils.h
	g++ -c src/assembler.cpp -o bin/assembler.o -std=c++17

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

bin/parser.o: lib/parser.cpp lib/parser.h
	g++ -c lib/parser.cpp -o bin/parser.o -std=c++17

bin:
	mkdir bin