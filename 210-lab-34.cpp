//COMSC-210 | Lab 34 | Noel Mier-Luna
#include <iostream>
#include <vector>
using namespace std;

const int SIZE = 11; // Reduced to 11 to avoid empty vertices in adjacency list.

// Represents a network cable between two devices
struct NetworkLink {   // originally: struct Edge
    int deviceA;       // originally: src
    int deviceB;       // originally: dest
    int latency;       // originally: weight
};

typedef pair<int, int> Connection;  // originally Pair (device, latency)

class NetworkGraph {   // originally: class Graph
public:
    vector<vector<Connection>> adjacencyList;  // originally adjList

    // Constructor builds the network topology
    NetworkGraph(vector<NetworkLink> const &links) {  // originally edges
        adjacencyList.resize(SIZE);

        for (auto &link : links) {
            int a = link.deviceA;   // originally src
            int b = link.deviceB;   // originally dest
            int w = link.latency;   // originally weight

            adjacencyList[a].push_back({b, w});
            adjacencyList[b].push_back({a, w});  // undirected network cable
        }
    }

    // Depth-first route scan (DFS)
    void deepRouteScan(int device, vector<bool>& visited) {  // originally DFSUtil
        visited[device] = true;
        cout << device << " ";

        for (auto &conn : adjacencyList[device]) {
            int nextDevice = conn.first;  // originally neighbor.first
            if (!visited[nextDevice])
                deepRouteScan(nextDevice, visited);
        }
    }

    // Public DFS wrapper
    void runDeepScan(int startDevice) {  // originally DFS
        vector<bool> visited(SIZE, false);
        cout << "Deep Route Scan starting at device " << startDevice << ": ";
        deepRouteScan(startDevice, visited);
        cout << endl;
    }

    // Minimum-hop routing (BFS)
    void runMinHopRoute(int startDevice) {  // originally BFS
        vector<bool> visited(SIZE, false);
        vector<int> queue;

        visited[startDevice] = true;
        queue.push_back(startDevice);

        cout << "Minimum-Hop Route starting at device " << startDevice << ": ";

        int index = 0;
        while (index < queue.size()) {
            int device = queue[index++];
            cout << device << " ";

            for (auto &conn : adjacencyList[device]) {
                int nextDevice = conn.first;
                if (!visited[nextDevice]) {
                    visited[nextDevice] = true;
                    queue.push_back(nextDevice);
                }
            }
        }
        cout << endl;
    }

    // Print network topology
    void printNetwork() {  // originally printGraph
        cout << "Network Topology (Adjacency List):" << endl;
        for (int i = 0; i < adjacencyList.size(); i++) {
            cout << "Device " << i << " --> ";
            for (Connection c : adjacencyList[i])
                cout << "(Device " << c.first << ", latency " << c.second << "ms) ";
            cout << endl;
        }
    }
};

int main() {
    // Creates a vector of  network links (edges) with latency (weight)
    vector<NetworkLink> links = {
        {0,1,10}, {0,2,7},
        {1,3,6}, {2,4,9},
        {3,5,4}, {5,6,3},
        {6,7,11}, {7,8,5},
        {8,9,8}, {9,10,12},
        {10,4,14}, {6,1,5}
    };

    NetworkGraph network(links);

    network.printNetwork();
    network.runDeepScan(0);     // DFS from ServerRoom
    network.runMinHopRoute(0);  // BFS from ServerRoom

    return 0;
}