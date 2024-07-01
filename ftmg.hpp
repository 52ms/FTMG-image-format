#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <X11/Xlib.h>

#define RGB(r, g, b) ((r << 16) | (g << 8) | b)

typedef std::string string;

unsigned int stringToHex(string strNum) {
    unsigned int returnNum = 0;
    std::stringstream ss;
    ss << std::hex << strNum;
    ss >> returnNum;
    return returnNum;
}

string openFile(const char* filePath) {
    string outData;
    std::fstream file (filePath);
    if (file.is_open()) {
        string currentLine;
        int lineNum = 0;
        while (getline(file, currentLine)) {
            ++lineNum;
            if (lineNum == 1 && currentLine != "#IMG_TYPE FTMG") {
                break;
            }
            outData += currentLine;
            outData += "\n";
        }
        outData.pop_back();
        file.close();
    }

    return outData;
}

std::vector<std::vector<string>> readData(string fileData) {
    std::vector<std::vector<string>> outData;

    string line1;
    std::stringstream ss1(fileData);

    int lineNum = 0;

    while (getline(ss1, line1, '\n')) {
        ++lineNum;
        if (lineNum == 1) continue;
        std::vector<string> currentLine;
        string line2;
        std::stringstream ss2(line1);

        while (getline(ss2, line2, ',')) {
            currentLine.push_back(line2);
        }

        outData.push_back(currentLine);
    }

    return outData;
}

void drawData(Display* display, Window window, GC gc, int startingX, int startingY, std::vector<std::vector<string>> fileData, int scale) {
    int currentX = startingX;
    int currentY = startingY;

    int pixelSize = (scale == NULL) ? 1 : scale;

    for (const auto& line : fileData) {
        currentX = startingX;
        currentY += pixelSize;
        for (const auto& value : line) {
            currentX += pixelSize;
            unsigned int curretnColor = stringToHex(value);
            XSetForeground(display, gc, curretnColor);
            XFillRectangle(display, window, gc, currentX, currentY, pixelSize, pixelSize);
        }
    }
}

void ftmgImageDraw(Display* display, Window window, GC gc, const char* path, int x, int y) {
    string fileData = openFile(path);

    drawData(display, window, gc, x, y, readData(fileData), NULL);
}

void ftmgImageDraw(Display* display, Window window, GC gc, const char* path, int x, int y, int scale) {
    string fileData = openFile(path);

    drawData(display, window, gc, x, y, readData(fileData), scale);
}