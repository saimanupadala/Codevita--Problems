#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>
#include <climits>

using namespace std;

#ifndef LLONG_MAX
#define LLONG_MAX 9223372036854775807LL
#endif

struct State {
    long long cost;
    int r, c;
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

State mk(long long cost, int r, int c) {
    State s;
    s.cost = cost; s.r = r; s.c = c;
    return s;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<long long> > grid(n, vector<long long>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> grid[i][j];
        }
    }

    int sr, sc;
    cin >> sr >> sc;
    --sr; --sc;

    int dr, dc;
    cin >> dr >> dc;
    --dr; --dc;

    int k;
    cin >> k;
    vector<vector<bool> > blocked(n, vector<bool>(m, false));
    for (int i = 0; i < k; ++i) {
        int br, bc;
        cin >> br >> bc;
        blocked[br - 1][bc - 1] = true;
    }

    int drs[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dcs[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    vector<vector<long long> > dist(n, vector<long long>(m, LLONG_MAX));
    priority_queue<State, vector<State>, greater<State> > pq;

    dist[sr][sc] = 0;
    pq.push(mk(0, sr, sc));

    while (!pq.empty()) {
        State top = pq.top();
        pq.pop();
        long long current_cost = top.cost;
        int r = top.r;
        int c = top.c;

        if (current_cost > dist[r][c]) continue;
        if (r == dr && c == dc) {
            cout << current_cost;
            return 0;
        }

        // Find max among unblocked neighbors of (r, c) excluding each neighbor
        for (int i = 0; i < 8; ++i) {
            int nr = r + drs[i];
            int nc = c + dcs[i];

            if (nr >= 0 && nr < n && nc >= 0 && nc < m && !blocked[nr][nc]) {
                long long max_other = grid[r][c];
                for (int j = 0; j < 8; ++j) {
                    if (i == j) continue;
                    int nnr = r + drs[j];
                    int nnc = c + dcs[j];
                    if (nnr >= 0 && nnr < n && nnc >= 0 && nnc < m && !blocked[nnr][nnc]) {
                        max_other = max(max_other, grid[nnr][nnc]);
                    }
                }

                long long required_val = max_other + 1;
                long long add_cost = max(0LL, required_val - grid[nr][nc]);

                if (dist[r][c] + add_cost < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + add_cost;
                    pq.push(mk(dist[nr][nc], nr, nc));
                }
            }
        }
    }

    cout << (dist[dr][dc] == LLONG_MAX ? -1 : dist[dr][dc]);
    return 0;
}
