#include <iostream>

//Necessary includes for Windows sockets
#pragma comment(lib, "ws2_32.lib") 
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#include <string>
#include "GraphicsClient.h"

using namespace std;

//Sets the url and port for this client, uses helper connectClient() method
GraphicsClient::GraphicsClient(string url, int port)
{
    this->url = url;
    this->port = port;

    connectClient();
    drawGUI();
}

//Connects the client to the given url and port
void GraphicsClient::connectClient()
{
    //Windows socket necessity
    WORD versionWanted = MAKEWORD(1,1);
    WSADATA wsaData;
    WSAStartup(versionWanted, &wsaData);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        throw GraphicsClientException("Error creating socket.");
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, url.c_str(), &serv_addr.sin_addr)<=0)
    {
        closesocket(sockfd);
        WSACleanup();
        throw GraphicsClientException("Invalid address or address not supported.");
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        closesocket(sockfd);
        WSACleanup();
        throw GraphicsClientException("Connection failed.");
    }
}

//Copy the information from rhs into this object and connect
GraphicsClient::GraphicsClient(const GraphicsClient &rhs) : GraphicsClient(rhs.url, rhs.port){}

//Close the socket
GraphicsClient::~GraphicsClient()
{
    closesocket(sockfd);
    WSACleanup(); //Required for Windows sockets
}

//Closes the current connection related to this client and
//connects to the url and port of the rhs
const GraphicsClient & GraphicsClient::operator= (const GraphicsClient &rhs)
{
    if(this != &rhs)
    {
        closesocket(sockfd);
        WSACleanup();
        url = rhs.url;
        port = rhs.port;
        connectClient();
    }
    return *this;
}


//Sets server background color
void GraphicsClient::setBackgroundColor(int r, int g, int b)
{
    char *red = intToHex(r);
    char *green = intToHex(g);
    char *blue = intToHex(b);
    char message[12];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x00;
    message[4] = 0x07;
    message[5] = 0x02;
    message[6] = red[1];
    message[7] = red[0];
    message[8] = green[1];
    message[9] = green[0];
    message[10] = blue[1];
    message[11] = blue[0];
    send(sockfd, message, 12, 0);
}

//Sets server drawing color
void GraphicsClient::setDrawingColor(int r, int g, int b)
{
    char *red = intToHex(r);
    char *green = intToHex(g);
    char *blue = intToHex(b);
    char message[12];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x00;
    message[4] = 0x07;
    message[5] = 0x06;
    message[6] = red[1];
    message[7] = red[0];
    message[8] = green[1];
    message[9] = green[0];
    message[10] = blue[1];
    message[11] = blue[0];
    send(sockfd, message, 12, 0);
}

//Set server display to background color
void GraphicsClient::clear()
{
    char message[6];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x00;
    message[4] = 0x01;
    message[5] = 0x01;
    send(sockfd, message, 6, 0); 
}

//Sets the pixel at (x, y) to given rgb value
void GraphicsClient::setPixel(int x, int y, int r, int g, int b)
{
    char *xRes = intToHex(x);
    char *yRes = intToHex(y);
    char *red = intToHex(r);
    char *green = intToHex(g);
    char *blue = intToHex(b);
    char message[20];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x00;
    message[4] = 0x0F;
    message[5] = 0x03;
    message[6] = xRes[3];
    message[7] = xRes[2];
    message[8] = xRes[1];
    message[9] = xRes[0];
    message[10] = yRes[3];
    message[11] = yRes[2];
    message[12] = yRes[1];
    message[13] = yRes[0];
    message[14] = red[1];
    message[15] = red[0];
    message[16] = green[1];
    message[17] = green[0];
    message[18] = blue[1];
    message[19] = blue[0];
    send(sockfd, message, 20, 0);
}

//Draws rectangle starting at (x, y) with the given width and height 
void  GraphicsClient::drawRectangle(int x, int y, int w, int h)
{
    char *xRes = intToHex(x);
    char *yRes = intToHex(y);
    char *wRes = intToHex(w);
    char *hRes = intToHex(h);
    char message[22];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x01;
    message[4] = 0x01;
    message[5] = 0x07;
    message[6] = xRes[3];
    message[7] = xRes[2];
    message[8] = xRes[1];
    message[9] = xRes[0];
    message[10] = yRes[3];
    message[11] = yRes[2];
    message[12] = yRes[1];
    message[13] = yRes[0];
    message[14] = wRes[3];
    message[15] = wRes[2];
    message[16] = wRes[1];
    message[17] = wRes[0];
    message[18] = hRes[3];
    message[19] = hRes[2];
    message[20] = hRes[1];
    message[21] = hRes[0];
    send(sockfd, message, 22, 0);
}

//Fills rectangle starting at (x, y) of given width and height
void GraphicsClient::fillRectangle(int x, int y, int w, int h)
{
    char *xRes = intToHex(x);
    char *yRes = intToHex(y);
    char *wRes = intToHex(w);
    char *hRes = intToHex(h);
    char message[22];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x01;
    message[4] = 0x01;
    message[5] = 0x08;
    message[6] = xRes[3];
    message[7] = xRes[2];
    message[8] = xRes[1];
    message[9] = xRes[0];
    message[10] = yRes[3];
    message[11] = yRes[2];
    message[12] = yRes[1];
    message[13] = yRes[0];
    message[14] = wRes[3];
    message[15] = wRes[2];
    message[16] = wRes[1];
    message[17] = wRes[0];
    message[18] = hRes[3];
    message[19] = hRes[2];
    message[20] = hRes[1];
    message[21] = hRes[0];
    send(sockfd, message, 22, 0);
}

//Sets the rectangle on the server display at (x, y) of given width and height to the background color
void GraphicsClient::clearRectangle(int x, int y, int w, int h)
{
    char *xRes = intToHex(x);
    char *yRes = intToHex(y);
    char *wRes = intToHex(w);
    char *hRes = intToHex(h);
    char message[22];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x01;
    message[4] = 0x01;
    message[5] = 0x09;
    message[6] = xRes[3];
    message[7] = xRes[2];
    message[8] = xRes[1];
    message[9] = xRes[0];
    message[10] = yRes[3];
    message[11] = yRes[2];
    message[12] = yRes[1];
    message[13] = yRes[0];
    message[14] = wRes[3];
    message[15] = wRes[2];
    message[16] = wRes[1];
    message[17] = wRes[0];
    message[18] = hRes[3];
    message[19] = hRes[2];
    message[20] = hRes[1];
    message[21] = hRes[0];
    send(sockfd, message, 22, 0);
}

//Draws oval inside rectangle starting at (x, y) with given width and height
void GraphicsClient::drawOval(int x, int y, int w, int h)
{
    char *xRes = intToHex(x);
    char *yRes = intToHex(y);
    char *wRes = intToHex(w);
    char *hRes = intToHex(h);
    char message[22];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x01;
    message[4] = 0x01;
    message[5] = 0x0A;
    message[6] = xRes[3];
    message[7] = xRes[2];
    message[8] = xRes[1];
    message[9] = xRes[0];
    message[10] = yRes[3];
    message[11] = yRes[2];
    message[12] = yRes[1];
    message[13] = yRes[0];
    message[14] = wRes[3];
    message[15] = wRes[2];
    message[16] = wRes[1];
    message[17] = wRes[0];
    message[18] = hRes[3];
    message[19] = hRes[2];
    message[20] = hRes[1];
    message[21] = hRes[0];
    send(sockfd, message, 22, 0);
}

//Fills oval inside rectangle starting at (x, y) with given width and height
void GraphicsClient::fillOval(int x, int y, int w, int h)
{
    char *xRes = intToHex(x);
    char *yRes = intToHex(y);
    char *wRes = intToHex(w);
    char *hRes = intToHex(h);
    char message[22];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x01;
    message[4] = 0x01;
    message[5] = 0x0B;
    message[6] = xRes[3];
    message[7] = xRes[2];
    message[8] = xRes[1];
    message[9] = xRes[0];
    message[10] = yRes[3];
    message[11] = yRes[2];
    message[12] = yRes[1];
    message[13] = yRes[0];
    message[14] = wRes[3];
    message[15] = wRes[2];
    message[16] = wRes[1];
    message[17] = wRes[0];
    message[18] = hRes[3];
    message[19] = hRes[2];
    message[20] = hRes[1];
    message[21] = hRes[0];
    send(sockfd, message, 22, 0);
}

//Draw line on the server display from (x1, y1) to (x2, y2)
void GraphicsClient::drawLine(int x1, int y1, int x2, int y2)
{
    char *x1Res = intToHex(x1);
    char *y1Res = intToHex(y1);
    char *x2Res = intToHex(x2);
    char *y2Res = intToHex(y2);
    char message[22];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x01;
    message[4] = 0x01;
    message[5] = 0x0D;
    message[6] = x1Res[3];
    message[7] = x1Res[2];
    message[8] = x1Res[1];
    message[9] = x1Res[0];
    message[10] = y1Res[3];
    message[11] = y1Res[2];
    message[12] = y1Res[1];
    message[13] = y1Res[0];
    message[14] = x2Res[3];
    message[15] = x2Res[2];
    message[16] = x2Res[1];
    message[17] = x2Res[0];
    message[18] = y2Res[3];
    message[19] = y2Res[2];
    message[20] = y2Res[1];
    message[21] = y2Res[0];
    send(sockfd, message, 22, 0);
}

//Displays message on the server display at location (x, y)
void GraphicsClient::drawString(int x, int y, string str)
{
    char *xRes = intToHex(x);
    char *yRes = intToHex(y);
    char *payloadSize = intToHex(2*(str.length())+9);
    char message[512];
    message[0] = 0xFF;
    message[1] = payloadSize[3];
    message[2] = payloadSize[2];
    message[3] = payloadSize[1];
    message[4] = payloadSize[0];
    message[5] = 0x05;
    message[6] = xRes[3];
    message[7] = xRes[2];
    message[8] = xRes[1];
    message[9] = xRes[0];
    message[10] = yRes[3];
    message[11] = yRes[2];
    message[12] = yRes[1];
    message[13] = yRes[0];
    int j = 14;
    for(int i = 0; i < str.length(); i++)
    {
        char *charRes = intToHex(str[i]);
        message[j] = charRes[1];
        message[j+1] = charRes[0];
        j = j+2;
    }
    send(sockfd, message, 2*str.length()+14, 0);
}

//Refresh graphics window
void GraphicsClient::repaint()
{
    char message[6];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x00;
    message[4] = 0x01;
    message[5] = 0x0C;
    send(sockfd, message, 6, 0);
}

//Opens window for the user to select a CA from, waits for the user to pick a file,
//The returns the path of the selected file
string GraphicsClient::requestFile()
{
    char message[6];
    message[0] = 0xFF;
    message[1] = 0x00;
    message[2] = 0x00;
    message[3] = 0x00;
    message[4] = 0x01;
    message[5] = 0x0E;
    send(sockfd, message, 6, 0);

    unsigned long count = 0;

    while(count == 0)
        ioctlsocket(sockfd, FIONREAD, &count);
    if(count == 6)
        return "";
    char *buffer = new char[count];
    recv(sockfd, buffer, count, 0);
    string res = "";
    for(int i = 6; i < count; i += 2)
        res = res + char((unsigned char)(buffer[i] & 0x0f) << 4 | (unsigned char)(buffer[i+1] & 0x0f));
    return res;
}

//Returns an array containing the (x, y) point of the click
int* GraphicsClient::parseMessage()
{
    unsigned long count;
    //Replace with 'ioctl' on Linux
    ioctlsocket(sockfd, FIONREAD, &count);
    if(count != 0)
    {
        int *xy = new int[2];
        char buffer[256];

        //Replace with 'read' on Linux
        recv(sockfd, buffer, count, 0);
        for(int i = 0; i < count; i += 15)
        {
            if(buffer[i+5] == 3)
            {
                xy[0] = hexToInt(buffer, i+7);
                xy[1] = hexToInt(buffer, i+11);
                return xy;
            }
        }
    }
    
    return NULL;
}

//Converts and int type to an array of four bytes such that
//only the lower 4 bits of each byte are used.
char * GraphicsClient::intToHex(int num)
{   
    char *result = new char[4];
    result[0] = num & 0x0000000f;
    result[1] = (num & 0x000000f0) >> 4;
    result[2] = (num & 0x00000f00) >> 8;
    result[3] = (num & 0x0000f000) >> 12;
    return result;
}

//Coverts the bytes in buffer to an int between start and end, inclusive
int GraphicsClient::hexToInt(char *buf, int start)
{
    int res = int((unsigned char)(buf[start] & 0x0f) << 12 |
                  (unsigned char)(buf[start+1] & 0x0f) << 8 |
                  (unsigned char)(buf[start+2] & 0x0f) << 4 |
                  (unsigned char)(buf[start+3] & 0x0f));
    return res;
}

//Draws the GUI
void GraphicsClient::drawGUI()
{
    setBackgroundColor(0, 0, 0);
    clear();
    setDrawingColor(100, 100, 100);
    fillRectangle(600, 0, 200, 600);

    setDrawingColor(50, 50, 50);
    fillRectangle(650, 10, 100, 30); //Step
    fillRectangle(650, 45, 100, 30); //Run
    fillRectangle(650, 80, 100, 30); //Pause
    fillRectangle(650, 115, 100, 30); //Reset
    fillRectangle(650, 150, 100, 30); //Random
    fillRectangle(650, 185, 100, 30); //Load
    fillRectangle(650, 220, 100, 30); //Quit
    fillRectangle(650, 255, 100, 50);

    setDrawingColor(255, 255, 255);
    drawRectangle(650, 285, 20, 20); //1
    drawRectangle(690, 285, 20, 20); //2
    drawRectangle(730, 285, 20, 20); //3

    drawString(685, 27, "Step");
    drawString(685, 62, "Run");
    drawString(680, 97, "Pause");
    drawString(680, 132, "Reset");
    drawString(675, 167, "Random");
    drawString(685, 202, "Load");
    drawString(685, 237, "Quit");
    drawString(685, 272, "Size");
    drawString(657, 300, "1");
    drawString(697, 300, "2");
    drawString(737, 300, "3");

    repaint();
}

int GraphicsClient::getButtonPressed(int x, int y)
{
    if(x >= 0 && x <= 600)
        return CA;
    else if(x >= 650 && x <= 750)
    {
        if(y >= 10 && y <= 40)
            return STEP;
        if(y >= 45 && y <= 75)
            return RUN;
        if(y >= 80 && y <= 110)
            return PAUSE;
        if(y >= 115 && y <= 145)
            return RESET;
        if(y >= 150 && y <= 180)
            return RANDOM;
        if(y >= 185 && y <= 215)
            return LOAD;
        if(y >= 220 && y <= 250)
            return QUIT;
        
        if(y >= 285 && y <= 305)
        {
            if(x <= 670)
                return SIZE_1;
            if(x >= 690 && x <= 710)
                return SIZE_2;
            if(x >= 730)
                return SIZE_3;
        }

        return NO_OPTION;
    }
    return NO_OPTION;
}