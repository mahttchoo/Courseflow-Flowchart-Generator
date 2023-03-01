#include <string>
#include <vector>

using namespace std;


#ifndef FLOWCHART_THE_FELLOWSHIP_COURSENODE_H
#define FLOWCHART_THE_FELLOWSHIP_COURSENODE_H


class CourseNode {
    string _courseCode;
    string _name;
    size_t _credits;
    vector<string> _requirements;
    vector<size_t> _quarters;

public:
    CourseNode(string code, string name, size_t credits, vector<string> reqs, vector<size_t> quarters);
    string ToString()const;
    string GetCourseCode()const;
    string GetName()const;
    size_t GetCredits()const;
    vector<string> GetRequirements()const;
    vector<size_t> GetQuarters()const;
};


#endif //FLOWCHART_THE_FELLOWSHIP_COURSENODE_H
