#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <lemon/smart_graph.h>
#include "coursenode.h"

using namespace std;
using namespace lemon;

CourseNode* createNode(string input);

int main() {
    SmartDigraph graph;
    SmartDigraph::NodeMap<CourseNode*> data(graph);
    SmartDigraph::Node node1 = graph.addNode();

    /* Figure out how to read in the courses from the txt file and then store them as nodes in the graph.
     * I need to work with the file input
     * I should use getline to step through the file for one course at a time
     */
    // Example to test CourseNode
    //string input = "CSC 3430, Algorithms Analysis and Design, 4, [MAT 2200, CSC 2431], [2]";

    // Give an option to select the file later. Might have to use command line parameters
    string line;
    ifstream readFile;
    // Will change this to open the user selected file
    readFile.open("../major1.txt");
    if (readFile.is_open()) {
        while (!readFile.eof()) {
            getline(readFile, line);
            // Create the node with the given line that contains a course
            CourseNode* course = createNode(line);
            data[node1] = course;
            cout << data[node1]->ToString() << endl;
            cout << createNode(line)->ToString() << endl;
        }
    } else {
        cout << "File could not be opened." << endl;
    }

    readFile.close();
    // Step through the string and grab one piece at a time comma delimited

    // Making the graph
//    typedef adjacency_list<vecS, vecS, directedS, no_property, no_property> Graph;
//    Graph g;
//    unsigned long v1 = add_vertex(g);
//    unsigned long v2 = add_vertex(g);
//    cout << "v1: " + v1 << endl;
//    cout << "v2: " + v2 << endl;

    return 0;
};

/* This function is given a string input and pulls chunks of the string out with  gtline delimiters.
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
    while (!reqStream.eof()) {
        getline(reqStream, course, ',');
        // Remove the space before each course
//        course.erase(0,1);
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