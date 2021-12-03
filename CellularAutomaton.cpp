#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <iostream>
#include "CellularAutomaton.h"
#include "GraphicsClient.h"

#ifndef ASCII_OFFSET
#define ASCII_OFFSET 48
#endif

using namespace std;

//Create a new CellularAutomaton from a file with the given quiescent state
CellularAutomaton::CellularAutomaton(string filepath, int qstate)
{
    readFile(filepath);
    this->qstate = qstate;
}

//Create CA of size n*n with random initialization
CellularAutomaton::CellularAutomaton(int n, int quiescent)
{
    width = n;
    height = n;
    data = new (nothrow) unsigned char[width*height];
    
    if(quiescent == -1)
    {
        qstate = 0;
        randomInit();
    }
    else
    {
        clear();
        qstate = quiescent;
    }
}

//Create a new CellularAutomaton using another's information
CellularAutomaton::CellularAutomaton(const CellularAutomaton &rhs)
{
    width = rhs.width;
    height = rhs.height;
    qstate = rhs.qstate;
    data = new (nothrow) unsigned char[width*height];

    for(int r = 0; r < height; r++)
        for(int c = 0; c < width; c++)
            data[width*r + c] = rhs.data[width*r + c];
}

//Deconstructs the CellularAutomaton by deleting its data
CellularAutomaton::~CellularAutomaton()
{
    delete[] data;
}

//Assign the information of this CellularAutomaton to be that of the rhs.
//Deep copies rhs's data
const CellularAutomaton & CellularAutomaton::operator= (const CellularAutomaton &rhs)
{
    if(this != &rhs)
    {
        delete[] data;
        width = rhs.width;
        height = rhs.height;
        qstate = rhs.qstate;
        data = new (nothrow) unsigned char[width*height];

        for(int r = 0; r < height; r++)
            for(int c = 0; c < width; c++)
                data[width*r + c] = rhs.data[width*r + c];
    }

    return *this;
}

//Progesses the CellularAutomaton by one step using the given rule
void CellularAutomaton::step(unsigned char (*rule)(CellularAutomaton &ca, int x, int y))
{
    CellularAutomaton tmp = CellularAutomaton(*this);
    for(int r = 0; r < height; r++)
        for(int c = 0; c < width; c++)
            data[width*r+c] = rule(tmp, c, r);
}

/*
Let m be max(width, height)
m           Cell size in pixels         Cell gap in pixels
200<m<=600  1                           0
100<m<=200  2                           1
50<m<=100   4                           1
1<m<=50     10                          2

Draws the CellularAutomaton in the graphics window related to the given client
*/
void CellularAutomaton::displayCA(GraphicsClient &gc)
{
    int m = max(width, height);
    int cellSize, cellGap;
    if(m <= 600 && m > 200)
    {
        cellSize = 1;
        cellGap = 0;
    }
    else if(m <= 200 && m > 100)
    {
        cellSize = 2;
        cellGap = 1;
    }
    else if(m <= 100 && m > 50)
    {
        cellSize = 4;
        cellGap = 1;
    }
    else
    {
        cellSize = 10;
        cellGap = 2;
    }

    int serverX = 0, serverY = 0;
    gc.setDrawingColor(0, 0, 0);
    gc.fillRectangle(0, 0, 600, 600);
    //gc.setBackgroundColor(0, 0, 0);
    //gc.clearRectangle(0, 0, 600, 600);
    gc.setDrawingColor(50, 50, 255);

    for(int r = 0; r < height; r++)
    {
        for(int c = 0; c < width; c++)
        {
            if(data[width*r+c] != 0)
            {
                if(r == height-1 && c == width-1)
                {
                    gc.fillRectangle(serverX, serverY, cellSize, cellSize);
                    break;
                }
                else if(c == width-1)
                {
                    gc.fillRectangle(serverX, serverY, cellSize, cellSize);
                    serverX = 0;
                    serverY += (cellSize+cellGap);
                }
                else
                {
                    gc.fillRectangle(serverX, serverY, cellSize, cellSize);
                    serverX += (cellSize+cellGap);
                }
            }
            else
            {
                if(c == width-1)
                {
                    serverX = 0;
                    serverY += (cellSize+cellGap);
                }
                else
                {
                    serverX += (cellSize+cellGap);
                }
            }
        }
    }
    gc.repaint();
}

//Loads the data from the file into the CellularAutomaton
//Uses modified C code from Project 1C
void CellularAutomaton::readFile(string filepath)
{
    FILE *file = fopen(filepath.c_str(), "r");
    if(file == NULL)
    {
        throw CellularAutomatonException("File not found or invalid path.");
    }
    const size_t line_size = 600;
    char *buffer = new char[line_size];

    //Read and parse first line
    fgets(buffer, line_size, file);
    height = atoi(strtok(buffer, " "));
    width = atoi(strtok(NULL, " "));
    data = new (nothrow) unsigned char[width*height];

    int row = 0, col = 0;
    while(fgets(buffer, line_size, file) != NULL)
    {
        for(int i = 0; buffer[i] != '\0'; i++)
            if(buffer[i] != ' ' && buffer[i] != '\n')
            {
                unsigned char value = (unsigned char)(buffer[i]-ASCII_OFFSET);

                if(value == 0 || value == 1)
                {
                    data[width*row+col] = value;
                    col += 1;
                    if(col == width)
                        col = 0;
                }
                else
                {
                    delete[] buffer;
                    delete[] data;
                    fclose(file);
                    throw CellularAutomatonException("Invalid number of states in file. GOL allows only 2 states.");
                }
            }
        row += 1;
    }
    delete[] buffer;
    fclose(file);
}

void CellularAutomaton::randomInit()
{
    srand(time(NULL));
    for(int r = 0; r < height; r++)
        for(int c = 0; c < width; c++)
            data[width*r + c] = (unsigned char)(rand() % 2);
}

//Returns the width of the CA
int CellularAutomaton::getWidth()
{
    return width;
}

//Returns the heigth of the CA
int CellularAutomaton::getHeight()
{
    return height;
}

//Returns the data of the CA
unsigned char * CellularAutomaton::getData()
{
    return data;
}

//Updates the cell when clicked in on the client in the CA
void CellularAutomaton::updateCellOnClick(GraphicsClient &gc, int x, int y)
{
    //Calculate appropriate cell size and gap
    int m = max(width, height);
    int cellSize, cellGap;
    if(m <= 600 && m > 200)
    {
        cellSize = 1;
        cellGap = 0;
    }
    else if(m <= 200 && m > 100)
    {
        cellSize = 2;
        cellGap = 1;
    }
    else if(m <= 100 && m > 50)
    {
        cellSize = 4;
        cellGap = 1;
    }
    else
    {
        cellSize = 10;
        cellGap = 2;
    }

    //Make sure click was in the CA area
    if((x >= 0 && y >= 0) && (x <= (width*cellSize)+((width-1)*cellGap) && y <= (width*cellSize)+((width-1)*cellGap)) &&(x <= 600 && y <= 600))
    {
        //If the click was in a cell, and not a gap
        if(x % (cellSize+cellGap) < cellSize && y % (cellSize+cellGap) < cellSize)
        {
            //Calculate (x, y) where the original square would be drawn from
            x = x-(x % (cellSize+cellGap));
            y = y-(y % (cellSize+cellGap));

            int col = x/(cellSize+cellGap);
            int row = y/(cellSize+cellGap);

            //Draw appropriate cell, flip value in the ca
            if(data[width*row + col] == 0)
            {
                data[width*row + col] = 1;
                gc.setDrawingColor(50, 50, 255);
                gc.fillRectangle(x, y, cellSize, cellSize);
                gc.repaint();
            }
            else
            {
                data[width*row + col] = 0;
                gc.clearRectangle(x, y, cellSize, cellSize);
                gc.repaint();
            }
        }
    }
}

//Sets the CA to an all 0 state
void CellularAutomaton::clear()
{
    for(int r = 0; r < height; r++)
        for(int c = 0; c < width; c++)
            data[width*r+c] = 0;
}