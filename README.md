# Creating a Flowchart in C++
Created by Mark Clemmer and Matthew Fisk

This project takes in an input file with all the classes for a major and puts them into a graph, which is used to create a flowchart to show the order in which the classes could be taken.

## Description
This project was created in C++ with mingw and uses the Lemon library to make a graph out of the courses from an input file. This graph is then used to create a flowchart using a Portable Pixel Map (PPM), which shows the courses that can be taken each quarter for the specific major. We created an output.txt file to show the courses.

## Requirements
- CMake VERSION 3.16.3 was used to create this project
- The GNU C++ Compiler is required to run the code through Ubuntu Linux 
- The Lemon library included above is needed for the graph
- A ppm file viewer is needed to view the flowchart output. A ppm viewer can be downloaded or the online ppm viewer from this website can be used https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html

## User Manual
### Setup:
Download the project from this repository. The Lemon library is included in this project, and no additional things are needed.
### Running the Program in Ubuntu:
1) g++ -o flowchart -I./lemon-1.3.1 -I./lemon-1.3.1/build main.cpp coursenode.cpp coursenode.h
2) ./flowchart
3) Follow the given instructions and input major1.txt or major2.txt.
4) Type "y" to enter constraints or "n" to have no constraints. If "y" was selected, enter the maximum credits per quarter and the quarter when you would like to start school.
Running the project should look like this:
![image](https://user-images.githubusercontent.com/94164990/226270211-c14c3b6e-0957-47ea-be09-73f85dac6ccc.png)


### Viewing the Results:
- Navigate into the folder where this project is located
- Open the output.txt file to see the text representation of how to take the courses. If you typed “y” into the command line when the code was running, output.txt will show the text representation with constraints. If “n” was entered, output.txt will show the text representation without constraints and automatically choose that the maximum credits per quarter should be 18, like in real life.
![image](https://user-images.githubusercontent.com/94164990/226266583-35ca4be2-82e8-44f1-9fad-9184ca807e50.png)

- Just like with the output.txt file, the results for the flowchart files that are created will change depending on if there are constraints. Each time the project is run, it will create a flowchart with edges (edges_picture.ppm) and a flowchart without edges (no_edges_picture.ppm) since the edges can cross over the courses and make it hard to read.
- Please open this ppm viewer (https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html) and then select the no_edges_picture.ppm or edges_picture.ppm file to view it.
![image](https://user-images.githubusercontent.com/94164990/226267737-f931db2f-7bdb-4845-8a53-0cddae4bcf05.png)
![image](https://user-images.githubusercontent.com/94164990/226267829-e38245f8-586e-46b6-a1dd-d724316fd025.png)


Please watch this Youtube video (https://www.youtube.com/watch?v=ddMZyd1Rh9o) where we walked through running the code and showed what the  output looks like.

## Reflection
*Write the reflection about getting the small groups in the minimum number of iterations, etc.*



