class Solution {
public:
    bool hasValidPath(vector<vector<char>>& grid) {
        int m = grid.size();
        int n = grid[0].size();

        // A valid parentheses string must have even length
        if ((m + n - 1) % 2 == 1)
            return false;

        // Starting cell must be '('
        if (grid[0][0] == ')')
            return false;

        int maxBalance = m + n;

        vector<vector<vector<bool>>> dp(
            m,
            vector<vector<bool>>(n, vector<bool>(maxBalance + 1, false))
        );

        dp[0][0][1] = true;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {

                if (i == 0 && j == 0)
                    continue;

                int change = (grid[i][j] == '(' ? 1 : -1);

                for (int balance = 0; balance <= maxBalance; balance++) {

                    int previousBalance = balance - change;

                    if (previousBalance < 0)
                        continue;

                    if (i > 0 && dp[i - 1][j][previousBalance])
                        dp[i][j][balance] = true;

                    if (j > 0 && dp[i][j - 1][previousBalance])
                        dp[i][j][balance] = true;
                }
            }
        }

        return dp[m - 1][n - 1][0];
    }
};