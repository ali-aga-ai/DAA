#include <bits/stdc++.h>
using namespace std;

unordered_map<int, int> cliqueSizeFrequency;
uint64_t cliqueCount = 0;
int maxi = 0;

class Graph
{
public:
    int n; // num of vertices
    vector<vector<int>> adj;
    vector<int> labels; // each vertex has a label
    unordered_map<int, int> labelToId;

    Graph() : n(0) {}
    int addVertex(int label)
    {
        if (labelToId.find(label) == labelToId.end())
        {
            labelToId[label] = n;
            labels.push_back(label);
            adj.push_back(vector<int>());
            n++;
            return n - 1;
        }
        return labelToId[label];
    }
    void addEdge(int l1, int l2)
    {
        int u = addVertex(l1);
        int v = addVertex(l2);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void sortAdjacency()
    {
        for (int i = 0; i < n; i++)
            sort(adj[i].begin(), adj[i].end());
    }
};

void readGraph(const string &filename, Graph &graph)
{
    ifstream inFile(filename);
    if (!inFile)
    {
        cerr << "Error opening file " << filename << endl;
        exit(1);
    }
    int edges = 0;
    string line;
    while (getline(inFile, line))
    {
        size_t pos = line.find_first_not_of(" \t");
        if (pos != string::npos && line[pos] == '#')
            continue;
        istringstream iss(line);
        int l1, l2;
        if (iss >> l1 >> l2)
        { // takes the first two numbers from the line
            edges++;
            graph.addEdge(l1, l2);
        }
    }
    graph.sortAdjacency();
    cout << "reading done" << endl;
    fflush(stdout); // optimises rw
}
vector<int> intersect(const vector<int> &a, const vector<int> &b)
{
    vector<int> result;
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
    return result;
}
vector<int> diff(const vector<int> &a, const vector<int> &b)
{
    vector<int> result;
    set_difference(a.begin(), a.end(), b.begin(), b.end(), back_inserter(result));
    return result;
}
vector<int> finddegen(const Graph &graph)
{ // degeneracy ordering, using bucket sort
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

    while (removedCount < n)
    {
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
        for (int u : graph.adj[v])
        {
            if (!removed[u])
            {
                degree[u]--;
                buckets[degree[u]].push_back(u);
            }
        }
    }
    
    reverse(order.begin(), order.end());
    
    return order;
}
void bkset(const Graph &graph, vector<int> R, vector<int> &P, vector<int> &X)
{
    if (P.empty() && X.empty())
    {
        cliqueCount++;
        maxi = maxi > R.size() ? maxi : R.size();
        cliqueSizeFrequency[R.size()]++;
        return;
    }

    vector<int> unionPX = P;
    unionPX.insert(unionPX.end(), X.begin(), X.end());
    
    int pivot = unionPX[0];
    int maxCount = intersect(P, graph.adj[pivot]).size();
    
    for (int u : unionPX)
    {
        int cnt = intersect(P, graph.adj[u]).size();
        if (cnt > maxCount)
        {
            maxCount = cnt;
            pivot = u;
        }
    }
    
    vector<int> candidates = diff(P, graph.adj[pivot]);
    
    for (int v : candidates)
    {
        if (find(P.begin(), P.end(), v) == P.end())
            continue;

        vector<int> newR = R;
        newR.push_back(v);
        vector<int> newP = intersect(P, graph.adj[v]);
        vector<int> newX = intersect(X, graph.adj[v]);

        bkset(graph, newR, newP, newX);

        P.erase(remove(P.begin(), P.end(), v), P.end());
        X.push_back(v);

        sort(P.begin(), P.end());
        sort(X.begin(), X.end());
    }
}

void bkdegen(const Graph &graph)
{
    vector<int> degenOrder = finddegen(graph);
    int n = graph.n;
    vector<int> orderIndex(n, -1);
    for (int i = 0; i < (int)degenOrder.size(); i++)
        orderIndex[degenOrder[i]] = i;
    for (int i = 0; i < (int)degenOrder.size(); i++)
    {
        int v = degenOrder[i];
        vector<int> P, X;
        for (int u : graph.adj[v])
        {
            if (orderIndex[u] > i)
                P.push_back(u);
            else if (orderIndex[u] < i)
                X.push_back(u);
        }
        sort(P.begin(), P.end());
        sort(X.begin(), X.end());
        vector<int> R = {v};
        bkset(graph, R, P, X);
    }
}

int main(int argc, char **argv)
{
    ios::sync_with_stdio(false);
    auto start = chrono::high_resolution_clock::now();
    Graph graph;
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <filename with edge>" << endl;
        return 1;
    }
    readGraph(argv[1], graph);

    cliqueCount = 0;
    cliqueSizeFrequency.clear();
    bkdegen(graph);

    cout << "Number of unique maximal cliques: " << cliqueCount << "\n";
    for (const auto &entry : cliqueSizeFrequency)
    {
        cout << "Cliques of size " << entry.first << ": " << entry.second << "\n";
    }
    cout << "Largest clique size is: " << maxi << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Elapsed time: " << duration.count() << " ms\n";
    return 0;
}
