/*
 * TODO: Give an option to select the file later. Might have to use command line parameters.
 * TODO: Allow user to select which file they want to make a graph of.
 * TODO: Make sure there are no memory leaks.
 * TODO: Make createNode() a constructor within the class and not a function here?
 * TODO: Look into changing the file reader to using a try/catch block and throwing errors if the file isn't opened.
 * TODO: Sort the nodes in the graph so searching for pre-reqs takes O(logn) instead of O(n).
 * TODO: Add a check to make sure the maxCredits is above 5 (or maybe we might have 12 minimum).
 */

/*
 * ---- There are currently two main approaches to generating the flowchart ----
 * Approach 1: Generate a vector with all of the CourseNodes, sort the vector, then add each element to our graph
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
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include "coursenode.h"
#include <lemon/smart_graph.h>

using namespace std;
using namespace lemon;

CourseNode* createNode(string input); // This should porbably be a constructor in the courseNode.cpp file.
vector<CourseNode*> mergeSort(vector<CourseNode*> v);
set<int> pickClasses(set<int> s); // Returns optimal set of classes that is under the max credits given a set of classes.

int main() {
    SmartDigraph graph;
    SmartDigraph::NodeMap<CourseNode*> data(graph);

    set<int> availableClasses[3]; // Array of sets of node ids of courses that are available. Array index determines quarter.
    set<int> finalClasses[4][3];
    cout << availableClasses->size() << endl;
    int a[5];

    string line;
    ifstream readFile;
    readFile.open("../major1.txt"); // Will change this to open the user selected file.
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
    for (SmartDigraph::NodeIt n(graph); n != INVALID; ++n) {
        cout << "Node of id " << graph.id(n) << "         " << data[n]->ToString() << endl;
    }

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
                     cout << "Arc between:\n\t" << data[j]->ToString() << "\n\t" << data[n]->ToString() << endl;
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
    cout << "\nThe number of arcs that should have been added is: " << arcCount << endl;
    cout << "Number of arcs: " << cnt << std::endl;
    cout << "Number of arcs using countArcs: " << countArcs(graph) << endl;

    // Asking the user start quarter and maximum number of credits
    int maxCredits = 18;
    int startQuarter = 1;

    cout << "Please enter the maximum number of credits you would like to take per quarter (5 - 18)" << endl;
    //cin >> maxCredits;
    cout << "Please enter the quarter you are starting school" << endl;
    cout << "[1] for Autumn, [2] for Winter, and [3] for Spring" << endl;
    //cin >> startQuarter;
    cout << "\nYou will take no more than " << maxCredits << " per quarter." << endl;
    cout << "You are starting in quarter " << startQuarter << "." << endl;

    // Generating a Set of classes the user actually has available.
    for (SmartDigraph::NodeIt n(graph); n != INVALID; ++n) {
        CourseNode* course = data[n];
        if (course->GetRequirements()[0] == "") { // If number of pre-requirements is zero
            vector<int> v = course->GetQuarters(); // why this arrow and not .???
            for (int j = 0; j < v.size(); j++) {
                availableClasses[v[j] - 1].insert(graph.id(n));
            }
        }
    }

    for (int i = 0; i < 3; i++) { // Iterate through availableClasses for quarters 1, 2, and 3.
        cout << "\nClasses that a freshman student can take in quarter " << i + 1 << endl;
        set<int>::iterator itr;

        // Displaying set elements
        for (itr = availableClasses[i].begin(); itr != availableClasses[i].end(); itr++) {
            cout << "\t" << data[graph.nodeFromId(*itr)]->ToString() << endl;
        }
    }

    set<int> s = pickClasses(availableClasses[startQuarter]);


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

set<int> pickClasses(set<int> s) {

}