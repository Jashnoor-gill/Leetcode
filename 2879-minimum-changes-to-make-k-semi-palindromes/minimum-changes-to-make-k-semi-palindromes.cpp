
class Solution {
public:
    int minimumChanges(string s, int k) {
        int n = s.size();
        const int INF = 1e9;

        // Step 1: Precompute cost of every substring
        vector<vector<int>> cost(n, vector<int>(n, INF));

        for (int len = 2; len <= n; len++) {
            for (int i = 0; i + len <= n; i++) {

                // Try every proper divisor
                for (int d = 1; d < len; d++) {

                    if (len % d != 0) continue;

                    int changes = 0;

                    // Form d sequences
                    for (int r = 0; r < d; r++) {

                        int l = i + r;
                        int right = i + r +
                            ((len - 1 - r) / d) * d;

                        // Make each sequence a palindrome
                        while (l < right) {
                            if (s[l] != s[right])
                                changes++;

                            l += d;
                            right -= d;
                        }
                    }

                    cost[i][i + len - 1] =
                        min(cost[i][i + len - 1], changes);
                }
            }
        }

        // Step 2: DP
        vector<vector<int>> dp(
            k + 1, vector<int>(n + 1, INF)
        );

        dp[0][0] = 0;

        for (int p = 1; p <= k; p++) {

            for (int i = 2 * p; i <= n; i++) {

                // Start of the last substring
                for (int j = 2 * (p - 1); j <= i - 2; j++) {

                    if (dp[p - 1][j] == INF)
                        continue;

                    dp[p][i] = min(
                        dp[p][i],
                        dp[p - 1][j] + cost[j][i - 1]
                    );
                }
            }
        }

        return dp[k][n];
    }
};
