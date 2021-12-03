# coms327p2
Author: Jacob Benton (jsbenton)
Project 2 for COM S 327

Part A and B:

GraphicsClient.h (WINDOWS)
class GraphicsClient
    private variables:
        string url; //URL connecting to
        int port; //Port number
        SOCKET sockfd; //From winsock2
        char * intToHex(int); //Converts int to byte string where only the lower 4 bits of each byte are used
        int hexToInt(int &, int); //Coverts the bytes in the buffer at a start point
        struct sockaddr_in serv_addr; //Used in socket connection
        void connectClient(); //Connects to the server with the current address and port
        void drawGUI(); //Draws the GUI in the right most 200 pixels of the CA
    public methods:
        #define NO_OPTION -1;
        #define CA 0
        #define SIZE_1 1
        #define SIZE_2 2
        #define SIZE_3 3
        #define STEP 4
        #define RUN 5
        #define PAUSE 6
        #define RESET 7
        #define RANDOM 8
        #define LOAD 9
        #define QUIT 10
        GraphicsClient(string, int); //Constructor, opens appropriate connection
        GraphicsClient(const GraphicsClient &); //Copy constructor
        ~GraphicsClient(); //Destructor
        const GraphicsClient & operator=(const GraphicsClient &); //Assignment equals
        void setBackgroundColor(int, int, int); //r,g,b
        void setDrawingColor(int, int, int); //r,g,b
        void clear(); //Set display to background color
        void setPixel(int, int, int, int, int); //x,y,r,g,b
        void drawRectangle(int, int, int, int); //x,y,w,h
        void fillRectangle(int, int, int, int); //x,y,w,h
        void clearRectangle(int, int, int, int); //x,y,w,h - sets to background color
        void drawOval(int, int, int, int); //draws oval inside rectangle x,y,w,h
        void fillOval(int, int, int, int); //fills oval inside rectangle x,y,w,h
        void clearOval(int, int, int, int);
        void drawLine(int, int, int, int); //x1,y1,x2,y2
        void drawString(int, int, string); //x,y,str
        void repaint(); //redraw
        void requestFile(); //Loads a file from the server
        int* parseMessage(); //Parse the (x, y) coordinates from server message
        int getButtonPressed(int, int); //Returns a code for which button was pressed based on the click at (x,y)
class GraphicsClientException 
    private:
        string msg;
    public:
        GraphicsClientException(string str): msg(str){};
        string message() {return msg;};

CellularAutomaton.h
class CellularAutomaton
    private:
        int width; - width of CA
        int height; - height of CA
        int qstate; - quiescent state of CA
        unsigned char *data; - array representing CA data
    public:
        CellularAutomaton(string, int); //Reads file in same format as Project 1C
        CellularAutomaton(const CellularAutomaton &); //Copy constructor (deep copies)
        ~CellularAutomaton(); //Destructor
        int getWidth(); //Returns CA width
        int getHeight(); //Returns CA height
        unsigned char *getData(); //Returns pointer to the data of the CA
        const CellularAutomaton & operator=(const CellularAutomaton &); //Operator equals assignment method
        unsigned char step(unsigned char(*)(Cellular Automaton, int, int)); //Peforms 1 step of the CA
        displayCA(GraphicsClient &); //Displays the CA in the given graphics window
        void randomInit();
        void updateCellOnClick(GraphicsClient &, int, int);
        void readFile(string); //Loads the CA from a file
class CellularAutomaton 
    private:
        string msg;
    public:
        CellularAutomatonException(string str): msg(str){};
        string message() {return msg;};