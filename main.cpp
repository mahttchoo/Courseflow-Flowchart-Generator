#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main() {
    /* Figure out how to read in the courses from the txt file and then store them as nodes in the graph.
     * I need to work with the file input
     * I should use getline to step through the file for one course at a time
     */
    // Example to test CourseNode
    string input = "CSC 3430, Algorithms Analysis and Design, 4, [MAT 2200, CSC 2431], [2]";
    // Step through the string and grab one piece at a time comma delimited
    stringstream stream(input);
    string courseCode;
    string name;
    size_t credits;
    string reqs;
    vector<string> requirements;
    vector<size_t> quarters;

    while (stream.good()) {
        getline(stream, courseCode, ',');
        getline(stream, name, ',');
        getline(stream, string_to_size_t(credits), ',');
        //credits = string_to_size_t(creditString);
        getline(stream, reqs, ']');
        // Now delete the opening bracket '[' amd then traverse through this string, delimited by commas
        // Create a function for this that returns a vector
        getline(stream, courseCode, ',');
    }
    cout << "Hello world" << endl;
    return 0;
};