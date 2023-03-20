# Creating a Flowchart in C++
Created by Mark Clemmer and Matthew Fisk

This project takes in an input file with all the classes for a major and puts them into a graph, which is used to create a flowchart to show the order in which the classes could be taken.

## Description
*Describe what your project is about and what language you used*

This project was created in C++ with mingw and uses the Lemon library to make a graph out of the courses from an input file. This graph is then used to create a flowchart using a Portable Pixel Map (PPM), which shows the courses that can be taken each quarter for the specific major.

## Requirements
*The requirements of the program, i.e. Python 3.1, what libraries are needed*
- 
- A ppm file viewer is needed to view the flowchart output. A ppm viewer can be downloaded or the online ppm viewer from this website can be used https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html

## User Manual
*Once a person clones this into their computer how the person is supposed to run the program, add screenshots showing how your program works, also add here the link to the Youtube video showing the program running*
### Setup:
- 
### Running the Program in Ubuntu:
- g++ -o flowchart -I./lemon-1.3.1 -I./lemon-1.3.1/build main.cpp coursenode.cpp coursenode.h
- ./flowchart

## Reflection
*Write the reflection about getting the small groups in the minimum number of iterations, etc.*



