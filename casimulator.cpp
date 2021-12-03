#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "CellularAutomaton.h"
#include "GraphicsClient.h"
using namespace std;

//GOL prototype
unsigned char gol(CellularAutomaton &, int, int);

//Sum neighbors prototype
int sumNeighbors(CellularAutomaton &, int, int);

//Main driver program
int main()
{
    //Client server used for user interactions
    GraphicsClient client("127.0.0.1", 7777);

    //Pointer to the CA being simulated
    CellularAutomaton *ca = nullptr;

    //File path for return server messages
    string path;

    //Simulation states
    bool running = false;
    bool quit = false;

    //Driving loop, runs while the user hasn't pressed the quit button
    while(!quit)
    {
        //Pause for 100ms
        this_thread::sleep_for(chrono::milliseconds(100));

        //Check for messages sent from server
        int *xy = client.parseMessage();

        //If a message was recieved
        if(xy != NULL)
        {
            //Get the button that was pressed
            int button = client.getButtonPressed(xy[0], xy[1]);
            switch(button)
            {
                default:
                    break;
                case SIZE_1:
                    ca = new CellularAutomaton(40, 0);
                    ca->displayCA(client);
                    break;
                case SIZE_2:
                    ca = new CellularAutomaton(150, 0);
                    ca->displayCA(client);
                    break;
                case SIZE_3:
                    ca = new CellularAutomaton(600, 0);
                    ca->displayCA(client);
                    break;
                case STEP:
                    ca->step(gol);
                    ca->displayCA(client);
                    break;
                case RUN:
                    running = true;
                    break;
                case PAUSE:
                    running = false;
                    break;
                case RESET:
                    ca->clear();
                    ca->displayCA(client);
                    break;
                case RANDOM:
                    if(ca == nullptr)
                        ca = new CellularAutomaton(150, -1);
                    else
                        ca->randomInit();
                    ca->displayCA(client);
                    break;
                case LOAD:
                    path = client.requestFile();
                    if(!path.empty())
                    {
                        ca = new CellularAutomaton(path, 0);
                        ca->displayCA(client);
                    }
                    break;
                case QUIT:
                    quit = true;
                    break;
                case CA:
                    ca->updateCellOnClick(client, xy[0], xy[1]);
                    break;
            }
        }
        //Step and display while the simulation is running
        if(running)
        {
            ca->step(gol);
            ca->displayCA(client);
        }
    }
    return 0;

    //Step, read, repeat
}

//Rule for simulating according to the Game of Life
unsigned char gol(CellularAutomaton &ca, int x, int y)
{
    unsigned char *data = ca.getData();
    unsigned char value = data[ca.getWidth()*y+x];
    int neighbors = sumNeighbors(ca,x,y);
    if((value == 1 && (neighbors == 2 || neighbors == 3)) || (value == 0 && neighbors == 3))
        return 1;
    return 0;
}

//Returns the count of neighbors around the cell at (x, y)
int sumNeighbors(CellularAutomaton &ca, int x, int y)
{
    int sum = 0;
    unsigned char *data = ca.getData();
    for(int h = -1; h < 2; h++)
    {
        for(int w = -1; w < 2; w++)
        {
            if(w == 0 && h == 0)
                continue;
            int hoffset, woffset;
            if(y+h < 0)
                hoffset = ca.getHeight()-1;
            else if(y+h >= ca.getHeight())
                hoffset = 0;
            else
                hoffset = y+h;
            if(x+w < 0)
                woffset = ca.getWidth()-1;
            else if(x+w >= ca.getWidth())
                woffset = 0;
            else
                woffset = x+w;

            if(data[ca.getWidth()*hoffset+woffset] != 0)
                sum++;
        }
    }
    return sum;
}