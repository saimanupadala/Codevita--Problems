#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;

static int N, M;
static vector<string> grid;
static vector<bool> isRowRod, isColRod;

static bool isCableCell(int r, int c) {
    if (r < 0 || r >= N || c < 0 || c >= M) return false;
    if (grid[r][c] == 'C') return true;
    if (grid[r][c] == 'R') {
        // cable crossing under a rod
        if (isRowRod[r] && r > 0 && r + 1 < N && grid[r - 1][c] == 'C' && grid[r + 1][c] == 'C') return true;
        if (isColRod[c] && c > 0 && c + 1 < M && grid[r][c - 1] == 'C' && grid[r][c + 1] == 'C') return true;
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N >> M)) return 0;

    grid.assign(N, string(M, '.'));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cin >> grid[i][j];
        }
    }

    isRowRod.assign(N, false);
    isColRod.assign(M, false);

    // Row rods
    for (int i = 0; i < N; ++i) {
        bool hasR = false;
        bool hasDot = false;
        for (int j = 0; j < M; ++j) {
            if (grid[i][j] == 'R') hasR = true;
            if (grid[i][j] == '.') hasDot = true;
        }
        if (hasR && !hasDot) {
            int rCount = 0;
            for (int j = 0; j < M; ++j) if (grid[i][j] == 'R') rCount++;
            bool rowRod = false;
            if (rCount >= 2 || rCount == 1) {
                bool allCVertical = true;
                for (int j = 0; j < M; ++j) {
                    if (grid[i][j] == 'C') {
                        bool upC = (i > 0 && grid[i - 1][j] == 'C');
                        bool downC = (i + 1 < N && grid[i + 1][j] == 'C');
                        if (!upC || !downC) {
                            allCVertical = false;
                            break;
                        }
                    }
                }
                if (allCVertical) rowRod = true;
            }
            isRowRod[i] = rowRod;
        }
    }

    // Column rods
    for (int j = 0; j < M; ++j) {
        bool hasR = false;
        bool hasDot = false;
        for (int i = 0; i < N; ++i) {
            if (grid[i][j] == 'R') hasR = true;
            if (grid[i][j] == '.') hasDot = true;
        }
        if (hasR && !hasDot) {
            bool allCHorizontal = true;
            for (int i = 0; i < N; ++i) {
                if (grid[i][j] == 'C') {
                    bool leftC = (j > 0 && grid[i][j - 1] == 'C');
                    bool rightC = (j + 1 < M && grid[i][j + 1] == 'C');
                    if (!leftC || !rightC) {
                        allCHorizontal = false;
                        break;
                    }
                }
            }
            isColRod[j] = allCHorizontal;
        }
    }

    // Find a cable endpoint on the boundary
    int startR = -1, startC = -1;
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if ((i == 0 || i == N - 1 || j == 0 || j == M - 1) && grid[i][j] == 'C' && !isRowRod[i] && !isColRod[j]) {
                int deg = 0;
                for (int d = 0; d < 4; ++d) {
                    if (isCableCell(i + dr[d], j + dc[d])) deg++;
                }
                if (deg == 1) {
                    startR = i;
                    startC = j;
                    break;
                }
            }
        }
        if (startR != -1) break;
    }

    // Trace the cable
    vector<pair<int, int> > path;
    vector<vector<bool> > vis(N, vector<bool>(M, false));
    int curR = startR, curC = startC;

    while (curR != -1) {
        path.push_back(make_pair(curR, curC));
        vis[curR][curC] = true;
        int nextR = -1, nextC = -1;

        if (path.size() >= 2 && (isRowRod[curR] || isColRod[curC])) {
            int prevR = path[path.size() - 2].first;
            int prevC = path[path.size() - 2].second;
            int nr = curR + (curR - prevR);
            int nc = curC + (curC - prevC);
            if (isCableCell(nr, nc) && !vis[nr][nc]) {
                nextR = nr;
                nextC = nc;
            }
        } else {
            for (int d = 0; d < 4; ++d) {
                int nr = curR + dr[d];
                int nc = curC + dc[d];
                if (isCableCell(nr, nc) && !vis[nr][nc]) {
                    nextR = nr;
                    nextC = nc;
                    break;
                }
            }
        }
        curR = nextR;
        curC = nextC;
    }

    int totalSwitches = 0;

    // Each row rod
    for (int i = 0; i < N; ++i) {
        if (!isRowRod[i]) continue;
        vector<char> st;
        for (size_t k = 0; k < path.size(); ++k) {
            if (path[k].first == i) {
                char state = grid[path[k].first][path[k].second];
                if (!st.empty() && st.back() == state) {
                    st.pop_back();
                } else {
                    st.push_back(state);
                }
            }
        }
        totalSwitches += (int)st.size() / 2;
    }

    // Each column rod
    for (int j = 0; j < M; ++j) {
        if (!isColRod[j]) continue;
        vector<char> st;
        for (size_t k = 0; k < path.size(); ++k) {
            if (path[k].second == j) {
                char state = grid[path[k].first][path[k].second];
                if (!st.empty() && st.back() == state) {
                    st.pop_back();
                } else {
                    st.push_back(state);
                }
            }
        }
        totalSwitches += (int)st.size() / 2;
    }

    cout << totalSwitches;
    return 0;
}
