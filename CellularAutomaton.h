#ifndef CELLULARAUTOMATON_H
#define CELLULARAUTOMATON_H


#ifndef NULL
#define NULL 0
#endif

#include "GraphicsClient.h"

using namespace std;

class CellularAutomaton
{
    private:
        int width;
        int height;
        int qstate;
        unsigned char *data;
    public:
        CellularAutomaton(string, int); //Reads file in same format as Project 1C
        CellularAutomaton(int, int); //Create automaton of size n*n with random states and qstate
        CellularAutomaton(const CellularAutomaton &); //Copy constructor (deep copies)
        ~CellularAutomaton(); //Destructor
        int getWidth(); //Returns CA width
        int getHeight(); //Returns CA height
        unsigned char *getData(); //Returns pointer to the data of the CA
        const CellularAutomaton & operator=(const CellularAutomaton &);
        void step(unsigned char(*)(CellularAutomaton &, int, int)); //Peforms 1 step of the CA
        void displayCA(GraphicsClient &); //Size of the cell defined by
        void randomInit();
        void updateCellOnClick(GraphicsClient &, int, int);
        void readFile(string); //Loads the CA from a file
        void clear(); //Resets all the CA to the all 0 state
        /*
        Let m be max(width, height)
        m           Cell size in pixels         Cell gap in pixels
        200<m<=600  1                           0
        100<m<=200  2                           1
        50<m<=100   4                           1
        1<m<=50     10                          2
        */
};

class CellularAutomatonException
{
    private:
        string msg;
    public:
        CellularAutomatonException(string str): msg(str){};
        string message() {return msg;};
};

#endif