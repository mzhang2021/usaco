#include <bits/stdc++.h>
using namespace std;

int id = 0, answer[300005];
vector<pair<int, int>> adjInit[100005];
map<int, pair<int, int>> adjPosTime[100005];
map<int, int> cycleId[100005];

int main() {
    int n, k;
    cin >> n >> k;
    for (int i=0; i<k; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        adjInit[a].emplace_back(b, i);
        adjInit[b].emplace_back(a, i);
    }

    // (position, time of arrival)
    for (int i=0; i<n; i++)
        for (int j=0; j<(int)adjInit[i].size(); j++) {
            // edges -> (other position it's connected to, time)
            int t = adjInit[i][j].second;
            adjPosTime[i][t] = adjInit[i][(j+1)%adjInit[i].size()];
        }

    // (i, 0)
    // (i, last time in i's adjacency list)
    for (int i=0; i<n; i++) {
        if (adjInit[i].empty()) {
            cout << "1\n";
            continue;
        }
        int t = adjInit[i].back().second;
        if (!cycleId[i].count(t)) {
            // explore the cycle containing (i, t)
            // (3, 4), (3, 6)
            int u = i;
            set<int> st;
            while (!cycleId[u].count(t)) {
                cycleId[u][t] = id;
                st.insert(u);
                pair<int, int> nxt = adjPosTime[u][t];
                u = nxt.first;
                t = nxt.second;
            }
            answer[id] = st.size();
            id++;
        }
        cout << answer[cycleId[i][t]] << "\n";
    }

    return 0;
}
