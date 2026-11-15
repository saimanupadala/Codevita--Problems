#include <iostream>
#include <algorithm>
using namespace std;

struct S { int x1, y1, x2, y2; };
S s[60];
int n;

bool contains(const S &a, int px, int py) {
    if (px < a.x1 || px > a.x2) return false;
    return (py - a.y1) * (a.x2 - a.x1) == (px - a.x1) * (a.y2 - a.y1);
}

// (x,y) nundi (dx,dy) direction lo ee slide continue avthunda
bool arm(int x, int y, int dx, int dy) {
    for (int i = 0; i < n; i++)
        if (contains(s[i], x, y) && contains(s[i], x + dx, y + dy))
            return true;
    return false;
}

bool onAny(int x, int y) {
    for (int i = 0; i < n; i++)
        if (contains(s[i], x, y)) return true;
    return false;
}

int main() {
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> s[i].x1 >> s[i].y1 >> s[i].x2 >> s[i].y2;
        if (s[i].x1 > s[i].x2) {
            swap(s[i].x1, s[i].x2);
            swap(s[i].y1, s[i].y2);
        }
    }
    int x, y;
    long long E;
    cin >> x >> y >> E;

    bool landed = onAny(x, y);

    while (true) {
        if (!landed) {
            if (y <= 0) { cout << x << " " << 0; return 0; }
            int best = -1;
            for (int i = 0; i < n; i++) {
                if (s[i].x1 == s[i].x2) continue;
                if (x < s[i].x1 || x > s[i].x2) continue;
                int yy = s[i].y1 + (x - s[i].x1) * (s[i].y2 - s[i].y1) / (s[i].x2 - s[i].x1);
                if (yy < y && yy > best) best = yy;
            }
            if (best < 0) { cout << x << " " << 0; return 0; }
            y = best;
        }
        landed = false;

        // slide meeda jaarey loop
        while (true) {
            if (y == 0) { cout << x << " " << y; return 0; }

            bool UL = arm(x, y, -1, 1), UR = arm(x, y, 1, 1);
            bool DL = arm(x, y, -1, -1), DR = arm(x, y, 1, -1);

            if (UL && UR) {                 // V shape, stuck
                long long cost = (long long)x * y;
                if (E < cost) { cout << x << " " << y; return 0; }
                E -= cost;
            }

            int d = 0;
            if (DL && !DR) d = -1;
            else if (DR && !DL) d = 1;
            if (d == 0) break;              // slide ayipoindi, gravity

            if (E == 0) { cout << x << " " << y; return 0; }
            x += d;
            y -= 1;
            E -= 1;
        }
    }
    return 0;
}
