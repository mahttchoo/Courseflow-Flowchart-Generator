#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "coursenode.h"

//#include <lemon/list_graph.h>
#include <lemon/smart_graph.h>

using namespace std;
using namespace lemon;


CourseNode* createNode(string input);

int main() {
    // Example to test CourseNode
//    string input = "CSC 3430, Algorithms Analysis and Design, 4, [MAT 2200, CSC 2431], [2]";
//    CourseNode* myNode = createNode(input);
//    cout << myNode->ToString() << endl;

    // Create the graph
    SmartDigraph graph;
    SmartDigraph::NodeMap<CourseNode*> data(graph);


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
            // Add node to graph here
            SmartDigraph::Node n = graph.addNode();
            data[n] = course;
        }
    } else {
        cout << "File could not be opened." << endl;
    }
    readFile.close();

    // Step through the graph
    int count = 0;
    for (SmartDigraph::NodeIt n(graph); n != INVALID; ++n) {
        count++;
        // cout << "value for n is: " << graph.id(n) << endl;
        cout << "value for n is: " << data[n]->ToString() << endl;
    }
    std::cout << "Number of nodes: " << count << " and nodeNum is " << graph.nodeNum() << std::endl;

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
                     cout << "Arc between " << data[j]->ToString() << " and " << data[n]->ToString() << endl;
                     SmartDigraph::Arc a = graph.addArc(j, n);
                     arcCount++;
                 }
             }
         }
    }

    // Now iterate through the arcs to make sure they are all added
    int cnt = 0;
    for (SmartDigraph::ArcIt a(graph); a != INVALID; ++a) {
        cnt++;
    }
    cout << "The number of arcs that should have been added is: " << arcCount << endl;
    cout << "Number of arcs: " << cnt << std::endl;
    cout << "Number of arcs using countArcs: " << countArcs(graph) << endl;

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