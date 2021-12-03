all: automaton clean

automaton: casimulator.o GraphicsClient.o	CellularAutomaton.o
	g++ -g -o automaton casimulator.o GraphicsClient.o CellularAutomaton.o -lws2_32

main.o: casimulator.cpp GraphicsClient.h CellularAutomaton.h
	g++ -g -c casimulator.cpp
GraphicsClient.o: GraphicsClient.cpp GraphicsClient.h
	g++ -g -c GraphicsClient.cpp -lws2_32
CellularAutomaton.o: CellularAutomaton.cpp CellularAutomaton.h GraphicsClient.h
	g++ -g -c CellularAutomaton.cpp

clean:
	del -f *.o *~