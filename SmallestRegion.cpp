#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Rect {
    int x1, y1, x2, y2;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Rect> rects(n);
    for (int i = 0; i < n; ++i) {
        cin >> rects[i].x1 >> rects[i].y1 >> rects[i].x2 >> rects[i].y2;
    }

    int ox1, oy1, ox2, oy2;
    cin >> ox1 >> oy1 >> ox2 >> oy2;

    vector<int> x_coords;
    x_coords.push_back(ox1);
    x_coords.push_back(ox2);

    for (int x = ox1 + 1; x < ox2; ++x) {
        bool valid = true;
        for (size_t i = 0; i < rects.size(); ++i) {
            if (rects[i].x1 < x && x < rects[i].x2) {
                valid = false;
                break;
            }
        }
        if (valid) {
            x_coords.push_back(x);
        }
    }

    vector<int> y_coords;
    y_coords.push_back(oy1);
    y_coords.push_back(oy2);

    for (int y = oy1 + 1; y < oy2; ++y) {
        bool valid = true;
        for (size_t i = 0; i < rects.size(); ++i) {
            if (rects[i].y1 < y && y < rects[i].y2) {
                valid = false;
                break;
            }
        }
        if (valid) {
            y_coords.push_back(y);
        }
    }

    sort(x_coords.begin(), x_coords.end());
    sort(y_coords.begin(), y_coords.end());

    long long min_area = -1;

    for (size_t i = 0; i < x_coords.size() - 1; ++i) {
        long long width = x_coords[i+1] - x_coords[i];
        for (size_t j = 0; j < y_coords.size() - 1; ++j) {
            long long height = y_coords[j+1] - y_coords[j];
            long long area = width * height;
            if (min_area == -1 || area < min_area) {
                min_area = area;
            }
        }
    }

    cout << min_area;

    return 0;
}
