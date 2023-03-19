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
#include "coursenode.h"
#include <lemon/smart_graph.h>
#include <lemon/list_graph.h>

using namespace std;
using namespace lemon;

CourseNode* createNode(string input, SmartDigraph::NodeMap<CourseNode*>& data, SmartDigraph& graph); // This should porbably be a constructor in the courseNode.cpp file.
set<int> pickClasses(set<int> s, int maxCredits, int year, SmartDigraph::NodeMap<CourseNode*>& data, SmartDigraph& graph); // Returns optimal set of classes that is under the max credits given a set of classes.
void assignPriority(int id, SmartDigraph::NodeMap<CourseNode*>& data, SmartDigraph& graph);

int main() {
    SmartDigraph graph;
    SmartDigraph::NodeMap<CourseNode*> data(graph);


    set<int> availableClasses[3]; // Array of sets of node ids of courses that are available. Array index determines quarter.
    set<int> finalClasses[4][3];

    string line;
    ifstream readFile;

    string inputFile;
    bool open = false;
    cout << "Please enter a .txt file containing the required courses for a major." << endl;
    while (!readFile.is_open()) {
        cin >> inputFile;
        try {
            // Use "../" + inputFile to run in CLion, and inputFile to run in ubuntu
            readFile.open(inputFile);
        } catch (const ifstream::failure& e) {
            cout << "Please enter a .txt file that is in this folder." << endl;
        }
//        open = true;
    }
    cout << "Please enter a .txt file containing the required courses for a major." << endl;
    cin >> inputFile;

    if (readFile.is_open()) {
        // TODO: define vector here
        while (!readFile.eof()) {
            getline(readFile, line);
            // Create the node with the given line that contains a course
            CourseNode* course = createNode(line, data, graph);
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

    // TODO: This is what causes O(n^2) time complexity, change later for O(nlogn)
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
                 }
             }
         }
    }

    // Asking the user start quarter and maximum number of credits
    int maxCredits = 0;
    int startQuarter = 0;
    string input;

    // Take the user input as a string, then try to convert it to an integer. If this doesn't work,
    // tell the user to input an integer. The input must be between 5 and 18 credits
    while (maxCredits < 5 || maxCredits > 18) {
        cout << "Please enter the maximum number of credits you would like to take per quarter (5 - 18)" << endl;
        cin >> input;
        try {
            maxCredits = stoi(input);
        } catch (exception &err) {
            cout << "Please enter an integer." << endl;
        }
    }
    // The input must be 1, 2, or 3 for the quarter selection. Anything else won't work, and the user
    // must try again.
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
    cout << "\nYou will take no more than " << maxCredits << " classes per quarter and are starting in quarter ";
    cout << startQuarter << "." << endl << endl;

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

    for (set<int>::iterator itr = rootCourses.begin(); itr != rootCourses.end(); itr++) {
        assignPriority(*itr, data, graph);
    }

    // File output to .txt file to be read and used in python to create a display of the classes
    // Use ../output.txt in CLion, and output.txt for ubuntu
    ofstream outputFile("output.txt");

    int currentYear = 1;
    cout << "Year 1:" << endl;
    int currentQuarter = startQuarter - 1;
    while (!availableClasses[0].empty() || !availableClasses[1].empty() || !availableClasses[2].empty()) {
        // Each time that the quarter is 3 (quarters are 0, 1, 2), another year has gone by
        if (currentQuarter == 3) {
            currentYear++;
            cout << endl << "Year " << currentYear << ":" << endl;
        }

        currentQuarter = currentQuarter % 3;
        set<int> s = pickClasses(availableClasses[currentQuarter], maxCredits, currentYear, data, graph);

        cout << "\nCLASSES TO TAKE DURING QUARTER " << currentQuarter + 1 << ":" << endl;

        outputFile << "Quarter " << currentQuarter + 1 << endl;

        for (auto itr = s.begin(); itr !=s.end(); itr++) {
            cout << "\t" << data[graph.nodeFromId(*itr)]->ToString() << endl;

            outputFile << data[graph.nodeFromId(*itr)]->ToString() << endl;

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

    return 0;
};

/* This function is given a string input and pulls chunks of the string out with  getline delimiters.
 * It sends the courseCode, name, credits, requirements, and quarters offered into the CourseNode constructor
 * to create a new CourseNode object.
 * Returns a pointer to the CourseNode object that is created
 */
CourseNode* createNode(string input, SmartDigraph::NodeMap<CourseNode*>& data, SmartDigraph& graph) {
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

set<int> pickClasses(set<int> s, int maxCredits, int year, SmartDigraph::NodeMap<CourseNode*>& data, SmartDigraph& graph) {
    int creditsLeft = maxCredits;
    set<int> retSet;
    while(true) {
        int bestCourse = -1;
        set<int>::iterator itr;
        for (itr = s.begin(); itr != s.end(); itr++) {
            // This stops CSC 3000 and CSC 4941 from being taken before they should be taken
            if (data[graph.nodeFromId(*itr)]->GetCourseCode() != "CSC 3000" &&
            data[graph.nodeFromId(*itr)]->GetCourseCode() != "CSC 4941" &&
            data[graph.nodeFromId(*itr)]->GetCourseCode() != "CHM 4899") {
                if (bestCourse == -1 || data[graph.nodeFromId(*itr)]->GetPriority() >
                data[graph.nodeFromId(bestCourse)]->GetPriority()) {
                    if (data[graph.nodeFromId(*itr)]->GetCredits() <= creditsLeft) {
                        bestCourse = *itr;
                    }
                }
            } else if (year >= 3 && data[graph.nodeFromId(*itr)]->GetCourseCode() == "CSC 3000") {
                // This makes sure that CSC 3000 is only taken on the 3rd year or after
                if (bestCourse == -1 || data[graph.nodeFromId(*itr)]->GetPriority() >
                data[graph.nodeFromId(bestCourse)]->GetPriority()) {
                    if (data[graph.nodeFromId(*itr)]->GetCredits() <= creditsLeft) {
                        bestCourse = *itr;
                    }
                }
            } else if (year >= 4 && (data[graph.nodeFromId(*itr)]->GetCourseCode() == "CSC 4941" ||
            data[graph.nodeFromId(*itr)]->GetCourseCode() == "CHM 4899")) {
                // This makes sure that CSC 4941 or CHM 4899 is only taken on the 4th year or after
                if (bestCourse == -1 || data[graph.nodeFromId(*itr)]->GetPriority() >
                data[graph.nodeFromId(bestCourse)]->GetPriority()) {
                    if (data[graph.nodeFromId(*itr)]->GetCredits() <= creditsLeft) {
                        bestCourse = *itr;
                    }
                }
            }
        }
        if (bestCourse == -1) {
            return retSet;
        }
        retSet.insert(bestCourse);
        s.erase(bestCourse);
        creditsLeft -= data[graph.nodeFromId(bestCourse)]->GetCredits();
    }
}

void assignPriority(int id, SmartDigraph::NodeMap<CourseNode*>& data, SmartDigraph& graph) {
    SmartDigraph::Node node = graph.nodeFromId(id);
    if (data[node]->GetPriority() > -1) {
        return;
    }
    int max = -1;
    for (SmartDigraph::OutArcIt a(graph, node); a != INVALID; ++a) {
        assignPriority(graph.id(graph.target(a)), data, graph);
        if (data[graph.target(a)]->GetPriority() > max) {
            max = data[graph.target(a)]->GetPriority();
        }
    }
    data[node]->SetPriority(max + 1);
}