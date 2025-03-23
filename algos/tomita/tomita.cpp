#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
using namespace std;

static set<int> Q;       
static int clique_count = 0;
static map<int,int> clique_size_dist;

int max_cand(const set<int>& subg,
             const set<int>& cand,
             const vector<vector<int>>& adj)
{
    int best_u = -1;
    int max_intersection = -1;
    for (auto node : subg) {
        int intersection_size = 0;
        for (int neighbor : adj[node]) {
            if (cand.find(neighbor) != cand.end()) {
                intersection_size++;
            }
        }
        if (intersection_size > max_intersection) {
            max_intersection = intersection_size;
            best_u = node;
        }
    }
    return best_u;
}

set<int> diff(const set<int>& cand,
                  const vector<int>& neighbors)
{
    set<int> result;
    set_difference(
        cand.begin(), cand.end(),
        neighbors.begin(), neighbors.end(),
        inserter(result, result.end())
    );
    return result;
}

set<int> intersect(const set<int>& subg,
                       const vector<int>& neighbors)
{
    set<int> result;
    set_intersection(
        subg.begin(), subg.end(),
        neighbors.begin(), neighbors.end(),
        inserter(result, result.end())
    );
    return result;
}

void expand(const set<int>& subg,
            set<int>& cand,
            const vector<vector<int>>& adj)
{
    if (subg.empty()) {
        int clique_size = (int)Q.size();
        clique_size_dist[clique_size]++;
        clique_count++;
        return;
    }

    int u = max_cand(subg, cand, adj);
    set<int> cand_minus = diff(cand, adj[u]);

    for (auto node : cand_minus) {
        Q.insert(node);
        set<int> subgq = intersect(subg, adj[node]);
        set<int> candq = intersect(cand, adj[node]);
        expand(subgq, candq, adj);
        cand.erase(node);
        Q.erase(node);
    }
}

int main()
{
    //string filen = "C:\\Users\\rishi\\Studies\\CS\\daa_tomita\\Wiki-Vote.txt";
    string filen = "C:\\Users\\rishi\\Studies\\CS\\daa_tomita\\Email-Enron.txt";
    //string filen = "C:\\Users\\rishi\\Studies\\CS\\daa_tomita\\as-skitter.txt";
    ifstream file(filen);

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < 5; ++i) {
        string temp;
        getline(file, temp);
    }

    vector<pair<int,int>> edges;
    set<int> vset;

    {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            int a, b;
            if (!(iss >> a >> b)) continue;
            if (a == b) continue;
            edges.push_back({a, b});
            vset.insert(a);
            vset.insert(b);
        }
    }
    file.close();

    unordered_map<int,int> node_mapping;
    node_mapping.reserve(vset.size());
    int idx = 0;
    for (auto node : vset) {
        node_mapping[node] = idx++;
    }
    int n = (int)vset.size();

    vector<vector<int>> adj(n);
    for (auto &e : edges) {
        int u = node_mapping[e.first];
        int v = node_mapping[e.second];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 0; i < n; ++i) {
        sort(adj[i].begin(), adj[i].end());
        adj[i].erase(unique(adj[i].begin(), adj[i].end()), adj[i].end());
    }

    set<int> subg, cand;
    for (int i = 0; i < n; ++i) {
        subg.insert(i);
        cand.insert(i);
    }

    //cout<<"Loaded"<<endl;
    expand(subg, cand, adj);

    auto end = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(end - start).count();

    cout << "Total cliques found: " << clique_count << endl;
    cout << "Execution time: " << elapsed << " seconds\n\n";
    cout << "Clique Size Distribution:\n";
    for (auto entry : clique_size_dist) {
        cout << "Size " << entry.first << ": " << entry.second << " clique(s)\n";
    }
    return 0;
}
