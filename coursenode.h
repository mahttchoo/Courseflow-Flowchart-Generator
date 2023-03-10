#include <string>
#include <vector>

using namespace std;


#ifndef FLOWCHART_THE_FELLOWSHIP_COURSENODE_H
#define FLOWCHART_THE_FELLOWSHIP_COURSENODE_H


class CourseNode {
    string _courseCode;
    string _name;
    int _credits;
    vector<string> _requirements;
    vector<int> _quarters;

public:
    CourseNode(string code, string name, int credits, vector<string> reqs, vector<int> quarters);
    string ToString()const;
    string GetCourseCode()const;
    string GetName()const;
    int GetCredits()const;
    vector<string> GetRequirements()const;
    vector<int> GetQuarters()const;
};

#endif //FLOWCHART_THE_FELLOWSHIP_COURSENODE_H