#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <queue>
#include <utility>

using namespace std;

typedef set<pair<int, int> > EdgeSet;

void findCycles(int u, int p, vector<int>& path, set<vector<int> >& cycles,
                const map<int, vector<int> >& adj) {
    path.push_back(u);
    map<int, vector<int> >::const_iterator it = adj.find(u);
    if (it != adj.end()) {
        for (size_t k = 0; k < it->second.size(); ++k) {
            int v = it->second[k];
            if (v == p) continue;
            vector<int>::iterator pathIt = find(path.begin(), path.end(), v);
            if (pathIt != path.end()) {
                vector<int> cycle(pathIt, path.end());
                if (cycle.size() >= 3) {
                    vector<int>::iterator minIt = min_element(cycle.begin(), cycle.end());
                    rotate(cycle.begin(), minIt, cycle.end());

                    vector<int> revCycle = cycle;
                    reverse(revCycle.begin() + 1, revCycle.end());
                    vector<int>::iterator minRevIt = min_element(revCycle.begin(), revCycle.end());
                    rotate(revCycle.begin(), minRevIt, revCycle.end());

                    if (cycle < revCycle) {
                        cycles.insert(cycle);
                    } else {
                        cycles.insert(revCycle);
                    }
                }
            } else if (find(path.begin(), path.end(), v) == path.end()) {
                findCycles(v, u, path, cycles, adj);
            }
        }
    }
    path.pop_back();
}

EdgeSet rotateCycle(const EdgeSet& currentEdges, const vector<int>& cycle, int dir) {
    int k = (int)cycle.size();
    map<int, int> pi;
    for (int i = 0; i < k; ++i) {
        int next_idx = (i + dir + k) % k;
        pi[cycle[i]] = cycle[next_idx];
    }
    EdgeSet nextEdges;
    for (EdgeSet::const_iterator e = currentEdges.begin(); e != currentEdges.end(); ++e) {
        int u = e->first;
        int v = e->second;
        if (pi.count(u)) u = pi[u];
        if (pi.count(v)) v = pi[v];
        nextEdges.insert(make_pair(min(u, v), max(u, v)));
    }
    return nextEdges;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int E;
    if (!(cin >> E)) return 0;

    set<int> nodes;
    vector<pair<int, int> > initialEdges(E), expectedEdges(E);
    map<int, vector<int> > initAdj;

    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        initialEdges[i] = make_pair(min(u, v), max(u, v));
        initAdj[u].push_back(v);
        initAdj[v].push_back(u);
        nodes.insert(u);
        nodes.insert(v);
    }

    for (int i = 0; i < E; ++i) {
        int u, v;
        cin >> u >> v;
        expectedEdges[i] = make_pair(min(u, v), max(u, v));
        nodes.insert(u);
        nodes.insert(v);
    }

    set<vector<int> > cyclesSet;
    vector<int> path;
    int startNode = *nodes.begin();
    findCycles(startNode, -1, path, cyclesSet, initAdj);

    vector<vector<int> > cycles(cyclesSet.begin(), cyclesSet.end());

    EdgeSet targetEdges(expectedEdges.begin(), expectedEdges.end());
    EdgeSet currentEdges(initialEdges.begin(), initialEdges.end());

    if (currentEdges == targetEdges) {
        cout << 0;
        return 0;
    }

    set<EdgeSet> visited;
    queue<pair<EdgeSet, int> > q;
    q.push(make_pair(currentEdges, 0));
    visited.insert(currentEdges);

    while (!q.empty()) {
        pair<EdgeSet, int> front = q.front();
        q.pop();
        const EdgeSet& currEdges = front.first;
        int steps = front.second;

        if (currEdges == targetEdges) {
            cout << steps;
            return 0;
        }

        for (size_t c = 0; c < cycles.size(); ++c) {
            // Try both directions for each cycle
            for (int di = 0; di < 2; ++di) {
                int dir = (di == 0) ? 1 : -1;
                EdgeSet nextEdges = rotateCycle(currEdges, cycles[c], dir);
                if (visited.find(nextEdges) == visited.end()) {
                    visited.insert(nextEdges);
                    q.push(make_pair(nextEdges, steps + 1));
                }
            }
        }
    }

    cout << -1;
    return 0;
}
