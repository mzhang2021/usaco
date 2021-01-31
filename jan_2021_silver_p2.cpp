#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, q;
    string fence;
    cin >> n >> q >> fence;

    vector<int> pref(n), lastPosPref(26, -1);
    for (int i=0; i<n; i++) {
        if (i > 0)
            pref[i] = pref[i-1];
        if (lastPosPref[fence[i]-'A'] == -1) {
            // never seen this color before
            pref[i]++;
        } else {
            // check if there is no smaller letter in between the two occurrences of this letter
            bool ok = true;
            for (int j=0; j<fence[i]-'A'; j++)
                if (lastPosPref[j] > lastPosPref[fence[i]-'A']) {
                    ok = false;
                    break;
                }
            if (!ok) {
                // need a new stroke
                pref[i]++;
            }
        }
        lastPosPref[fence[i]-'A'] = i;
    }

    vector<int> suff(n), lastPosSuff(26, n);
    for (int i=n-1; i>=0; i--) {
        if (i + 1 < n)
            suff[i] = suff[i+1];
        if (lastPosSuff[fence[i]-'A'] == n) {
            // never seen this color before
            suff[i]++;
        } else {
            // check if there is no smaller letter in between the two occurrences of this letter
            bool ok = true;
            for (int j=0; j<fence[i]-'A'; j++)
                if (lastPosSuff[j] < lastPosSuff[fence[i]-'A']) {
                    ok = false;
                    break;
                }
            if (!ok) {
                // need a new stroke
                suff[i]++;
            }
        }
        lastPosSuff[fence[i]-'A'] = i;
    }

    for (int i=0; i<q; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;

        int answer = 0;
        if (a > 0)
            answer += pref[a-1];
        if (b + 1 < n)
            answer += suff[b+1];
        cout << answer << "\n";
    }

    return 0;
}
