class Solution {
public:
    vector<int> countServers(int n, vector<vector<int>>& logs, int x, vector<int>& queries) {
        sort(logs.begin(), logs.end(), [](vector<int>& a, vector<int>& b) {
            return a[1] < b[1];
        });

        int m = queries.size();

        vector<pair<int,int>> q;
        for (int i = 0; i < m; i++) {
            q.push_back({queries[i], i});
        }

        sort(q.begin(), q.end());

        vector<int> ans(m, 0), freq(n + 1, 0);

        int l = 0, r = 0, cnt = 0;

        for (auto [time, idx] : q) {
            while (r < logs.size() && logs[r][1] <= time) {
                int id = logs[r][0];

                if (freq[id] == 0) cnt++;

                freq[id]++;
                r++;
            }

            while (l < r && logs[l][1] < time - x) {
                int id = logs[l][0];

                freq[id]--;

                if (freq[id] == 0) cnt--;

                l++;
            }

            ans[idx] = n - cnt;
        }

        return ans;
    }
};