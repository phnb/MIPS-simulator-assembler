simulator: assembler.o simulator.o
	g++ -o simulator assembler.o simulator.o
assembler.o: assembler.cpp assembler.h
	g++ -c assembler.cpp
simulator.o: simulator.cpp
	g++ -c simulator.cpp
clean:
	rm -rf simulator assembler.o simulator.o
