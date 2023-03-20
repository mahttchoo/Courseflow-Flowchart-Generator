#include <string>

using namespace std;

#ifndef FLOWCHART_THE_FELLOWSHIP_PICTURE_H
#define FLOWCHART_THE_FELLOWSHIP_PICTURE_H


class Picture {
    int _width = 512;
    int _height = 100;
    int _colorMap[512][100][3];
    void placeLetter(int x, int y, char letter, int rgb[3]);

public:
    Picture(int width, int height, int backgroundColor[]);
    void placeBox(int x, int y, int width, int height, int rgb[3]);
    void placeLine(int start_x, int start_y, int end_x, int end_y, int width, int rgb[3]);
    void placeWord(int x, int y, string s, int rgb[3]);
    void createPPM(string name);
};


#endif //FLOWCHART_THE_FELLOWSHIP_PICTURE_H
