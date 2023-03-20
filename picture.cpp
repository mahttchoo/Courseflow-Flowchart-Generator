// TODO: Research pixel line generation from two points.
// TODO: Research array casting (mostly for letter generation method).
// TODO: Write word method.
// TODO: Get line() to work upwards too.
// TODO: Add arrowhead to line().
// TODO: line() doesn't seem to be hitting exactly the target.
// TODO: Look into storing the letter arrays in an external file (JSON?).
// TODO: Maybe make mega array to hold all of the letter arrays?

#include "picture.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>

using namespace std;

// Does this work?
//no
//how to make work?
Picture::Picture(int width, int height, int backgroundColor[]) {
    //_width = 512;
    //_height = 255;
    for (int i = 0; i < _width; i++) {
        for (int j = 0; j < _height; j++) {
            _colorMap[i][j][0] = backgroundColor[0];
            _colorMap[i][j][1] = backgroundColor[1];
            _colorMap[i][j][2] = backgroundColor[2];
        }
    }
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 70; j++) {
            _colorMap[i][j][0] = 14;
            _colorMap[i][j][1] = 255;
            _colorMap[i][j][2] = 0;
        }
    }
    _colorMap[5][3][0] = 0;
    _colorMap[5][3][1] = 0;
    _colorMap[5][3][2] = 255;
}

void Picture::placeBox(int x, int y, int width, int height, int rgb[]) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            _colorMap[x + i][y + j][0] = rgb[0];
            _colorMap[x + i][y + j][1] = rgb[1];
            _colorMap[x + i][y + j][2] = rgb[2];
        }
    }
}

void Picture::placeLine(int start_x, int start_y, int end_x, int end_y, int width, int rgb[3]) {
    int sy = start_y;
    cout << "line() was called" << endl;
    int y_segments = (end_y - start_y) / (end_x - start_x);
    int x_segments = (end_x - start_x) / (end_y - start_y);
    y_segments++;
    x_segments++;
    cout << "segments: " << y_segments << endl;

    for (int x = start_x; x < end_x; x++) {
        if (x % x_segments == 0) {
            sy++;
        }
        for (int y = sy; y < y_segments + sy; y++) {
            _colorMap[x][y][0] = rgb[0];
            _colorMap[x][y][1] = rgb[1];
            _colorMap[x][y][2] = rgb[2];
        }
        sy += y_segments - 1;
    }
    _colorMap[start_x][start_y][0] = 0;
    _colorMap[start_x][start_y][1] = 0;
    _colorMap[start_x][start_y][2] = 0;
    _colorMap[end_x][end_y][0] = 0;
    _colorMap[end_x][end_y][1] = 0;
    _colorMap[end_x][end_y][2] = 0;
}

void Picture::placeWord(int x, int y, string s, int rgb[3]) {
    cout << "Called placeWord" << endl;
    cout << "string value: " << s << endl;
    for (int i = 0; i < s.size(); i++) {
        cout << "value at " << i << ": " << s.at(i) << endl;
    }
}

void Picture::placeLetter(int x, int y, char letter, int rgb[3]) {
    cout << "letter character: " << letter << endl;
    cout << "letter integer: " << int(letter) << endl;
    int letArr[10][7];
    switch(int(letter)) {
        case 67: // Letter C also this is gross ew
            int a[10][7] = {0,1,1,1,1,1,0,
                            1,1,1,0,0,1,1,
                            1,1,0,0,0,0,0,
                            1,0,0,0,0,0,0,
                            1,0,0,0,0,0,0,
                            1,1,0,0,0,0,0,
                            1,1,1,0,0,1,1,
                            0,1,1,1,1,1,0};
            for (int i = 0; i < 7; i++) {
                for (int j = 0; j < 10; j++) {
                    if (a[j][i] == 1) {
                        _colorMap[x + i][y + j][0] = rgb[0];
                        _colorMap[x + i][y + j][1] = rgb[1];
                        _colorMap[x + i][y + j][2] = rgb[2];
                    }
                }
            }
            break;
    }
}

void Picture::createPPM(string name) {
    cout << "ran createPPM" << endl;
    //string file = name << ".ppm";
    ofstream img (name);
    img << "P3" << endl;
    img << _width << " " << _height << endl;
    img << "255" << endl;
    for (int x = 0; x < _width; x++) {
        for (int y = 0; y < _height; y++) {
            int r = _colorMap[x][y][0];
            int g = _colorMap[x][y][1];
            int b = _colorMap[x][y][2];

            img << r << " " << g << " " << b << endl;
        }
    }
}

/*
int t[5];
memset(t, 0, 4);

for (int i = 0; i < 5; i++) {
    cout << t[i] << endl;
}

 int a[7][10] = {0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0,
                             0,0,0,0,0,0,0};
 */

// https://www.youtube.com/watch?v=fbH005SzEMc&ab_channel=MarcusMathiassen
//
// Command to run through Ubuntu in the flowchart project. When accessing file in code it must be "major.txt" and not "../major.txt"
// g++ -I./lemon-1.3.1 -I./lemon-1.3.1/build -I./ main.cpp coursenode.cpp coursenode.h
// g++ -I./lemon-1.3.1 -I./lemon-1.3.1/build -I./ main.cpp coursenode.cpp coursenode.h picture.cpp picture.h