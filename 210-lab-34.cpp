//COMSC-210 | Lab 34 | Noel Mier-Luna
#include <iostream>
#include <vector>
using namespace std;

const int SIZE = 11; // Reduced to 11 to avoid empty vertices in adjacency list.

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

        // Depth-First Search
    void DFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << v << " ";

        for (auto &neighbor : adjList[v]) {
            int next = neighbor.first;
            if (!visited[next])
                DFSUtil(next, visited);
        }
    }

    // Public DFS
    void DFS(int start) {
        vector<bool> visited(SIZE, false);
        cout << "DFS starting at " << start << ": ";
        DFSUtil(start, visited);
        cout << endl;
    }

    // Breadth-First Search
    void BFS(int start) {
        vector<bool> visited(SIZE, false);
        vector<int> queue;

        visited[start] = true;
        queue.push_back(start);

        cout << "BFS starting at " << start << ": ";

        int index = 0;
        while (index < queue.size()) {
            int v = queue[index++];
            cout << v << " ";

            for (auto &neighbor : adjList[v]) {
                int next = neighbor.first;
                if (!visited[next]) {
                    visited[next] = true;
                    queue.push_back(next);
                }
            }
        }
        cout << endl;
    }


    // Print the graph's adjacency list.
    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }
};

int main() {
    // Creates a vector of graph edges/weights.
    vector<Edge> edges = {
    {0,1,10}, {0,2,7},
    {1,3,6}, {2,4,9},
    {3,5,4}, {5,6,3},
    {6,7,11}, {7,8,5},
    {8,9,8}, {9,10,12},
    {10,4,14}, {6,1,5}
};



    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();
    // Performs DFS and BFS starting from vertex 0
    graph.DFS(0);
    graph.BFS(0);


    return 0;
}