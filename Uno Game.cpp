#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>

using namespace std;

int parent_arr[35];
int find_set(int v) {
    if (v == parent_arr[v])
        return v;
    return parent_arr[v] = find_set(parent_arr[v]);
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        parent_arr[b] = a;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<string> names(n);
    map<string, int> nameToId;
    for (int i = 0; i < n; ++i) {
        cin >> names[i];
        nameToId[names[i]] = i;
    }

    vector<int> skills(n);
    for (int i = 0; i < n; ++i) {
        cin >> skills[i];
    }

    for (int i = 0; i < n; ++i) parent_arr[i] = i;

    int n1;
    cin >> n1;
    for (int i = 0; i < n1; ++i) {
        string u, v;
        cin >> u >> v;
        union_sets(nameToId[u], nameToId[v]);
    }

    int n2;
    cin >> n2;
    vector<pair<int, int> > rivals;
    for (int i = 0; i < n2; ++i) {
        string u, v;
        cin >> u >> v;
        rivals.push_back(make_pair(nameToId[u], nameToId[v]));
    }

    int skillLimit;
    cin >> skillLimit;

    map<int, vector<int> > components;
    for (int i = 0; i < n; ++i) {
        int root = find_set(i);
        components[root].push_back(i);
    }

    vector<vector<int> > compList;
    vector<int> compCost;
    vector<int> compSize;

    for (map<int, vector<int> >::iterator it = components.begin();
         it != components.end(); ++it) {
        vector<int> &group = it->second;
        int totalSkill = 0;
        for (size_t k = 0; k < group.size(); ++k) {
            totalSkill += skills[group[k]];
        }
        compList.push_back(group);
        compCost.push_back(totalSkill);
        compSize.push_back((int)group.size());
    }

    int numComps = (int)compList.size();
    vector<int> playerComp(n, -1);
    for (int i = 0; i < numComps; ++i) {
        for (size_t k = 0; k < compList[i].size(); ++k) {
            playerComp[compList[i][k]] = i;
        }
    }

    vector<int> rivalMask(numComps, 0);
    for (size_t k = 0; k < rivals.size(); ++k) {
        int c1 = playerComp[rivals[k].first];
        int c2 = playerComp[rivals[k].second];
        if (c1 != c2) {
            rivalMask[c1] |= (1 << c2);
            rivalMask[c2] |= (1 << c1);
        } else {
            rivalMask[c1] |= (1 << c1);
        }
    }

    int maxPlayers = 0;
    int totalSubmasks = (1 << numComps);

    for (int mask = 0; mask < totalSubmasks; ++mask) {
        bool isValid = true;
        int currentCost = 0;
        int currentCount = 0;

        for (int i = 0; i < numComps; ++i) {
            if ((mask >> i) & 1) {
                if (rivalMask[i] & mask) {
                    isValid = false;
                    break;
                }
                currentCost += compCost[i];
                currentCount += compSize[i];
            }
        }

        if (isValid && currentCost <= skillLimit) {
            maxPlayers = max(maxPlayers, currentCount);
        }
    }

    cout << maxPlayers;
    return 0;
}
