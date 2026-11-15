#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Point {
    double x, y;
};

double get_polygon_area(const vector<Point>& poly) {
    int n = poly.size();
    if (n < 3) return 0.0;
    double area = 0.0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += (poly[i].x * poly[j].y) - (poly[j].x * poly[i].y);
    }
    return abs(area) / 2.0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Point> pts(n);
    for (int i = 0; i < n; ++i) {
        cin >> pts[i].x >> pts[i].y;
    }

    double init_area = get_polygon_area(pts);
    double perim = 0.0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        perim += abs(pts[i].x - pts[j].x) + abs(pts[i].y - pts[j].y);
    }

    double max_vol = 0.0;

    for (int step = 1; step <= 200; ++step) {
        double h = step * 0.1;
        
        // Offset logic for rectilinear polygon
        double rem_area = init_area - (perim * h) + (4.0 * h * h);
        
        if (rem_area <= 0.001) continue;

        // Verify min edge constraints
        bool valid = true;
        for (int i = 0; i < n; ++i) {
            int j = (i + 1) % n;
            double edge_len = abs(pts[i].x - pts[j].x) + abs(pts[i].y - pts[j].y);
            if (edge_len - 2.0 * h < 0.1 - 1e-6) {
                valid = false;
                break;
            }
        }

        if (valid) {
            double vol = rem_area * h;
            if (vol > max_vol) {
                max_vol = vol;
            }
        }
    }

    cout << fixed << setprecision(2) << max_vol ;

    return 0;
}
