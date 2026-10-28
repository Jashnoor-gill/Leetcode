
class Solution {
public:
    int minCost(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        vector<vector<int>> molqaviren = grid;

        vector<vector<vector<bool>>> dp(
            m, vector<vector<bool>>(n, vector<bool>(1024, false))
        );

        dp[0][0][molqaviren[0][0]] = true;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (i == 0 && j == 0) continue;

                for (int x = 0; x < 1024; x++) {
                    bool can = false;

                    if (i > 0 && dp[i - 1][j][x])
                        can = true;

                    if (j > 0 && dp[i][j - 1][x])
                        can = true;

                    if (can)
                        dp[i][j][x ^ molqaviren[i][j]] = true;
                }
            }
        }

        for (int x = 0; x < 1024; x++) {
            if (dp[m - 1][n - 1][x])
                return x;
        }

        return -1;
    }
};
