#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "coursenode.h"
//#include <boost/algorithm/string.hpp>
//
//using namespace boost::algorithm;
//#include <boost/algorithm/string/trim.hpp>

using namespace std;

CourseNode* createNode(string input);

int main() {
    /* Figure out how to read in the courses from the txt file and then store them as nodes in the graph.
     * I need to work with the file input
     * I should use getline to step through the file for one course at a time
     */
    // Example to test CourseNode
    //string input = "CSC 3430, Algorithms Analysis and Design, 4, [MAT 2200, CSC 2431], [2]";

    // Give an option to select the file later. Might have to use command line parameters
    string line;
    ifstream readFile;
    readFile.open("../major1.txt");
    if (readFile.is_open()) {
        while (!readFile.eof()) {
            getline(readFile, line);
            cout << createNode(line)->ToString() << endl;
        }
    }


    // Step through the string and grab one piece at a time comma delimited

    return 0;
};

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
    getline(stream, creditString, ',');
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