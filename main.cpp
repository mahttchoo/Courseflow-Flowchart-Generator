#include <iostream>
//#include <GL/gl.h>
#include <algorithm>
#include <utility>
//#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

struct Vertex {
    string temp;
};
struct Edge {
    // idk
};

int main() {
    typedef adjacency_list<vecS, vecS, directedS, no_property, no_property> Graph;
    Graph g;
    unsigned long v1 = add_vertex(g);
    unsigned long v2 = add_vertex(g);
    cout << "v1: " + v1 << endl;
    cout << "v2: " + v2 << endl;
    return 0;
};

// https://www.youtube.com/watch?v=GSp2531Wti4&ab_channel=nerd_mmccoo
// https://www.boost.org/doc/libs/1_37_0/libs/graph/doc/adjacency_list.html