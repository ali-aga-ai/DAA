#include <bits/stdc++.h>
using namespace std;

uint64_t cliqueCount = 0;

class Graph {
public:
    int n; // number of vertices
    vector<vector<int>> adj;      // adjacency list (each list is kept sorted)
    vector<int> labels;           // index -> label mapping
    unordered_map<int, int> labelToId;  // label -> index mapping

    Graph() : n(0) {}

    // addVertex: Adds a vertex with the given integer label if it doesn't already exist.
    int addVertex(int label) {
        if (labelToId.find(label) == labelToId.end()){
            labelToId[label] = n;
            labels.push_back(label);
            adj.push_back(vector<int>());
            n++;
            return n - 1;
        }
        return labelToId[label];
    }

    // addEdge: Adds an edge between the two vertices with labels l1 and l2.
    // Undirected graph: add each vertex to the other's list.
    void addEdge(int l1, int l2) {
        int u = addVertex(l1);
        int v = addVertex(l2);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void sortAdjacency() {
        for (int i = 0; i < n; i++)
            sort(adj[i].begin(), adj[i].end());
    }
};

// Reads the graph from file using only the adjacency list representation.
void readGraph(const string &filename, Graph &graph) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error opening file " << filename << endl;
        exit(1);
    }
    int edges = 0;
    string line;
    while (getline(inFile, line)) {
        size_t pos = line.find_first_not_of(" \t");
        if (pos != string::npos && line[pos] == '#') // ignore commented lines
            continue;
        istringstream iss(line);
        int l1, l2;
        if (iss >> l1 >> l2) {
            edges++;
            if (edges % 11095298 == 0) {
                cout << "yeah" << endl;
            }
            graph.addEdge(l1, l2);
        }
    }
    graph.sortAdjacency();
    cout << "reading done" << endl;
    fflush(stdout);
}

// Utility function: set intersection of two sorted vectors.
vector<int> setIntersection(const vector<int>& a, const vector<int>& b) {
    vector<int> result;
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
    return result;
}

// Utility function: set difference a \ b for two sorted vectors.
vector<int> setDifference(const vector<int>& a, const vector<int>& b) {
    vector<int> result;
    set_difference(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
    return result;
}

// Computes the degeneracy ordering using only the adjacency list.
// This implementation uses a bucket sort approach on the vertex degrees.
vector<int> computeDegeneracyOrder(const Graph &graph) {
    int n = graph.n;
    vector<int> degree(n), order;
    vector<bool> removed(n, false);
    for (int i = 0; i < n; i++)
        degree[i] = graph.adj[i].size();
    int maxDeg = *max_element(degree.begin(), degree.end());
    vector<vector<int>> buckets(maxDeg + 1);
    for (int i = 0; i < n; i++)
        buckets[degree[i]].push_back(i);
    int removedCount = 0;
    while (removedCount < n) {
        int d = 0;
        while (d <= maxDeg && buckets[d].empty())
            d++;
        if (d > maxDeg)
            break;
        int v = buckets[d].back();
        buckets[d].pop_back();
        if (removed[v])
            continue;
        removed[v] = true;
        order.push_back(v);
        removedCount++;
        // For each neighbor u of v, if u is not removed then decrease its degree.
        for (int u : graph.adj[v]) {
            if (!removed[u]) {
                degree[u]--;
                buckets[degree[u]].push_back(u);
            }
        }
    }
    reverse(order.begin(), order.end());
    return order;
}

/*
   Bron–Kerbosch algorithm with pivoting using sorted vectors,
   mimicking the bitset version exactly:
     - Compute the pivot (from P ∪ X) that maximizes |P ∩ N(u)|
     - Compute the candidate set as: candidates = P \ N(pivot)
     - For each candidate v (after checking that v is still in P), recursively search with:
         R ∪ {v}, P ∩ N(v), X ∩ N(v)
     - After processing v, remove it from P and add it to X.
*/
void BronKerboschSet(const Graph &graph, vector<int> R, vector<int>& P, vector<int>& X) {
    if (P.empty() && X.empty()) {
        cliqueCount++;
        return;
    }
    // Pivot selection: choose u from P ∪ X that maximizes |P ∩ N(u)|
    vector<int> unionPX = P;
    unionPX.insert(unionPX.end(), X.begin(), X.end());
    int pivot = unionPX[0];
    int maxCount = setIntersection(P, graph.adj[pivot]).size();
    for (int u : unionPX) {
        int cnt = setIntersection(P, graph.adj[u]).size();
        if (cnt > maxCount) {
            maxCount = cnt;
            pivot = u;
        }
    }
    // Compute candidate set as: candidates = P \ N(pivot)
    vector<int> candidates = setDifference(P, graph.adj[pivot]);
    // Process each candidate as in the bitset version
    for (int v : candidates) {
        // Check if v is still in P (it might have been removed by an earlier iteration)
        if (find(P.begin(), P.end(), v) == P.end())
            continue;
        vector<int> newR = R;
        newR.push_back(v);
        vector<int> newP = setIntersection(P, graph.adj[v]);
        vector<int> newX = setIntersection(X, graph.adj[v]);
        BronKerboschSet(graph, newR, newP, newX);
        // After recursion: remove v from P and add it to X (mirroring bitset update)
        P.erase(remove(P.begin(), P.end(), v), P.end());
        X.push_back(v);
        sort(P.begin(), P.end());
        sort(X.begin(), X.end());
    }
}

// Runs the Bron–Kerbosch algorithm in degeneracy order using the set representation.
void BronKerboschDegeneracySet(const Graph &graph) {
    vector<int> degenOrder = computeDegeneracyOrder(graph);
    int n = graph.n;
    vector<int> orderIndex(n, -1);
    for (int i = 0; i < (int)degenOrder.size(); i++)
        orderIndex[degenOrder[i]] = i;
    for (int i = 0; i < (int)degenOrder.size(); i++) {
        int v = degenOrder[i];
        vector<int> P, X;
        for (int u : graph.adj[v]) {
            if (orderIndex[u] > i)
                P.push_back(u);
            else if (orderIndex[u] < i)
                X.push_back(u);
        }
        sort(P.begin(), P.end());
        sort(X.begin(), X.end());
        vector<int> R = {v};
        BronKerboschSet(graph, R, P, X);
    }
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    auto start = chrono::high_resolution_clock::now();
    Graph graph;
    // Read graph from file specified as the first command line argument.
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <graph_filename>" << endl;
        return 1;
    }
    readGraph(argv[1], graph);
    
    cliqueCount = 0;
    BronKerboschDegeneracySet(graph);
    
    cout << "Number of unique maximal cliques: " << cliqueCount << "\n";
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Elapsed time: " << duration.count() << " ms\n";
    return 0;
}