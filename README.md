# CellularAutomaton
This is a project implementing Conway's Game of Life using cellular automata.
It uses sockets to communicate with a graphics server for displaying the results
as well as handling the graphical user interface.

The GraphicsServerV2.jar file was provided by James Lathrop of Iowa State University;
This was originally a class project.

To run the code:
1. Navigate to the repository folder in a command window.
2. Run 'git pull' to update the repository.
3. Run 'make'. The makefile will create an exectuable called 'automaton'.
4. Open in another command window and navigate to the repository folder.
5. Type 'java -jar GraphicsServerV2.jar <port> <flag>' in the second window.
	<port> - The port you want to run the server on
	<flag> - optional, the only thing that can go here is 'debug' where the server will output
			 the bytes it recieves.
6. In the original command window, type 'automaton' to run the program.