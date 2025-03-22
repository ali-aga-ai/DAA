#include <bits/stdc++.h>
#include <chrono>

using namespace std;
vector<int> degeneracyOrder;
int numCliques = 0;

// Custom hash function for sets
struct SetHash {
    size_t operator()(const set<int>& s) const {
        size_t hash = 0;
        for (int x : s) {
            hash = hash * 31 + x;
        }
        return hash;
    }
};

// Custom hash function for tuples of sets
struct TupleSetHash {
    size_t operator()(const tuple<set<int>, set<int>, set<int>>& t) const {
        return SetHash()(get<0>(t)) ^ (SetHash()(get<1>(t)) << 1) ^ (SetHash()(get<2>(t)) << 2);
    }
};

// Use unordered_map instead of map for faster lookups
unordered_map<tuple<set<int>, set<int>, set<int>>, int, TupleSetHash> memo;

void findDegeneracyOrder(vector<vector<int>> &adj, const map<int, int> &vertexMap) {
    int n = adj.size();
    vector<int> degree(n);
    vector<bool> removed(n, false);
    degeneracyOrder.clear();
    
    for (int i = 0; i < n; i++) {
        degree[i] = adj[i].size(); // degree represents the number of neighbors
    }
    
    for (int i = 0; i < n; i++) {
        int minDegVertex = -1;
        for (int v = 0; v < n; v++) {
            if (!removed[v] && (minDegVertex == -1 || degree[v] < degree[minDegVertex])) {
                minDegVertex = v;
            }
        }
        
        degeneracyOrder.push_back(minDegVertex);
        removed[minDegVertex] = true; // remove vertex with least neighbours from graph

        // update number of neighbours of all neighbours of minDegVertex        
        for (int neighbor : adj[minDegVertex]) {
            if (!removed[neighbor]) {
                degree[neighbor]--;
            }
        }
    }
}

void bronKerboschPivot(set<int> &P, set<int> &R, set<int> &X, vector<vector<int>> &adj){
    // Check if this subproblem has already been solved
    tuple<set<int>, set<int>, set<int>> state = make_tuple(P, R, X);
    if (memo.find(state) != memo.end()) {
        numCliques += memo[state];
        return;
    }
    
    int localCliques = 0;
    
    if(P.empty() && X.empty()){
        numCliques++;
        if(numCliques % 1000 == 0) cout << numCliques << " cliques found" << endl;
        memo[state] = 1;
        return; // one clique found
    }
    
    // Optimized pivot selection: choose vertex that maximizes |P ∩ N(u)|
    int maxIntersection = -1;
    int u = -1;
    
    // Check vertices from both P and X
    for (int v : P) {
        int intersection = 0;
        for (int neighbor : adj[v]) {
            if (P.find(neighbor) != P.end()) {
                intersection++;
            }
        }
        if (intersection > maxIntersection) {
            maxIntersection = intersection;
            u = v;
        }
    }
    
    for (int v : X) {
        int intersection = 0;
        for (int neighbor : adj[v]) {
            if (P.find(neighbor) != P.end()) {
                intersection++;
            }
        }
        if (intersection > maxIntersection) {
            maxIntersection = intersection;
            u = v;
        }
    }
    
    // Fallback if no pivot found (shouldn't happen if P or X has elements)
    if (u == -1 && !P.empty()) {
        u = *P.begin();
    } else if (u == -1 && !X.empty()) {
        u = *X.begin();
    }
    
    set<int> u_neighbors;
    for (int neighbor : adj[u]) {
        u_neighbors.insert(neighbor);
    }
    
    set<int> in_p_not_u_neighbour;
    set_difference(P.begin(), P.end(), u_neighbors.begin(), u_neighbors.end(),
                   inserter(in_p_not_u_neighbour, in_p_not_u_neighbour.begin()));
    
    for (auto v : in_p_not_u_neighbour) {
        set<int> new_R = R;
        new_R.insert(v);
        
        set<int> N_v;
        for (int neighbor : adj[v]) {
            N_v.insert(neighbor);
        }
        
        set<int> p2, x2;
        set_intersection(P.begin(), P.end(), N_v.begin(), N_v.end(), inserter(p2, p2.begin()));
        set_intersection(X.begin(), X.end(), N_v.begin(), N_v.end(), inserter(x2, x2.begin()));
        
        int beforeCliques = numCliques;
        bronKerboschPivot(p2, new_R, x2, adj);
        localCliques += (numCliques - beforeCliques);
        
        P.erase(v);
        X.insert(v);
    }
    
    // Store the result in the memo table
    memo[state] = localCliques;
}

void bronKerboschDegeneracy(vector<vector<int>> &adj, const map<int, int> &vertexMap) {
    findDegeneracyOrder(adj, vertexMap);
    memo.clear(); // Clear memoization table before starting
    
    for (int i = 0; i < degeneracyOrder.size(); i++) {
        int v = degeneracyOrder[i];
        
        set<int> P, X, R;
        R.insert(v);
        
        set<int> N_v;
        for (int neighbor : adj[v]) {
            N_v.insert(neighbor);
        }
        
        
        set<int> v_later, v_earlier;
        for (int j = i + 1; j < degeneracyOrder.size(); j++) {
            v_later.insert(degeneracyOrder[j]);
        }
        for (int j = 0; j < i; j++) {
            v_earlier.insert(degeneracyOrder[j]);
        }
        
        set_intersection(N_v.begin(), N_v.end(), v_later.begin(), v_later.end(), inserter(P, P.begin()));
        set_intersection(N_v.begin(), N_v.end(), v_earlier.begin(), v_earlier.end(), inserter(X, X.begin()));
        
        bronKerboschPivot(P, R, X, adj);
    }
}

int main() {
    auto start = chrono::high_resolution_clock::now();

    ifstream infile("biggest.txt");  
    if (!infile) {
        cerr << "Failed to open file" << endl;
        return 1;
    }

    int v, e;
    infile >> v >> e;  //4 number of vertices and edges
    
    vector<pair<int, int>> edges;
    set<int> uniqueVertices;
    
    for (int i = 0; i < e; i++) {
        int a, b;
        if (!(infile >> a >> b)) {
            cerr << "Error reading edge " << i << endl;
            return 1;
        }
        edges.push_back({a, b});
        uniqueVertices.insert(a);
        uniqueVertices.insert(b);
    }
    
    // a mapping from original vertex IDs to sequential indices
    map<int, int> vertexToIndex;
    map<int, int> indexToVertex;
    int index = 0;
    
    for (int vertex : uniqueVertices) {
        vertexToIndex[vertex] = index;
        indexToVertex[index] = vertex;
        index++;
    }
    
    //  adjacency list using the mapped indices
    vector<vector<int>> adj(uniqueVertices.size());
    
    for (const auto& edge : edges) {
        int a = vertexToIndex[edge.first];
        int b = vertexToIndex[edge.second];
        adj[a].push_back(b);
        adj[b].push_back(a);  // Undirected graph
    }
    
    infile.close();  
    
    cout << "Number of unique vertices: " << uniqueVertices.size() << endl;
    cout << "Number of edges: " << edges.size() << endl;
    
    bronKerboschDegeneracy(adj, vertexToIndex);
    cout << "Number of cliques: " << numCliques << endl;
    auto end = chrono::high_resolution_clock::now();
    cout << "Time taken: " 
         << chrono::duration<double>(end - start).count() 
         << " seconds\n";
    return 0;
}