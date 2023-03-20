/*
 * TODO: Give an option to select the file later. Might have to use command line parameters.
 * TODO: Allow user to select which file they want to make a graph of.
 * TODO: Make sure there are no memory leaks.
 * TODO: Make createNode() a constructor within the class and not a function here?
 * TODO: Look into changing the file reader to using a try/catch block and throwing errors if the file isn't opened.
 * TODO: Sort the nodes in the graph so searching for pre-reqs takes O(logn) instead of O(n).
 * TODO: Add a check to make sure the maxCredits is above 5 (or maybe we might have 12 minimum).
 * TODO: Try passing a set by reference into pickClasses, and editing that set as I go through.
 * TODO: Double check the interval partitioning algorithm and maybe rewrite pickClasses to use it.
 */

/*
 * This code works!
 * g++ -I./lemon-1.3.1 -I./lemon-1.3.1/build main.cpp coursenode.cpp coursenode.h
 *
 * ---- There are currently two main approaches to generating the flowchart ----
 * Approach 1:
 *      Generate a vector with all of the CourseNodes, sort the vector, then add each element to our graph
 *      Benefits: Since the ids will be sorted, finding pre-reqs will be in O(logn) time.
 *      Downsides: We need O(n) to add into vector, O(nlogn) to sort, and another O(n) to add to graph.
 *      Conclusion: Would probably be better for a large number of classes, or for a lot of pre-reqs
 * Approach 2:
 *      Benefits: Only O(n) to add into graph.
 *      Downsides: O(n) every time we want to add a pre-req.
 *      Conclusion: Would probably be better for smaller number of classes, or for fewer pre-reqs.
 * Final Conclusion:
 *      Approach 1 seems better, but is a harder to code. We will add it later and will go with approach 2
 *      for the time being.
 *
 *
 * ---- Traversing through the graph on our own ----
 *      I can't find a way to get a list of arcs given a node in lemon. Not sure why. What I can do is when we
 *      generate the arcs, I can store the id of the arc in the CourseNode class. We can access this with a getter.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cstring>
#include "picture.h"
#include "coursenode.h"
#include <lemon/smart_graph.h>

using namespace std;
using namespace lemon;

CourseNode* createNode(string input); // This should probably be a constructor in the courseNode.cpp file.
set<int> pickClasses(set<int> s, int maxCredits, int year); // Returns optimal set of classes that is under the max credits given a set of classes.
void assignPriority(int id); // Given the id of a node, sets the priority of that node and every node reachable from it as the length of its longest path.

SmartDigraph graph;
SmartDigraph::NodeMap<CourseNode*> data(graph);

void box(int x, int y, int width, int height, int rgb[3]);
void line(int start_x, int start_y, int end_x, int end_y, int width, int rgb[3]);
void letter(int x, int y, char letter, int rgb[3]);
void word(int x, int y, string s, int rgb[3]);

const int width = 1100, height = 450;
int arr[width][height][3];

int main() {
    /*
    int rgb[3] = {255,0,0};
    Picture* pic = new Picture(512,255,rgb);
    int rgb2[3] = {255,255,255};
    //pic->placeBox(30,10,100,20,rgb2);
    pic->createPPM("pic.ppm");
     */

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            arr[i][j][0] = 200;
            arr[i][j][1] = 200;
            arr[i][j][2] = 200;
        }
    }

    set<int> availableClasses[3]; // Array of sets of node ids of courses that are available. Array index determines quarter.
    set<int> finalClasses[4][3];

    string line;
    ifstream readFile;
    readFile.open("major1.txt"); // Will change this to open the user selected file.
    if (readFile.is_open()) {
        // TODO: define vector here
        while (!readFile.eof()) {
            getline(readFile, line);
            // Create the node with the given line that contains a course
            CourseNode* course = createNode(line);
            // Add node to graph here
            SmartDigraph::Node n = graph.addNode();
            data[n] = course;
        }
        //TODO:
        // Use merge sort on vector
        // Iterate through vector and put course nodes into graph
        /*for (int i = 0; i < vect.size(); i++) {
            SmartDigraph::Node n = graph.addNode();
            data[n] = vect[i];
        }*/
    } else {
        // Should we set this to throwing an error using a try/catch block?
        cout << "File could not be opened." << endl;
    }
    readFile.close();

    // Step through the graph
//    for (SmartDigraph::NodeIt n(graph); n != INVALID; ++n) {
//        cout << "Node of id " << graph.id(n) << "         " << data[n]->ToString() << endl;
//    }

    // TODO: This is what causes O(n^2) time complexity, change later for O(nlogn)
    int arcCount = 0;
    // Iterate through the graph to add the edges/arcs
    for (SmartDigraph::NodeIt n(graph); n != INVALID; ++n) {
         // Traverse through the requirements vector for the current node
         for (int i = 0; i < data[n]->GetRequirements().size(); i++) {
             string req = data[n]->GetRequirements()[i];
             for (SmartDigraph::NodeIt j(graph); j != INVALID; ++j) {
                 if (data[j]->GetCourseCode() == req) {
                     // Add the arc if the course code is equal to the current requirement
                     // The arc should be directed from data[j] to data[n]
                     SmartDigraph::Arc a = graph.addArc(j, n);
                     data[j]->AddArc(graph.id(a));
                     arcCount++;
                 }
             }

         }
    }

    // Asking the user start quarter and maximum number of credits
    int maxCredits = 0;
    int startQuarter = 0;
    string input;

    while (maxCredits < 5 || maxCredits > 18) {
        cout << "Please enter the maximum number of credits you would like to take per quarter (5 - 18)" << endl;
        cin >> input;
        try {
            maxCredits = stoi(input);
        } catch (exception &err) {
            cout << "Please enter an integer." << endl;
        }
    }
    cout << "Please enter the quarter you are starting school" << endl;
    while (startQuarter < 1 || startQuarter > 3) {
        cout << "[1] for Autumn, [2] for Winter, and [3] for Spring" << endl;
        cin >> input;
        try {
            startQuarter = stoi(input);
        } catch (exception &err) {
            cout << "Please enter an integer." << endl;
        }
    }
    cout << "\nYou will take no more than " << maxCredits << " per quarter." << endl;
    cout << "You are starting in quarter " << startQuarter << "." << endl << endl;

    // Generating a Set of classes the user actually has available.
    set<int> rootCourses;
    for (SmartDigraph::NodeIt n(graph); n != INVALID; ++n) {
        CourseNode* course = data[n];
        if (course->GetRequirements()[0] == "") { // If number of pre-requirements is zero
            rootCourses.insert(graph.id(n));
            vector<int> v = course->GetQuarters();
            for (int j = 0; j < v.size(); j++) {
                availableClasses[v[j] - 1].insert(graph.id(n));
            }
        }
    }

    // Driver function for assignPriority(), since assignPriority() is a DFS recursive function.
    for (set<int>::iterator itr = rootCourses.begin(); itr != rootCourses.end(); itr++) {
        assignPriority(*itr);
    }

//    for (int i = 0; i < 3; i++) { // Iterate through availableClasses for quarters 1, 2, and 3.
//        cout << "\nClasses that a freshman student can take in quarter " << i + 1 << endl;
//        set<int>::iterator itr;
//        // Displaying set elements
//        for (itr = availableClasses[i].begin(); itr != availableClasses[i].end(); itr++) {
//            cout << "\t" << data[graph.nodeFromId(*itr)]->ToString() << endl;
//        }
//    }

    /*
    cout << "\n\nPrinting out the node priorities:" << endl << endl;
    for (SmartDigraph::NodeIt n(graph); n != INVALID; ++n) {
        cout << data[n]->ToString() << endl << "\tPriority Value: " << data[n]->GetPriority() << endl;
    }
    */

    // File output to .txt file to be read and used in Javascript to create a display of the classes
    ofstream outputFile("output.txt");

    // This runs as long as there are more classes the user can take.
    int currentYear = 1;
    cout << "Year 1:" << endl;
    int currentQuarter = startQuarter - 1; // We have fall/winter/spring quarters labelled as 1/2/3 for the user, but the availableClasses[][] stores this using index 0/1/2 respectively.
    while (!availableClasses[0].empty() || !availableClasses[1].empty() || !availableClasses[2].empty()) {
        // Each time that the quarter is 3 (quarters are 0, 1, 2), another year has gone by
        if (currentQuarter == 3) {
            currentYear++;
            cout << endl << "Year " << currentYear << ":" << endl;
        }

        currentQuarter = currentQuarter % 3; // If quarter is greater than spring, it wraps it back to fall.
        set<int> s = pickClasses(availableClasses[currentQuarter], maxCredits, currentYear);

        cout << "\nCLASSES TO TAKE DURING QUARTER " << currentQuarter + 1 << ":" << endl;

        outputFile << "Quarter " << currentQuarter + 1 << endl;

        // Iterate through set of classes generated by pickClasses() for the current quarter,
        // add those classes to the output and output file,
        // then remove them from classes the user can take in availableClasses.
        int c = -1;
        for (auto itr = s.begin(); itr !=s.end(); itr++) {
            c++;
            cout << "\t" << data[graph.nodeFromId(*itr)]->ToString() << endl;

            outputFile << data[graph.nodeFromId(*itr)]->ToString() << endl;

            int rgb[3] = {100,100,100};
            box(((currentYear - 1) * 190) + (currentQuarter * 60) + 10 + ((startQuarter - 1) * -50),(c * 71) + 10,40,15,rgb);

            int rgb2[3] = {255,255,100};
            string code = data[graph.nodeFromId(*itr)]->GetCourseCode();
            char credits = char(data[graph.nodeFromId(*itr)]->GetCredits() + 48);
            word(((currentYear - 1) * 190) + (currentQuarter * 60) + 13 + ((startQuarter - 1) * -50),(c * 71) + 12,code,rgb2);
            letter(((currentYear - 1) * 190) + (currentQuarter * 60) + 13 + ((startQuarter - 1) * -50),(c * 71) + 18,credits,rgb2);

            availableClasses[0].erase(*itr);
            availableClasses[1].erase(*itr);
            availableClasses[2].erase(*itr);
            for (SmartDigraph::OutArcIt a(graph, graph.nodeFromId(*itr)); a != INVALID; ++a) {
                vector<int> v = data[graph.target(a)]->GetQuarters();
                for (int j = 0; j < v.size(); j++) {
                    availableClasses[v[j] - 1].insert(graph.id(graph.target(a)));
                }
            }
        }
        currentQuarter++;
    }

    outputFile.close();
    // Destructor
    for (SmartDigraph::NodeIt n(graph); n != INVALID; ++n) {
        delete data[n];
    }

    ofstream img ("picture.ppm");
    img << "P3" << endl;
    img << width << " " << height << endl;
    img << "255" << endl;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int r = arr[x][y][0];
            int g = arr[x][y][1];
            int b = arr[x][y][2];

            img << r << " " << g << " " << b << endl;
        }
    }
    img.close();

    return 0;
};

/* This function is given a string input and pulls chunks of the string out with  getline delimiters.
 * It sends the courseCode, name, credits, requirements, and quarters offered into the CourseNode constructor
 * to create a new CourseNode object.
 * Returns a pointer to the CourseNode object that is created
 */
CourseNode* createNode(string input) {
    stringstream stream(input);
    string courseCode;
    string name;
    // int credits;
    string creditString;
    string reqs;
    vector<string> requirements;
    string terms;
    vector<int> quarters;

    getline(stream, courseCode, ',');
    getline(stream, name, ',');
    name.erase(0,1);
    getline(stream, creditString, ',');
    creditString.erase(0,1);
    int credits = stoi(creditString);
    getline(stream, reqs, ']');
    // Now delete the opening bracket '[' amd then traverse through this string, delimited by commas
    reqs.erase(0,2);
    // Create a function for this that returns a vector. Can't make a function since the vectors hold different types
    stringstream reqStream(reqs);
    string course;
    int count = 0;
    while (!reqStream.eof()) {
        getline(reqStream, course, ',');
        if (count != 0) {
            // Remove the space before each course, except for the first course, which has no space before it
            course.erase(0,1);
        }
        count++;
        requirements.push_back(course);
    }

    getline(stream, terms, ']');
    terms.erase(0,3);
    // Create a function for this that returns a vector. Can't make a function since the vectors hold different types
    stringstream termStream(terms);
    string quarter;
    while (!termStream.eof()) {
        getline(termStream, quarter, ',');
        int temp = stoi(quarter);
        quarters.push_back(temp);
    }

    CourseNode* node = new CourseNode(courseCode, name, credits, requirements, quarters);
    //cout << node->ToString() << endl;

    return node;
}

// Greedy algorithm that picks the classes based on their order of priority.
set<int> pickClasses(set<int> s, int maxCredits, int year) {
    int creditsLeft = maxCredits;
    set<int> retSet; // Return Set
    while(true) {
        int bestCourse = -1;

        // Iterate through every class in the set of available classes, test if they are under the current creditsLeft, and then set its ID as bestCourse if it has the highest priority so far
        set<int>::iterator itr;
        for (itr = s.begin(); itr != s.end(); itr++) {
            // This stops CSC 3000 and CSC 4941 from being taken before they should be taken
            if (data[graph.nodeFromId(*itr)]->GetCourseCode() != "CSC 3000" && data[graph.nodeFromId(*itr)]->GetCourseCode() != "CSC 4941") {
                if (bestCourse == -1 || data[graph.nodeFromId(*itr)]->GetPriority() > data[graph.nodeFromId(bestCourse)]->GetPriority()) {
                    if (data[graph.nodeFromId(*itr)]->GetCredits() <= creditsLeft) {
                        bestCourse = *itr; // Setting bestCourse as class ID with the highest priority out of possible classes searched.
                    }
                }
            } else if (year >= 3 && data[graph.nodeFromId(*itr)]->GetCourseCode() == "CSC 3000") {
                // This makes sure that CSC 3000 is only taken on the 3rd year or after
                if (bestCourse == -1 || data[graph.nodeFromId(*itr)]->GetPriority() > data[graph.nodeFromId(bestCourse)]->GetPriority()) {
                    if (data[graph.nodeFromId(*itr)]->GetCredits() <= creditsLeft) {
                        bestCourse = *itr;
                    }
                }
            } else if (year >= 4 && data[graph.nodeFromId(*itr)]->GetCourseCode() == "CSC 4941") {
                // This makes sure that CSC 4941 is only taken on the 4th year or after
                if (bestCourse == -1 || data[graph.nodeFromId(*itr)]->GetPriority() > data[graph.nodeFromId(bestCourse)]->GetPriority()) {
                    if (data[graph.nodeFromId(*itr)]->GetCredits() <= creditsLeft) {
                        bestCourse = *itr;
                    }
                }
            }
        }
        if (bestCourse == -1) { // Only occurs when the iterator loops through all classes and it can't take any more classes.
            return retSet;
        }
        retSet.insert(bestCourse);
        s.erase(bestCourse);
        creditsLeft -= data[graph.nodeFromId(bestCourse)]->GetCredits();
    }
}

// Recursive DFS that counts the priority of each node as it traverses back up the graph.
void assignPriority(int id) {
    SmartDigraph::Node node = graph.nodeFromId(id);

    // If the node's priority was already set using a previous call of assignPriority() on a different root node, don't traverse that path of classes again.
    // This works because -1 is the default priority of a class, it means it's priority hasn't been generated yet.
    if (data[node]->GetPriority() > -1) { // DYNAMIC PROGRAMMING!!!!
        return;
    }

    // Call assignPriority() on every child node.
    int max = -1;
    for (SmartDigraph::OutArcIt a(graph, node); a != INVALID; ++a) {
        SmartDigraph::Node target = graph.target(a);
        assignPriority(graph.id(target)); // Recursive call

        // Picking the child node with the greatest priority, and basing the priority of the node entered off of that one.
        if (data[target]->GetPriority() > max) {
            max = data[target]->GetPriority();
        }
    }

    // for loop is skipped if there are no children, since max is set to -1, priority is set to 0. This is the base case.
    data[node]->SetPriority(max + 1);
}

// Currently un-used
vector<CourseNode*> mergeSort(vector<CourseNode*> v) {
    cout << v.size() << endl;
    // Base Case
    if (v.size() == 1) {
        return v;
    }

    // Assign v1 = v[0] to v[n/2] and v2 = v[n/2 + 1] to v[n]
    //v1 = mergeSort(v1);
    //v2 = mergeSort(v2);

    // Merging both sides
}

void box(int x, int y, int width, int height, int rgb[3]) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            arr[x + i][y + j][0] = rgb[0];
            arr[x + i][y + j][1] = rgb[1];
            arr[x + i][y + j][2] = rgb[2];
        }
    }
}

void line(int start_x, int start_y, int end_x, int end_y, int width, int rgb[3]) {
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
            arr[x][y][0] = rgb[0];
            arr[x][y][1] = rgb[1];
            arr[x][y][2] = rgb[2];
        }
        sy += y_segments - 1;
    }
    arr[start_x][start_y][0] = 0;
    arr[start_x][start_y][1] = 0;
    arr[start_x][start_y][2] = 0;
    arr[end_x][end_y][0] = 0;
    arr[end_x][end_y][1] = 0;
    arr[end_x][end_y][2] = 0;
}

void word(int x, int y, string s, int rgb[3]){
    for (int i = 0; i < s.size(); i++) {
        letter(x + (i * 4),y,s.at(i),rgb);
    }
}

void letter(int x, int y, char letter, int rgb[3]) {
    int letArr[5][3];
    switch(int(letter)) {
        case 32: // Space
        {
            int a[5][3] = {0, 0, 0,
                           0, 0, 0,
                           0, 0, 0,
                           0, 0, 0,
                           0, 0, 0};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 48: // Number 0
        {
            int a[5][3] = {0, 1, 0,
                           1, 0, 1,
                           1, 1, 1,
                           1, 0, 1,
                           0, 1, 0};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 49: // Number 1
        {
            int a[5][3] = {0, 1, 0,
                           1, 1, 0,
                           0, 1, 0,
                           0, 1, 0,
                           1, 1, 1};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 50: // Number 2
        {
            int a[5][3] = {0, 1, 0,
                           1, 0, 1,
                           0, 0, 1,
                           1, 1, 0,
                           1, 1, 1};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 51: // Number 3
        {
            int a[5][3] = {1, 1, 0,
                           0, 0, 1,
                           1, 1, 0,
                           0, 0, 1,
                           1, 1, 0};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 52: // Number 4
        {
            int a[5][3] = {1, 0, 1,
                           1, 0, 1,
                           1, 1, 1,
                           0, 0, 1,
                           0, 0, 1};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 53: // Number 5
        {
            int a[5][3] = {1, 1, 1,
                           1, 0, 0,
                           1, 1, 0,
                           0, 0, 1,
                           1, 1, 1};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 54: // Number 6
        {
            int a[5][3] = {0, 1, 1,
                           1, 0, 0,
                           1, 1, 0,
                           1, 0, 1,
                           1, 1, 1};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 55: // Number 7
        {
            int a[5][3] = {1, 1, 1,
                           0, 0, 1,
                           0, 1, 0,
                           0, 1, 0,
                           0, 1, 0};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 56: // Number 8
        {
            int a[5][3] = {0, 1, 0,
                           1, 0, 1,
                           0, 1, 0,
                           1, 0, 1,
                           0, 1, 0};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 57: // Number 9
        {
            int a[5][3] = {0, 1, 1,
                           1, 0, 1,
                           0, 1, 1,
                           0, 0, 1,
                           1, 1, 0};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 65: // Letter A
        {
            int a[5][3] = {0, 1, 0,
                           1, 0, 1,
                           1, 1, 1,
                           1, 0, 1,
                           1, 0, 1};
            memcpy(letArr, a, sizeof(letArr));
            break;
        }
        case 66: // Letter B
        {
            int b[5][3] = {1, 1, 0,
                           1, 0, 1,
                           1, 1, 0,
                           1, 0, 1,
                           1, 1, 0};
            memcpy(letArr, b, sizeof(letArr));
            break;
        }
        case 67: // Letter C
        {
            int c[5][3] = {1, 1, 1,
                           1, 0, 0,
                           1, 0, 0,
                           1, 0, 0,
                           1, 1, 1};
            memcpy(letArr, c, sizeof(letArr));
            break;
        }
        case 68: // Letter D
        {
            int d[5][3] = {1, 1, 0,
                           1, 0, 1,
                           1, 0, 1,
                           1, 0, 1,
                           1, 1, 0};
            memcpy(letArr, d, sizeof(letArr));
            break;
        }
        case 69: // Letter E
        {
            int e[5][3] = {1, 1, 1,
                           1, 0, 0,
                           1, 1, 1,
                           1, 0, 0,
                           1, 1, 1};
            memcpy(letArr, e, sizeof(letArr));
            break;
        }
        case 70: // Letter F
        {
            int f[5][3] = {1, 1, 1,
                           1, 0, 0,
                           1, 1, 1,
                           1, 0, 0,
                           1, 0, 0};
            memcpy(letArr, f, sizeof(letArr));
            break;
        }
        case 71: // Letter G
        {
            int g[5][3] = {1, 1, 1,
                           1, 0, 0,
                           1, 0, 1,
                           1, 0, 1,
                           1, 1, 1};
            memcpy(letArr, g, sizeof(letArr));
            break;
        }
        case 72: // Letter H
        {
            int g[5][3] = {1, 0, 1,
                           1, 0, 1,
                           1, 1, 1,
                           1, 0, 1,
                           1, 0, 1};
            memcpy(letArr, g, sizeof(letArr));
            break;
        }
        case 73: // Letter I
        {
            int i[5][3] = {1, 1, 1,
                           0, 1, 0,
                           0, 1, 0,
                           0, 1, 0,
                           1, 1, 1};
            memcpy(letArr, i, sizeof(letArr));
            break;
        }
        case 77: // Letter M
        {
            int m[5][3] = {1, 0, 1,
                           1, 1, 1,
                           1, 1, 1,
                           1, 0, 1,
                           1, 0, 1};
            memcpy(letArr, m, sizeof(letArr));
            break;
        }
        case 78: // Letter N
        {
            int n[5][3] = {1, 0, 1,
                           1, 1, 1,
                           1, 1, 1,
                           1, 1, 1,
                           1, 0, 1};
            memcpy(letArr, n, sizeof(letArr));
            break;
        }
        case 79: // Letter O
        {
            int o[5][3] = {1, 1, 1,
                           1, 0, 1,
                           1, 0, 1,
                           1, 0, 1,
                           1, 1, 1};
            memcpy(letArr, o, sizeof(letArr));
            break;
        }
        case 80: // Letter P
        {
            int p[5][3] = {1, 1, 0,
                           1, 0, 1,
                           1, 1, 0,
                           1, 0, 0,
                           1, 0, 0};
            memcpy(letArr, p, sizeof(letArr));
            break;
        }
        case 82: // Letter R
        {
            int r[5][3] = {1, 1, 0,
                           1, 0, 1,
                           1, 1, 0,
                           1, 0, 1,
                           1, 0, 1};
            memcpy(letArr, r, sizeof(letArr));
            break;
        }
        case 83: // Letter S
        {
            int s[5][3] = {1, 1, 1,
                           1, 0, 0,
                           1, 1, 1,
                           0, 0, 1,
                           1, 1, 1};
            memcpy(letArr, s, sizeof(letArr));
            break;
        }
        case 84: // Letter T
        {
            int t[5][3] = {1, 1, 1,
                           0, 1, 0,
                           0, 1, 0,
                           0, 1, 0,
                           0, 1, 0};
            memcpy(letArr, t, sizeof(letArr));
            break;
        }
        case 85: // Letter U
        {
            int u[5][3] = {1, 0, 1,
                           1, 0, 1,
                           1, 0, 1,
                           1, 0, 1,
                           1, 1, 1};
            memcpy(letArr, u, sizeof(letArr));
        }
            break;
        case 87: // Letter W
        {
            int w[5][3] = {1, 0, 1,
                           1, 0, 1,
                           1, 1, 1,
                           1, 1, 1,
                           1, 0, 1};
            memcpy(letArr, w, sizeof(letArr));
            break;
        }
        case 89: // Letter Y
        {
            int y[5][3] = {1, 0, 1,
                           1, 0, 1,
                           0, 1, 0,
                           0, 1, 0,
                           0, 1, 0};
            memcpy(letArr, y, sizeof(letArr));
            break;
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            if (letArr[j][i] == 1) {
                arr[x + i][y + j][0] = rgb[0];
                arr[x + i][y + j][1] = rgb[1];
                arr[x + i][y + j][2] = rgb[2];
            }
        }
    }
}