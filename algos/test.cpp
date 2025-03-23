#include <bits/stdc++.h>
using namespace std;

int n;
vector<set<int>> adj;
set<set<int>> allCliques;
map<int,int> vertexToIndex;
map<int,int> indexToVertex;

struct Frame {
    int i;
    vector<int> inC;
    vector<int> S;
    vector<int> T;
};

void iterativeUPDATE() {
    vector<Frame> stack;
    Frame init;
    init.i = 2;
    init.inC.assign(n + 1, 0);
    init.inC[1] = 1;
    init.S.assign(n + 1, 0);
    init.T.assign(n + 1, 0);
    stack.push_back(init);

    while (!stack.empty()) {
        Frame current = stack.back();
        stack.pop_back();
        int i = current.i;

        if (i == n + 1) {
            set<int> clique;
            for (int x = 1; x <= n; ++x) {
                if (current.inC[x])
                    clique.insert(indexToVertex[x]);
            }
            if (allCliques.find(clique) == allCliques.end()) {
                allCliques.insert(clique);
                int y = (int)allCliques.size();
                cout << y << "\n";
            }
            continue;
        }

        bool nonEmpty = false;
        for (int x = 1; x <= n; ++x) {
            if (current.inC[x] && !adj[i].count(x)) {
                nonEmpty = true;
                break;
            }
        }
        if (nonEmpty) {
            Frame skipFrame = current;
            skipFrame.i = i + 1;
            stack.push_back(skipFrame);
        }

        int cnt = 0;
        for (int x = 1; x <= n; ++x) {
            if (current.inC[x] && adj[i].count(x))
                cnt++;
        }

        for (int x = 1; x <= n; ++x) {
            if (current.inC[x] && adj[i].count(x)) {
                for (int y : adj[x]) {
                    if (!current.inC[y] && y != i)
                        current.T[y]++;
                }
            }
            if (current.inC[x] && !adj[i].count(x)) {
                for (int y : adj[x]) {
                    if (!current.inC[y])
                        current.S[y]++;
                }
            }
        }

        bool FLAG = true;
        for (int y : adj[i]) {
            if (!current.inC[y] && y < i && current.T[y] == cnt) {
                FLAG = false;
                break;
            }
        }

        vector<int> C_minus_Ni;
        for (int x = 1; x <= n; ++x) {
            if (current.inC[x] && !adj[i].count(x))
                C_minus_Ni.push_back(x);
        }
        sort(C_minus_Ni.begin(), C_minus_Ni.end());
        int p = C_minus_Ni.size();

        for (int k = 0; k < p; ++k) {
            int jk = C_minus_Ni[k];
            for (int y : adj[jk]) {
                if (!current.inC[y] && y < i && current.T[y] == cnt) {
                    if (y >= jk) {
                        current.S[y]--;
                    } else {
                        bool isFirstBigger = true;
                        for (int m = 0; m < k; ++m) {
                            if (C_minus_Ni[m] > y) {
                                isFirstBigger = false;
                                break;
                            }
                        }
                        if (isFirstBigger) {
                            if ((current.S[y] + k) == p && y >= (jk - 1)) {
                                FLAG = false;
                            }
                        }
                    }
                }
            }
        }

        if (!C_minus_Ni.empty()) {
            int jp = C_minus_Ni.back();
            for (int y : adj[i]) {
                if (!current.inC[y] && y != i && y < i && current.T[y] == cnt && current.S[y] == 0) {
                    if (jp < y || jp < i - 1) {
                        FLAG = false;
                    }
                }
            }
        }

        for (int x = 1; x <= n; ++x) {
            if (current.inC[x] && adj[i].count(x)) {
                for (int y : adj[x]) {
                    if (!current.inC[y] && y != i)
                        current.T[y] = 0;
                }
            }
            if (current.inC[x] && !adj[i].count(x)) {
                for (int y : adj[x]) {
                    if (!current.inC[y])
                        current.S[y] = 0;
                }
            }
        }

        if (FLAG) {
            vector<int> newC(n + 1, 0);
            newC[i] = 1;
            for (int x = 1; x <= n; ++x) {
                if (current.inC[x] && adj[i].count(x))
                    newC[x] = 1;
            }
            Frame includeFrame;
            includeFrame.i = i + 1;
            includeFrame.inC = newC;
            includeFrame.S = current.S;
            includeFrame.T = current.T;
            stack.push_back(includeFrame);
        }
    }
}

void CLIQUE() {
    iterativeUPDATE();

    cout << "\nFinal cliques:\n";
    for (const auto &clique : allCliques) {
        cout << "Clique: ";
        for (int v : clique) {
            cout << v << " ";
        }
        cout << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ifstream inputFile("wiki-Vote.txt");
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file.\n";
        return 1;
    }

    int m;
    inputFile >> n >> m;

    unordered_map<int, set<int>> origAdj;
    origAdj.reserve(n + 10);

    set<int> distinctVertices;
    for (int i = 0; i < m; ++i) {
        int u, v;
        inputFile >> u >> v;
        if (u == v) continue;
        distinctVertices.insert(u);
        distinctVertices.insert(v);
        origAdj[u].insert(v);
        origAdj[v].insert(u);
    }
    inputFile.close();

    vector<pair<int, int>> degList;
    degList.reserve(distinctVertices.size());
    for (auto originalV : distinctVertices) {
        int deg = (int)origAdj[originalV].size();
        degList.push_back({deg, originalV});
    }

    sort(degList.begin(), degList.end(),
         [](auto &a, auto &b) { return a.first < b.first; });

    n = (int)degList.size();
    vertexToIndex.clear();
    indexToVertex.clear();
    for (int i = 0; i < n; ++i) {
        int newLabel = i + 1;
        int originalV = degList[i].second;
        vertexToIndex[originalV] = newLabel;
        indexToVertex[newLabel] = originalV;
    }

    adj.assign(n + 1, {});
    for (auto &kv : origAdj) {
        int origV = kv.first;
        if (!vertexToIndex.count(origV)) continue;
        int u = vertexToIndex[origV];
        for (int origNbh : kv.second) {
            if (!vertexToIndex.count(origNbh)) continue;
            int v = vertexToIndex[origNbh];
            adj[u].insert(v);
        }
    }

    CLIQUE();
    return 0;
}
