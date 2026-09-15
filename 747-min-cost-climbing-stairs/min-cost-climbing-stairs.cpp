class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int p2 = cost[0];
        int p1 = cost[1];

        int n = cost.size();

        for (int i = 2; i < n; i++) {
            int curr = min(p1, p2) + cost[i];

            p2 = p1;
            p1 = curr;
        }

        return min(p1, p2);
    }
};