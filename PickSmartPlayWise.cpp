#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cmath>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> PL;

const double PI = 3.14159265358979323846;

map<PL, int> idMap;
vector<double> nx, ny;          // node coordinates (rounded to 2 decimals)

int getId(double x, double y) {
    ll kx = (ll)floor(x * 100.0 + 0.5);
    ll ky = (ll)floor(y * 100.0 + 0.5);
    PL key = make_pair(kx, ky);
    map<PL, int>::iterator it = idMap.find(key);
    if (it != idMap.end()) return it->second;
    int id = (int)nx.size();
    idMap[key] = id;
    nx.push_back(kx / 100.0);
    ny.push_back(ky / 100.0);
    return id;
}

int main() {
    int n;
    cin >> n;
    vector<ll> X1(n), Y1(n), X2(n), Y2(n);
    for (int i = 0; i < n; i++)
        cin >> X1[i] >> Y1[i] >> X2[i] >> Y2[i];

    // har stick meeda unna points: (t, nodeId)
    vector< vector< pair<double, int> > > pts(n);
    for (int i = 0; i < n; i++) {
        pts[i].push_back(make_pair(0.0, getId((double)X1[i], (double)Y1[i])));
        pts[i].push_back(make_pair(1.0, getId((double)X2[i], (double)Y2[i])));
    }

    // sticks madhya intersections
    for (int i = 0; i < n; i++) {
        ll ax = X1[i], ay = Y1[i];
        ll r0 = X2[i] - ax, r1 = Y2[i] - ay;
        for (int j = i + 1; j < n; j++) {
            ll cx = X1[j], cy = Y1[j];
            ll s0 = X2[j] - cx, s1 = Y2[j] - cy;
            ll den = r0 * s1 - r1 * s0;
            if (den == 0) continue;           // parallel
            ll tn = (cx - ax) * s1 - (cy - ay) * s0;
            ll un = (cx - ax) * r1 - (cy - ay) * r0;
            if (den < 0) { den = -den; tn = -tn; un = -un; }
            if (tn < 0 || tn > den || un < 0 || un > den) continue;
            double t = (double)tn / den;
            double u = (double)un / den;
            double px = ax + t * r0;
            double py = ay + t * r1;
            int id = getId(px, py);
            pts[i].push_back(make_pair(t, id));
            pts[j].push_back(make_pair(u, id));
        }
    }

    // sticks ni split chesi graph edges
    set< pair<int, int> > edges;
    for (int i = 0; i < n; i++) {
        sort(pts[i].begin(), pts[i].end());
        for (size_t k = 0; k + 1 < pts[i].size(); k++) {
            int a = pts[i][k].second, b = pts[i][k + 1].second;
            if (a == b) continue;
            edges.insert(make_pair(min(a, b), max(a, b)));
        }
    }

    int V = (int)nx.size();
    vector< set<int> > adj(V);
    for (set< pair<int, int> >::iterator it = edges.begin(); it != edges.end(); ++it) {
        adj[it->first].insert(it->second);
        adj[it->second].insert(it->first);
    }

    // dangling ends ni teesesthe closed figure migulutundi
    vector<bool> alive(V, true);
    bool changed = true;
    while (changed) {
        changed = false;
        for (int v = 0; v < V; v++) {
            if (alive[v] && adj[v].size() <= 1) {
                alive[v] = false;
                changed = true;
                for (set<int>::iterator it = adj[v].begin(); it != adj[v].end(); ++it)
                    adj[*it].erase(v);
                adj[v].clear();
            }
        }
    }

    int start = -1;
    for (int v = 0; v < V; v++)
        if (alive[v]) { start = v; break; }

    if (start == -1) {
        cout << "Abandoned";
        return 0;
    }

    // cycle lo walk cheyyali
    vector<int> order;
    order.push_back(start);
    int prev = -1, cur = start;
    for (int guard = 0; guard < 10000; guard++) {
        int nxt = -1;
        for (set<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); ++it)
            if (*it != prev) { nxt = *it; break; }
        if (nxt == -1 || nxt == start) break;
        order.push_back(nxt);
        prev = cur;
        cur = nxt;
    }

    int m = (int)order.size();
    double area2 = 0, used = 0;
    for (int k = 0; k < m; k++) {
        int a = order[k], b = order[(k + 1) % m];
        area2 += nx[a] * ny[b] - nx[b] * ny[a];
        used += sqrt((nx[b] - nx[a]) * (nx[b] - nx[a]) + (ny[b] - ny[a]) * (ny[b] - ny[a]));
    }
    double kalyan = fabs(area2) / 2.0;

    double total = 0;
    for (int i = 0; i < n; i++) {
        double dx = (double)(X2[i] - X1[i]);
        double dy = (double)(Y2[i] - Y1[i]);
        total += sqrt(dx * dx + dy * dy);
    }
    double left = total - used;
    if (left < 0) left = 0;
    double computer = left * left / (4.0 * PI);   // leftover tho best figure = circle

    if (kalyan > computer) cout << "Kalyan";
    else cout << "Computer";
    return 0;
}
