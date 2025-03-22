#include <bits/stdc++.h>

using namespace std;
vector<int> degeneracyOrder;
int numCliques = 0;

map<tuple<set<int>, set<int>, set<int>>, int> memo; // memoisation 

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
    // memo check
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
    
    int u = -1; // u is the pivot
    if(!P.empty()){
        u = *P.begin(); // accessing first element of a set
    }
    else{
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
    
    memo[state] = localCliques;
}

void bronKerboschDegeneracy(vector<vector<int>> &adj, const map<int, int> &vertexMap) {
    findDegeneracyOrder(adj, vertexMap);
    memo.clear();// clearing memo table (mnot necessary)
    
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
    ifstream infile("dataset.txt");  // Open file
    if (!infile) {
        cerr << "Failed to open file" << endl;
        return 1;
    }

    int v, e;
    infile >> v >> e;  // Read number of vertices and edges
    
    // First, read all edges to find the actual vertices in use
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
    
    // Create a mapping from original vertex IDs to sequential indices
    map<int, int> vertexToIndex;
    map<int, int> indexToVertex;
    int index = 0;
    
    for (int vertex : uniqueVertices) {
        vertexToIndex[vertex] = index;
        indexToVertex[index] = vertex;
        index++;
    }
    
    // Now create the adjacency list using the mapped indices
    vector<vector<int>> adj(uniqueVertices.size());
    
    for (const auto& edge : edges) {
        int a = vertexToIndex[edge.first];
        int b = vertexToIndex[edge.second];
        adj[a].push_back(b);
        adj[b].push_back(a);  // Undirected graph
    }
    
    infile.close();  // Close the file
    
    cout << "Number of unique vertices: " << uniqueVertices.size() << endl;
    cout << "Number of edges: " << edges.size() << endl;
    
    bronKerboschDegeneracy(adj, vertexToIndex);
    cout << "Number of cliques: " << numCliques << endl;

    return 0;
}