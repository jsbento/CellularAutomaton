#ifndef GRAPHICSCLIENT_H
#define GRAPHICSCLIENT_H

#ifndef NULL
#define NULL 0
#endif

//Necessary includes for Windows sockets
#pragma comment(lib, "ws2_32.lib") 
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#include <string>

#define NO_OPTION -1 //The following list of definitions here
#define CA 0         //are used for determining which button  
#define SIZE_1 1     //was pressed on the client
#define SIZE_2 2
#define SIZE_3 3
#define STEP 4
#define RUN 5
#define PAUSE 6
#define RESET 7
#define RANDOM 8
#define LOAD 9
#define QUIT 10

using namespace std;

class GraphicsClient
{
    private:
        string url; //URL to connect to
        int port; //Port number to connect to
        SOCKET sockfd; //From winsock2
        char * intToHex(int); //Converts integers to the required byte format for the server
        int hexToInt(char *, int); //Coverts the bytes in the buffer at a start point
        struct sockaddr_in serv_addr;
        void connectClient(); //Helper method for connecting when the port or url change
        void drawGUI();
    public:
        GraphicsClient(string, int); //Constructor
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
        void drawLine(int, int, int, int); //x1,y1,x2,y2
        void drawString(int, int, string); //x,y,str
        void repaint(); //redraw
        string requestFile(); //Queries the server for a file and returns the path to the selected file
        int* parseMessage(); //Parse the (x, y) coordinates from server message
        int getButtonPressed(int, int); //Returns a code for which button was pressed based on the click at (x,y)
};

class GraphicsClientException 
{
    private:
        string msg;
    public:
        GraphicsClientException(string str): msg(str){};
        string message() {return msg;};
};

#endif