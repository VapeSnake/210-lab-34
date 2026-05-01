// COMSC-210 | Lab 34 | Noel Mier-Luna
#include <iostream>
#include <vector>
#include <queue> // For Dijkstra's Algorithm
#include <algorithm> // For sorting edges in Kruskal's Algorithm
using namespace std;

const int SIZE = 11; // Reduced to 11 to avoid empty vertices in adjacency list.

// Represents a network cable between two devices
struct NetworkLink
{                // originally: struct Edge
    int deviceA; // originally: src
    int deviceB; // originally: dest
    int latency; // originally: weight
};

typedef pair<int, int> Connection; // originally Pair (device, latency)

class NetworkGraph
{ // originally: class Graph
public:
    vector<vector<Connection>> adjacencyList; // originally adjList

    // Constructor builds the network topology
    NetworkGraph(vector<NetworkLink> const &links)
    { // originally edges
        adjacencyList.resize(SIZE);

        for (auto &link : links)
        {
            int a = link.deviceA; // originally src
            int b = link.deviceB; // originally dest
            int w = link.latency; // originally weight

            adjacencyList[a].push_back({b, w});
            adjacencyList[b].push_back({a, w}); // undirected network cable
        }
    }

    // Depth-first route scan (DFS)
    void deepRouteScan(int device, vector<bool> &visited)
    { // originally DFSUtil
        visited[device] = true;
        cout << device << " ";

        for (auto &conn : adjacencyList[device])
        {
            int nextDevice = conn.first; // originally neighbor.first
            if (!visited[nextDevice])
                deepRouteScan(nextDevice, visited);
        }
    }

    // Public DFS wrapper
    void runDeepScan(int startDevice)
    { // originally DFS
        vector<bool> visited(SIZE, false);
        cout << "Deep Route Scan starting at device " << startDevice << ": ";
        deepRouteScan(startDevice, visited);
        cout << endl;
    }

    // Minimum-hop routing (BFS)
    void runMinHopRoute(int startDevice)
    { // originally BFS
        vector<bool> visited(SIZE, false);
        vector<int> queue;

        visited[startDevice] = true;
        queue.push_back(startDevice);

        cout << "Minimum-Hop Route starting at device " << startDevice << ": ";

        int index = 0;
        while (index < queue.size())
        {
            int device = queue[index++];
            cout << device << " ";

            for (auto &conn : adjacencyList[device])
            {
                int nextDevice = conn.first;
                if (!visited[nextDevice])
                {
                    visited[nextDevice] = true;
                    queue.push_back(nextDevice);
                }
            }
        }
        cout << endl;
    }

    // Print network topology
    void printNetwork()
    { // originally printGraph
        cout << "Network Topology (Adjacency List):" << endl;
        for (int i = 0; i < adjacencyList.size(); i++)
        {
            cout << "Device " << i << " --> ";
            for (Connection c : adjacencyList[i])
                cout << "(Device " << c.first << ", latency " << c.second << "ms) ";
            cout << endl;
        }
    }

    // Shortest-path routing (Dijkstra's Algorithm)
    void runShortestPath(int startDevice)
    { // NEW METHOD
        const int INF = 1e9;

        vector<int> minLatency(SIZE, INF); // originally: distance[]
        minLatency[startDevice] = 0;

        // Min-heap priority queue: (latency, device)
        priority_queue<
            pair<int, int>,
            vector<pair<int, int>>,
            greater<pair<int, int>>>
            pq;

        pq.push({0, startDevice});

        while (!pq.empty())
        {
            auto [currentLatency, device] = pq.top();
            pq.pop();

            if (currentLatency > minLatency[device])
                continue;

            for (auto &conn : adjacencyList[device])
            {
                int nextDevice = conn.first;   // originally neighbor.first
                int edgeLatency = conn.second; // originally weight

                if (minLatency[device] + edgeLatency < minLatency[nextDevice])
                {
                    minLatency[nextDevice] = minLatency[device] + edgeLatency;
                    pq.push({minLatency[nextDevice], nextDevice});
                }
            }
        }

        // Print results
        cout << "Shortest path from device " << startDevice << ":" << endl;
        for (int i = 0; i < SIZE; i++)
        {
            cout << startDevice << " -> " << i << " : " << minLatency[i] << endl;
        }
    }
    int findSet(int device, vector<int> &parent)
    {
        if (parent[device] == device)
            return device;
        return parent[device] = findSet(parent[device], parent); // path compression
    }

    void unionSet(int a, int b, vector<int> &parent, vector<int> &rank)
    {
        a = findSet(a, parent);
        b = findSet(b, parent);

        if (a != b)
        {
            if (rank[a] < rank[b])
                swap(a, b);
            parent[b] = a;
            if (rank[a] == rank[b])
                rank[a]++;
        }
    }

    // --- Minimum Spanning Tree (Kruskal's Algorithm) ---
    void runMinimumSpanningTree()
    { // NEW METHOD
        struct MSTEdge
        {
            int deviceA; // originally src
            int deviceB; // originally dest
            int latency; // originally weight
        };

        vector<MSTEdge> allLinks;

        // Convert adjacency list to edge list
        for (int device = 0; device < SIZE; device++)
        {
            for (auto &conn : adjacencyList[device])
            {
                int neighbor = conn.first;
                int latency = conn.second;

                if (device < neighbor)
                {
                    // avoid duplicates (undirected graph)
                    allLinks.push_back({device, neighbor, latency});
                }
            }
        }

        // Sort edges by latency (ascending)
        sort(allLinks.begin(), allLinks.end(),
             [](const MSTEdge &a, const MSTEdge &b)
             {
                 return a.latency < b.latency;
             });

        // Prepare DSU
        vector<int> parent(SIZE), rank(SIZE, 0);
        for (int i = 0; i < SIZE; i++)
            parent[i] = i;

        cout << "Minimum Spanning Tree edges:" << endl;

        // Kruskal's algorithm
        for (auto &link : allLinks)
        {
            int a = link.deviceA;
            int b = link.deviceB;

            if (findSet(a, parent) != findSet(b, parent))
            {
                unionSet(a, b, parent, rank);

                cout << "Edge from " << a
                     << " to " << b
                     << " with capacity: " << link.latency
                     << " units" << endl;
            }
        }
    }
};

int main()
{
    // Creates a vector of  network links (edges) with latency (weight)
    vector<NetworkLink> links = {
        {0, 1, 10}, {0, 2, 7}, {1, 3, 6}, {2, 4, 9}, {3, 5, 4}, {5, 6, 3}, {6, 7, 11}, {7, 8, 5}, {8, 9, 8}, {9, 10, 12}, {10, 4, 14}, {6, 1, 5}};

    NetworkGraph network(links);

    network.printNetwork();
    network.runDeepScan(0);     // DFS from ServerRoom
    network.runMinHopRoute(0);  // BFS from ServerRoom
    network.runShortestPath(0); // Dijkstra's from ServerRoom
    network.runMinimumSpanningTree(); // Kruskal's for MST

    return 0;
}