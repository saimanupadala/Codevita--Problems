#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct Line {
    long long x1, y1, x2, y2;
};

bool is_covered(long long a, long long b, const vector<pair<long long, long long>>& segs) {
    if (a > b) swap(a, b);
    vector<pair<long long, long long>> intervals;
    for (auto& s : segs) {
        long long sx1 = min(s.first, s.second);
        long long sx2 = max(s.first, s.second);
        if (sx2 >= a && sx1 <= b) {
            intervals.push_back({max(sx1, a), min(sx2, b)});
        }
    }
    if (intervals.empty()) return false;
    sort(intervals.begin(), intervals.end());
    long long current_start = intervals[0].first;
    long long current_end = intervals[0].second;
    if (current_start > a) return false;

    for (size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i].first <= current_end) {
            current_end = max(current_end, intervals[i].second);
        } else {
            break;
        }
    }
    return current_start <= a && current_end >= b;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<Line> h_lines, v_lines;
    for (int i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        if (y1 == y2) {
            h_lines.push_back({min(x1, x2), y1, max(x1, x2), y1});
        } else if (x1 == x2) {
            v_lines.push_back({x1, min(y1, y2), x1, max(y1, y2)});
        }
    }

    set<pair<long long, long long>> points;
    for (size_t i = 0; i < h_lines.size(); ++i) {
        for (size_t j = 0; j < v_lines.size(); ++j) {
            long long hx1 = h_lines[i].x1, hx2 = h_lines[i].x2, hy = h_lines[i].y1;
            long long vx = v_lines[j].x1, vy1 = v_lines[j].y1, vy2 = v_lines[j].y2;

            if (vx >= hx1 && vx <= hx2 && hy >= vy1 && hy <= vy2) {
                points.insert({vx, hy});
            }
        }
    }

    vector<pair<long long, long long>> pts(points.begin(), points.end());
    int count = 0;
    int m = pts.size();

    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j < m; ++j) {
            long long x1 = pts[i].first, y1 = pts[i].second;
            long long x2 = pts[j].first, y2 = pts[j].second;

            if (x1 < x2 && y1 < y2) {
                if (points.count({x2, y1}) && points.count({x1, y2})) {
                    vector<pair<long long, long long>> h_at_y1, h_at_y2;
                    vector<pair<long long, long long>> v_at_x1, v_at_x2;

                    for (auto& hl : h_lines) {
                        if (hl.y1 == y1) h_at_y1.push_back({hl.x1, hl.x2});
                        if (hl.y1 == y2) h_at_y2.push_back({hl.x1, hl.x2});
                    }
                    for (auto& vl : v_lines) {
                        if (vl.x1 == x1) v_at_x1.push_back({vl.y1, vl.y2});
                        if (vl.x1 == x2) v_at_x2.push_back({vl.y1, vl.y2});
                    }

                    bool bottom = is_covered(x1, x2, h_at_y1);
                    bool top = is_covered(x1, x2, h_at_y2);
                    bool left = is_covered(y1, y2, v_at_x1);
                    bool right = is_covered(y1, y2, v_at_x2);

                    if (top && bottom && left && right) {
                        count++;
                    }
                }
            }
        }
    }

    cout << count;

    return 0;
}
