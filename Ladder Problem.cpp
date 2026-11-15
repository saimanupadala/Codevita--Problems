#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <climits>
#include <cstring>
using namespace std;

int M, N, K;
vector<string> grid;

bool freeLadder(int r, int c, int o) {
    if (o == 0) {
        if (r < 0 || r >= M || c < 0 || c + K > N) return false;
        for (int t = 0; t < K; t++) if (grid[r][c + t] == 'B') return false;
    } else {
        if (c < 0 || c >= N || r < 0 || r + K > M) return false;
        for (int t = 0; t < K; t++) if (grid[r + t][c] == 'B') return false;
    }
    return true;
}

bool squareFree(int top, int left) {
    if (top < 0 || left < 0 || top + K > M || left + K > N) return false;
    for (int i = top; i < top + K; i++)
        for (int j = left; j < left + K; j++)
            if (grid[i][j] == 'B') return false;
    return true;
}

struct State { int r, c, o; };

State mk(int r, int c, int o) {
    State s;
    s.r = r; s.c = c; s.o = o;
    return s;
}

State stateOf(const vector<pair<int,int> >& cells) {
    int r = INT_MAX, c = INT_MAX;
    bool h = true;
    for (size_t i = 0; i < cells.size(); i++) {
        r = min(r, cells[i].first);
        c = min(c, cells[i].second);
        if (cells[i].first != cells[0].first) h = false;
    }
    return mk(r, c, h ? 0 : 1);
}

int dist[16][16][2];

int main() {
    if (!(cin >> M >> N) || M <= 0 || N <= 0) { cout << "Impossible"; return 0; }

    string flat;
    char ch;
    while ((int)flat.size() < M * N && cin >> ch) {
        if (ch == 'l' || ch == 'I' || ch == 'L' || ch == 'B' || ch == 'E') {
            if (ch == 'I') ch = 'l';
            flat += ch;
        }
    }
    if ((int)flat.size() < M * N) { cout << "Impossible"; return 0; }

    grid.assign(M, "");
    for (int i = 0; i < M; i++) grid[i] = flat.substr(i * N, N);

    vector<pair<int,int> > src, dst;
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 'l') src.push_back(make_pair(i, j));
            else if (grid[i][j] == 'L') dst.push_back(make_pair(i, j));
        }
    if (src.empty() || dst.empty()) { cout << "Impossible"; return 0; }
    K = (int)src.size();

    State start = stateOf(src), goal = stateOf(dst);

    memset(dist, -1, sizeof(dist));
    dist[start.r][start.c][start.o] = 0;
    queue<State> q;
    q.push(start);
    const int dr[4] = {1, -1, 0, 0};
    const int dc[4] = {0, 0, 1, -1};

    while (!q.empty()) {
        State s = q.front(); q.pop();
        int d = dist[s.r][s.c][s.o];
        if (s.r == goal.r && s.c == goal.c && s.o == goal.o) { cout << d; return 0; }

        vector<State> nxt;
        for (int k = 0; k < 4; k++) {
            int nr = s.r + dr[k], nc = s.c + dc[k];
            if (freeLadder(nr, nc, s.o)) nxt.push_back(mk(nr, nc, s.o));
        }
        if (s.o == 0) {
            int tops[2];
            tops[0] = s.r; tops[1] = s.r - K + 1;
            for (int a = 0; a < 2; a++) {
                int top = tops[a];
                if (squareFree(top, s.c)) {
                    nxt.push_back(mk(top, s.c, 1));
                    nxt.push_back(mk(top, s.c + K - 1, 1));
                }
            }
        } else {
            int lefts[2];
            lefts[0] = s.c; lefts[1] = s.c - K + 1;
            for (int a = 0; a < 2; a++) {
                int left = lefts[a];
                if (squareFree(s.r, left)) {
                    nxt.push_back(mk(s.r, left, 0));
                    nxt.push_back(mk(s.r + K - 1, left, 0));
                }
            }
        }
        for (size_t i = 0; i < nxt.size(); i++) {
            State ns = nxt[i];
            if (dist[ns.r][ns.c][ns.o] == -1) {
                dist[ns.r][ns.c][ns.o] = d + 1;
                q.push(ns);
            }
        }
    }
    cout << "Impossible";
    return 0;
}
