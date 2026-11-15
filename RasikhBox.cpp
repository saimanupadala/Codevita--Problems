#include <iostream>
#include <vector>
#include <string>

using namespace std;

vector<string> apply_gravity(const vector<string>& grid) {
    int r = grid.size();
    int c = grid[0].size();
    vector<string> res(r, string(c, '.'));

    for (int j = 0; j < c; ++j) {
        int count = 0;
        for (int i = 0; i < r; ++i) {
            if (grid[i][j] == '*') {
                count++;
            }
        }
        for (int i = r - 1; i >= r - count; --i) {
            res[i][j] = '*';
        }
    }
    return res;
}

vector<string> rotate_right(const vector<string>& grid) {
    int r = grid.size();
    int c = grid[0].size();
    vector<string> res(c, string(r, '.'));

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            res[j][r - 1 - i] = grid[i][j];
        }
    }
    return res;
}

vector<string> rotate_left(const vector<string>& grid) {
    int r = grid.size();
    int c = grid[0].size();
    vector<string> res(c, string(r, '.'));

    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            res[c - 1 - j][i] = grid[i][j];
        }
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int m, n;
    if (!(cin >> m >> n)) return 0;

    vector<string> grid(m, string(n, '.'));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[i][j];
        }
    }

    grid = apply_gravity(grid);

    int k;
    if (cin >> k) {
        while (k--) {
            string dir;
            cin >> dir;
            if (dir == "right") {
                grid = rotate_right(grid);
            } else if (dir == "left") {
                grid = rotate_left(grid);
            }
            grid = apply_gravity(grid);
        }
    }

    for (int i = 0; i < (int)grid.size(); ++i) {
        for (int j = 0; j < (int)grid[i].size(); ++j) {
            cout << grid[i][j] << (j == (int)grid[i].size() - 1 ? "" : " ");
        }
        if (i != (int)grid.size() - 1) {
            cout << "\n";
        }
    }

    return 0;
}
