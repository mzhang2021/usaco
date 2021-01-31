#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<vector<int>> row(n, vector<int>(2)), col(n, vector<int>(2));
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++) {
            int a;
            cin >> a;
            row[i][j % 2] += a;
            col[j][i % 2] += a;
        }

    // case 1: alternating cow, no-cow in each row
    int answer1 = 0;
    for (int i=0; i<n; i++)
        answer1 += max(row[i][0], row[i][1]);

    // case 2: each col
    int answer2 = 0;
    for (int i=0; i<n; i++)
        answer2 += max(col[i][0], col[i][1]);

    cout << max(answer1, answer2) << "\n";

    return 0;
}
